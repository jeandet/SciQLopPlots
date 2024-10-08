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
#pragma once
#include "SciQLopPlots/Plotables/SciQLopGraphInterface.hpp"
#include "SciQLopPlots/Python/PythonInterface.hpp"
#include "SciQLopPlots/SciQLopPlotAxis.hpp"
#include "SciQLopPlots/enums.hpp"
#include <QPointF>
#include <qcustomplot.h>

class SciQLopPlotInterface : public QWidget
{
    Q_OBJECT
protected:
    QList<SciQLopPlotAxisInterface*> m_axes_to_rescale;
    QList<SciQLopPlotAxisInterface*> m_frozen_axes;

    inline virtual SciQLopGraphInterface* plot_impl(const PyBuffer& x, const PyBuffer& y,
        QStringList labels = QStringList(), QList<QColor> colors = QList<QColor>(),
        ::GraphType graph_type = ::GraphType::Line)
    {
        throw std::runtime_error("Not implemented");
    }

    inline virtual SciQLopGraphInterface* plot_impl(const PyBuffer& x, const PyBuffer& y,
        const PyBuffer& z, QString name = QStringLiteral("ColorMap"), bool y_log_scale = false,
        bool z_log_scale = false)
    {
        throw std::runtime_error("Not implemented");
    }

    inline virtual SciQLopGraphInterface* plot_impl(GetDataPyCallable callable,
        QStringList labels = QStringList(), QList<QColor> colors = QList<QColor>(),
        ::GraphType graph_type = ::GraphType::Line, QObject* sync_with = nullptr)
    {
        throw std::runtime_error("Not implemented");
    }

    inline virtual SciQLopGraphInterface* plot_impl(GetDataPyCallable callable,
        QString name = QStringLiteral("ColorMap"), bool y_log_scale = false,
        bool z_log_scale = false, QObject* sync_with = nullptr)
    {
        throw std::runtime_error("Not implemented");
    }

public:
    SciQLopPlotInterface(QWidget* parent = nullptr) : QWidget(parent) { }
    virtual ~SciQLopPlotInterface() = default;

    inline virtual SciQLopPlotAxisInterface* time_axis() const noexcept { return nullptr; }
    inline virtual SciQLopPlotAxisInterface* x_axis() const noexcept { return nullptr; }
    inline virtual SciQLopPlotAxisInterface* y_axis() const noexcept { return nullptr; }
    inline virtual SciQLopPlotAxisInterface* z_axis() const noexcept { return nullptr; }
    inline virtual SciQLopPlotAxisInterface* y2_axis() const noexcept { return nullptr; }
    inline virtual SciQLopPlotAxisInterface* x2_axis() const noexcept { return nullptr; }

    inline virtual SciQLopPlotAxisInterface* axis(AxisType axis) const noexcept
    {
        switch (axis)
        {
            case AxisType::TimeAxis:
                return time_axis();
            case AxisType::XAxis:
                return x_axis();
            case AxisType::YAxis:
                return y_axis();
            case AxisType::ZAxis:
                return z_axis();
            default:
                return nullptr;
        }
    }

    virtual SciQLopPlotAxisInterface* axis(Qt::AnchorPoint pos, int index = 0) const noexcept
    {
        if (index == 0)
        {

            switch (pos)
            {
                case Qt::AnchorPoint::AnchorBottom:
                    return x_axis();
                case Qt::AnchorPoint::AnchorLeft:
                    return y_axis();
                case Qt::AnchorPoint::AnchorRight:
                    return y2_axis();
                case Qt::AnchorPoint::AnchorTop:
                    return x2_axis();
                default:
                    break;
            }
        }
        return nullptr;
    }


    virtual void set_scroll_factor(double factor) noexcept { }
    virtual double scroll_factor() const noexcept { return std::nan(""); }

    virtual void enable_cursor(bool enable = true) noexcept { }
    virtual void enable_legend(bool show = true) noexcept { }

    virtual void minimize_margins() { }

    virtual void replot(bool immediate = false) { }


    inline virtual void rescale_axes() noexcept { rescale_axes(m_axes_to_rescale); }
    inline virtual void rescale_axes(const QList<SciQLopPlotAxisInterface*>& axes) noexcept
    {
        for (auto ax : axes)
        {
            ax->rescale();
        }
    }


