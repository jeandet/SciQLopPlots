/*------------------------------------------------------------------------------
-- This file is a part of the SciQLop Software
-- Copyright (C) 2023, Plasma Physics Laboratory - CNRS
--
-- This program is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation; either version 2 of the License, or
-- (at your option) any later version.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with this program; if not, write to the Free Software
-- Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
-------------------------------------------------------------------------------*/
/*-- Author : Alexis Jeandet
-- Mail : alexis.jeandet@member.fsf.org
----------------------------------------------------------------------------*/

#include "SciQLopPlots/Plotables/Resamplers/SciQLopColorMapResampler.hpp"
#include <cpp_utils/containers/algorithms.hpp>
#include <iostream>

#include "SciQLopPlots/Profiling.hpp"

inline std::vector<double> _generate_range(double start, double end, std::size_t n,
                                           bool log = false)
{
    std::vector<double> range(n);
    if (log)
    {
        auto step = std::log10((end - start)) / n;
        std::generate(std::begin(range), std::end(range),
                      [start, step, i = 0]() mutable { return start * std::pow(10, i++ * step); });
    }
    else
    {
        auto step = (end - start) / n;
        std::generate(std::begin(range), std::end(range),
                      [start, step, i = 0]() mutable { return start + i++ * step; });
    }
    return range;
}

inline double _nan_safe_min(const double a, const double b)
{
    if (std::isnan(a))
    {
        return b;
    }
    if (std::isnan(b))
    {
        return a;
    }
    return std::min(a, b);
}

inline double _nan_safe_max(const double a, const double b)
{
    if (std::isnan(a))
    {
        return b;
    }
    if (std::isnan(b))
    {
        return a;
    }
    return std::max(a, b);
}

inline std::pair<double, double> _y_bounds(const XYZView& v, std::size_t row, std::size_t n_cols,
                                           double min = std::nan(""), double max = std::nan(""))
{
    {
        auto j = 0UL;
        auto val = v.y(row, j);
        while (std::isnan(val) && j < n_cols)
        {
            j++;
            val = v.y(row, j);
        }
        min = _nan_safe_min(val, min);
    }
    {
        auto j = n_cols - 1;
        auto val = v.y(row, j);
        while (std::isnan(val) && j > 0)
        {
            j--;
            val = v.y(row, j);
        }
        max = _nan_safe_max(val, max);
    }

    return { min, max };
}

inline std::pair<double, double> _y_bounds(const XYZView& v)
{
    using namespace cpp_utils;
    auto shape = v.y_shape();
    if (!v.y_is_2d())
    {
        return _y_bounds(v, 0, shape.second);
    }
    else
    {
        auto bounds = std::pair { std::nan(""), std::nan("") };
        for (auto i = 0UL; i < shape.first; i++)
        {
            bounds = _y_bounds(v, i, shape.second, bounds.first, bounds.second);
        }
        return bounds;
    }
}

template <typename T>
inline void _divide(QCPColorMapData* data, const T& avg_count, const std::size_t n_x,
                    const std::size_t n_y)
{
    PROFILE_HERE;
    for (auto i = 0UL; i < n_x; i++)
    {
        for (auto j = 0UL; j < n_y; j++)
        {
            auto count = avg_count[i * n_y + j];
            if (count != 0)
            {
                data->setCell(i, j, data->cell(i, j) / count);
            }
            else
            {
                data->setCell(i, j, std::nan(""));
            }
        }
    }
}

template <typename T>
inline void _average_value(const XYZView& view, QCPColorMapData* data, T& avg_count,
                           std::size_t x_src_idx, std::size_t x_dest_idx, std::size_t y_src_idx,
                           std::size_t y_dest_idx, std::size_t n_y)
{
    auto z_val = view.z(x_src_idx, y_src_idx);
    data->setCell(x_dest_idx, y_dest_idx, data->cell(x_dest_idx, y_dest_idx) + z_val);
    avg_count[x_dest_idx * n_y + y_dest_idx] += 1;
}

