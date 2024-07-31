/*------------------------------------------------------------------------------
-- This file is a part of the SciQLop Software
-- Copyright (C) 2024, Plasma Physics Laboratory - CNRS
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
#include "SciQLopPlots/Plotables/SciQLopCurveResampler.hpp"

void CurveResampler::_resample_slot(const QCPRange newRange)
{
    QMutexLocker locker(&_data_mutex);
    if (_x.data() != nullptr && _x.flat_size() > 0)
    {


        const auto y_incr = (_dataOrder == SciQLopGraph::DataOrder::xFirst) ? 1UL : _line_cnt;
        for (auto line_index = 0UL; line_index < _line_cnt; line_index++)
        {
            const auto count = std::size(_x);
            const auto start_y = _y.data()
                + (line_index
                    * ((_dataOrder == SciQLopGraph::DataOrder::xFirst) ? _x.flat_size() : 1));
            emit this->setGraphData(line_index, copy_data(_x.data(), start_y, count, y_incr));
        }
        _x.release();
        _y.release();
    }
    emit this->refreshPlot();
}

CurveResampler::CurveResampler(SciQLopGraph::DataOrder dataOrder, std::size_t line_cnt)
        : _dataOrder { dataOrder }, _line_cnt { line_cnt }
{

    connect(this, &CurveResampler::_resample_sig, this, &CurveResampler::_resample_slot,
        Qt::QueuedConnection);
}

void CurveResampler::resample(const QCPRange newRange)
{
    emit this->_resample_sig(newRange);
}