    inline virtual SciQLopGraphInterface* line(const PyBuffer& x, const PyBuffer& y,
        QStringList labels = QStringList(), QList<QColor> colors = QList<QColor>())
    {
        return plot_impl(x, y, labels, colors, ::GraphType::Line);
    }

    inline virtual SciQLopGraphInterface* parametric_curve(const PyBuffer& x, const PyBuffer& y,
        QStringList labels = QStringList(), QList<QColor> colors = QList<QColor>())
    {
        return plot_impl(x, y, labels, colors, ::GraphType::ParametricCurve);
    }

    inline virtual SciQLopGraphInterface* colormap(const PyBuffer& x, const PyBuffer& y,
        const PyBuffer& z, QString name = QStringLiteral("ColorMap"), bool y_log_scale = false,
        bool z_log_scale = false)
    {
        return plot_impl(x, y, z, name, y_log_scale, z_log_scale);
    }


    inline virtual SciQLopGraphInterface* line(GetDataPyCallable callable,
        QStringList labels = QStringList(), QList<QColor> colors = QList<QColor>(),
        QObject* sync_with = nullptr)
    {
        return plot_impl(callable, labels, colors, ::GraphType::Line, sync_with);
    }
    inline virtual SciQLopGraphInterface* parametric_curve(GetDataPyCallable callable,
        QStringList labels = QStringList(), QList<QColor> colors = QList<QColor>(),
        QObject* sync_with = nullptr)
    {
        return plot_impl(callable, labels, colors, ::GraphType::ParametricCurve, sync_with);
    }
    inline virtual SciQLopGraphInterface* colormap(GetDataPyCallable callable,
        QString name = QStringLiteral("ColorMap"), bool y_log_scale = false,
        bool z_log_scale = false, QObject* sync_with = nullptr)
    {
        return plot_impl(callable, name, y_log_scale, z_log_scale, sync_with);
    }


    inline virtual SciQLopGraphInterface* graph(int index = -1)
    {
        throw std::runtime_error("Not implemented");
    }

    inline virtual SciQLopGraphInterface* graph(const QString& name)
    {
        throw std::runtime_error("Not implemented");
    }

#ifndef BINDINGS_H
    Q_SIGNAL void scroll_factor_changed(double factor);
    Q_SIGNAL void x_axis_range_changed(double lower, double upper);
    Q_SIGNAL void x2_axis_range_changed(double lower, double upper);
    Q_SIGNAL void y_axis_range_changed(double lower, double upper);
    Q_SIGNAL void z_axis_range_changed(double lower, double upper);
    Q_SIGNAL void y2_axis_range_changed(double lower, double upper);
    Q_SIGNAL void time_axis_range_changed(double lower, double upper);
#endif
protected:
    inline virtual QList<SciQLopPlotAxisInterface*> axes_to_rescale() const noexcept
    {
        return m_axes_to_rescale;
    }
    inline virtual void set_axes_to_rescale(QList<SciQLopPlotAxisInterface*>&& axes)
    {
        m_axes_to_rescale = axes;
    }

    inline void freeze_axis(SciQLopPlotAxisInterface* axis) noexcept { m_frozen_axes.append(axis); }

    virtual QList<SciQLopPlotAxisInterface*> selected_axes() const noexcept { return {}; }
    virtual SciQLopPlotAxisInterface* axis_at(const QPointF& pos) const noexcept { return nullptr; }

    inline virtual void keyPressEvent(QKeyEvent* event) override
    {
        QList<SciQLopPlotAxisInterface*> axes;
        QList<SciQLopPlotAxisInterface*> axes_to_rescale;
        if (auto ax = axis_at(mapFromGlobal(QCursor::pos())))
            axes.append(ax);
        else
            axes = selected_axes();
        if (axes.isEmpty())
            axes_to_rescale = m_axes_to_rescale;
        else
            axes_to_rescale = axes;

        for (auto ax : m_frozen_axes)
            axes_to_rescale.removeAll(ax);

        switch (event->key())
        {
            case Qt::Key_M:
                rescale_axes(axes_to_rescale);
                event->accept();
                replot();
                break;
            case Qt::Key_L:
                for (auto ax : axes)
                    ax->set_log(not ax->log());
                event->accept();
                replot();
                break;
            default:
                break;
        }
        if (!event->isAccepted())
            QWidget::keyPressEvent(event);
    }
};