template <typename T>
inline void _y_loop(const XYZView& view, QCPColorMapData* data, const T& y_axis, T& avg_count,
                    std::size_t x_src_idx, std::size_t x_dest_idx)
{
    auto n_y = std::size(y_axis);
    auto y_dest_idx = 0UL;
    for (auto y_src_idx = 0UL; y_src_idx < view.z_shape().second; y_src_idx++)
    {
        auto y_val = view.y(x_src_idx, y_src_idx);
        while (y_dest_idx < (n_y - 1) && y_val > y_axis[y_dest_idx])
        {
            if (y_src_idx > 0 && y_src_idx != view.z_shape().second - 1)
                _average_value(view, data, avg_count, x_src_idx, x_dest_idx, y_src_idx - 1,
                               y_dest_idx, n_y);
            y_dest_idx++;
        }
        _average_value(view, data, avg_count, x_src_idx, x_dest_idx, y_src_idx, y_dest_idx, n_y);
    }
}

template <typename T>
inline void _x_loop(const XYZView& view, QCPColorMapData* data, const T& x_axis, const T& y_axis,
                    T& avg_count)
{
    PROFILE_HERE;
    auto n_x = std::size(x_axis);
    auto x_dest_idx = 0UL;
    auto prev_x_val = view.x(0);
    auto dx = view.x(1) - prev_x_val;
    auto prev_dx = dx;
    for (auto x_src_idx = 0UL; x_src_idx < std::size(view); x_src_idx++)
    {
        auto x_val = view.x(x_src_idx);
        dx = x_val - prev_x_val;
        while (x_dest_idx < (n_x - 1) && x_val > x_axis[x_dest_idx])
        {
            if (x_src_idx < std::size(view) - 1)
            {
                auto next_dx = view.x(x_src_idx + 1) - x_val;
                if ((next_dx < (1.5 * dx))
                    && (dx < 1.5 * prev_dx)) // data gap criterion (50%) increase in dx
                {
                    _y_loop(view, data, y_axis, avg_count, x_src_idx, x_dest_idx);
                }
            }
            x_dest_idx++;
        }
        _y_loop(view, data, y_axis, avg_count, x_src_idx, x_dest_idx);
        prev_x_val = x_val;
        prev_dx = dx;
    }
}

template <typename T>
inline void _copy_and_average(const XYZView& view, QCPColorMapData* data, const T& x_axis,
                              const T& y_axis)
{
    PROFILE_HERE;
    auto n_x = std::size(x_axis);
    auto n_y = std::size(y_axis);
    std::vector<double> avg_count(n_x * n_y, 0);
    _x_loop(view, data, x_axis, y_axis, avg_count);
    _divide(data, avg_count, n_x, n_y);
}

auto _generate_axes(const XYZView& view, std::size_t max_x_size, std::size_t max_y_size, bool log)
{
    auto y_bounds = _y_bounds(view);
    auto x_bounds = std::pair { view.x(0), view.x(std::size(view) - 1) };
    auto n_y = std::min(max_y_size, view.y_shape().second * 2);
    auto n_x = std::min(max_x_size, std::size(view) * 2);
    auto x_axis = _generate_range(x_bounds.first, x_bounds.second, n_x);
    auto y_axis = _generate_range(y_bounds.first, y_bounds.second, n_y, log);
    return std::pair { x_axis, y_axis };
}

void ColormapResampler::_resample_impl(const PyBuffer& x, const PyBuffer& y, const PyBuffer& z,
                                       const QCPRange new_range, bool new_data)
{
    PROFILE_HERE_N("ColormapResampler::_resample_impl");
    if (x.data() != nullptr && x.flat_size())
    {
        const auto view = XYZView(x, y, z, new_range.lower, new_range.upper);
        if (std::size(view) > 10) // less does not make much sense
        {
            auto [x_axis, y_axis] = _generate_axes(view, this->_max_x_size, this->_max_y_size,
                                                   _log_scale.loadRelaxed());
            QCPColorMapData* data = new QCPColorMapData(std::size(x_axis), std::size(y_axis),
                                                        { x_axis.front(), x_axis.back() },
                                                        { y_axis.front(), y_axis.back() });
            _copy_and_average(view, data, x_axis, y_axis);
            data->recalculateDataBounds();
            Q_EMIT setGraphData(data);
        }
    }
}

ColormapResampler::ColormapResampler(QCPAxis::ScaleType scale_type)
        : AbstractResampler2d {}, _log_scale { scale_type == QCPAxis::stLogarithmic }
{
}
