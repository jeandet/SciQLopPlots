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
#include "SciQLopPlots/SciQLopPlot.hpp"
#include "SciQLopPlots/SciQLopTimeSeriesPlot.hpp"

#include "SciQLopPlots/MultiPlots/SciQLopMultiPlotPanel.hpp"
#include "SciQLopPlots/MultiPlots/SciQLopPlotCollection.hpp"
#include "SciQLopPlots/MultiPlots/SciQLopPlotContainer.hpp"
#include "SciQLopPlots/MultiPlots/TimeAxisSynchronizer.hpp"
#include "SciQLopPlots/MultiPlots/VPlotsAlign.hpp"
#include "SciQLopPlots/MultiPlots/XAxisSynchronizer.hpp"

#include <QKeyEvent>

SciQLopMultiPlotPanel::SciQLopMultiPlotPanel(
    QWidget* parent, bool synchronize_x, bool synchronize_time)
        : QScrollArea { nullptr }
{
    _container = new SciQLopPlotContainer(this);
    connect(_container, &SciQLopPlotContainer::plotListChanged, this,
        &SciQLopMultiPlotPanel::plotListChanged);
    setWidget(_container);
    this->setWidgetResizable(true);

    ::registerBehavior<VPlotsAlign>(_container);
    if (synchronize_x)
        ::registerBehavior<XAxisSynchronizer>(_container);
    if (synchronize_time)
        ::registerBehavior<TimeAxisSynchronizer>(_container);
}

void SciQLopMultiPlotPanel::addPlot(SciQLopPlotInterface* plot)
{
    _container->addPlot(plot);
}

void SciQLopMultiPlotPanel::removePlot(SciQLopPlotInterface* plot)
{
    _container->removePlot(plot);
}

SciQLopPlotInterface* SciQLopMultiPlotPanel::plotAt(int index) const
{
    return _container->plotAt(index);
}

const QList<SciQLopPlotInterface*>& SciQLopMultiPlotPanel::plots() const
{
    return _container->plots();
}

void SciQLopMultiPlotPanel::insertPlot(int index, SciQLopPlotInterface* plot)
{
    _container->insertPlot(index, plot);
}

void SciQLopMultiPlotPanel::movePlot(int from, int to)
{
    _container->movePlot(from, to);
}

void SciQLopMultiPlotPanel::movePlot(SciQLopPlotInterface* plot, int to)
{
    _container->movePlot(plot, to);
}

void SciQLopMultiPlotPanel::clear()
{
    _container->clear();
}

bool SciQLopMultiPlotPanel::contains(SciQLopPlotInterface* plot) const
{
    return _container->contains(plot);
}

bool SciQLopMultiPlotPanel::empty() const
{
    return _container->empty();
}

std::size_t SciQLopMultiPlotPanel::size() const
{
    return _container->size();
}

void SciQLopMultiPlotPanel::addWidget(QWidget* widget)
{
    _container->addWidget(widget);
}

void SciQLopMultiPlotPanel::removeWidget(QWidget* widget)
{
    _container->removeWidget(widget, true);
}

SciQLopPlotInterface* SciQLopMultiPlotPanel::create_plot(int index, PlotType plot_type)
{
    SciQLopPlotInterface* plot = nullptr;
    switch (plot_type)
    {
        case ::PlotType::BasicXY:
            plot = new SciQLopPlot();
            break;
        case ::PlotType::TimeSeries:
            plot = new SciQLopTimeSeriesPlot();
            break;
        default:
            break;
    }
    if (index == -1)
        addPlot(plot);
    else
        insertPlot(index, plot);
    return plot;
}

void SciQLopMultiPlotPanel::set_x_axis_range(double lower, double upper)
{
    _container->set_x_axis_range(lower, upper);
}

void SciQLopMultiPlotPanel::set_time_axis_range(double min, double max)
{
    _container->set_time_axis_range(min, max);
}

void SciQLopMultiPlotPanel::registerBehavior(SciQLopPlotCollectionBehavior* behavior)
{
    _container->registerBehavior(behavior);
}

void SciQLopMultiPlotPanel::removeBehavior(const QString& type_name)
{
    _container->removeBehavior(type_name);
}

QPair<SciQLopPlotInterface*, SciQLopGraphInterface*> SciQLopMultiPlotPanel::plot_impl(
    const PyBuffer& x, const PyBuffer& y, QStringList labels, QList<QColor> colors,
    PlotType plot_type, GraphType graph_type, int index)
{
    switch (plot_type)
    {
        case ::PlotType::BasicXY:
            return _plot<SciQLopPlot>(index, graph_type, x, y, labels, colors);
            break;
        case ::PlotType::TimeSeries:
            return _plot<SciQLopTimeSeriesPlot>(index, graph_type, x, y, labels, colors);
            break;
        default:
            break;
    }
    return { nullptr, nullptr };
}

QPair<SciQLopPlotInterface*, SciQLopGraphInterface*> SciQLopMultiPlotPanel::plot_impl(
    const PyBuffer& x, const PyBuffer& y, const PyBuffer& z, QString name, bool y_log_scale,
    bool z_log_scale, PlotType plot_type, int index)
{
    switch (plot_type)
    {
        case ::PlotType::BasicXY:
            return _plot<SciQLopPlot>(
                index, GraphType::ColorMap, x, y, z, name, y_log_scale, z_log_scale);
            break;
        case ::PlotType::TimeSeries:
            return _plot<SciQLopTimeSeriesPlot>(
                index, GraphType::ColorMap, x, y, z, name, y_log_scale, z_log_scale);
            break;
        default:
            break;
    }
    return { nullptr, nullptr };
}

QPair<SciQLopPlotInterface*, SciQLopGraphInterface*> SciQLopMultiPlotPanel::plot_impl(
    GetDataPyCallable callable, QStringList labels, QList<QColor> colors, GraphType graph_type,
    PlotType plot_type, QObject* sync_with, int index)
{
    switch (plot_type)
    {
        case ::PlotType::BasicXY:
            return _plot<SciQLopPlot>(index, graph_type, callable, labels, colors, sync_with);
            break;
        case ::PlotType::TimeSeries:
            return _plot<SciQLopTimeSeriesPlot>(
                index, graph_type, callable, labels, colors, sync_with);
            break;
        default:
            break;
    }
    return { nullptr, nullptr };
}

QPair<SciQLopPlotInterface*, SciQLopGraphInterface*> SciQLopMultiPlotPanel::plot_impl(
    GetDataPyCallable callable, QString name, bool y_log_scale, bool z_log_scale,
    PlotType plot_type, QObject* sync_with, int index)
{
    switch (plot_type)
    {
        case ::PlotType::BasicXY:
            return _plot<SciQLopPlot>(
                index, GraphType::ColorMap, callable, name, y_log_scale, z_log_scale, sync_with);
            break;
        case ::PlotType::TimeSeries:
            return _plot<SciQLopTimeSeriesPlot>(
                index, GraphType::ColorMap, callable, name, y_log_scale, z_log_scale, sync_with);
            break;
        default:
            break;
    }
    return { nullptr, nullptr };
}


void SciQLopMultiPlotPanel::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
        case Qt::Key_O:
            _container->organize_plots();
            event->accept();
            break;
        default:
            break;
    }
    if (!event->isAccepted())
        QScrollArea::keyPressEvent(event);
}
