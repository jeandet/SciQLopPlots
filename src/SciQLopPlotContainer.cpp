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

#include "SciQLopPlots/MultiPlots/SciQLopPlotContainer.hpp"

SciQLopPlotContainer::SciQLopPlotContainer(QWidget* parent)
        : QSplitter(Qt::Vertical, parent), _plots(new SciQLopPlotCollection(this))
{
    this->setLayout(new QVBoxLayout(this));
    this->setContentsMargins(0, 0, 0, 0);
    this->layout()->setContentsMargins(0, 0, 0, 0);
    this->layout()->setSpacing(0);
    this->setChildrenCollapsible(false);
    setProperty("empty", true);
    connect(_plots, &SciQLopPlotCollection::plotListChanged, this,
        &SciQLopPlotContainer::plotListChanged);
}

void SciQLopPlotContainer::insertWidget(int index, QWidget* widget)
{
    QSplitter::insertWidget(index, widget);
    if (auto* plot = qobject_cast<SciQLopPlotInterface*>(widget); plot)
    {
        _plots->insertPlot(index, plot);
    }
}

void SciQLopPlotContainer::addWidget(QWidget* widget)
{
    insertWidget(QSplitter::count(), widget);
}

void SciQLopPlotContainer::insertPlot(int index, SciQLopPlotInterface* plot)
{
    insertWidget(index, plot);
}

void SciQLopPlotContainer::addPlot(SciQLopPlotInterface* plot)
{
    addWidget(plot);
}

void SciQLopPlotContainer::movePlot(int from, int to)
{
    auto* plot = _plots->plotAt(from);
    _plots->movePlot(from, to);
    QSplitter::insertWidget(to, plot);
}

void SciQLopPlotContainer::movePlot(SciQLopPlotInterface* plot, int to)
{
    movePlot(_plots->plots().indexOf(plot), to);
}

void SciQLopPlotContainer::removePlot(SciQLopPlotInterface* plot)
{
    removePlot(plot, true);
}

void SciQLopPlotContainer::removePlot(SciQLopPlotInterface* plot, bool destroy)
{
    if (_plots->contains(plot))
    {
        removeWidget(plot, destroy);
        if (destroy)
            plot->deleteLater();
        if (_plots->empty())
            setProperty("empty", true);
    }
}

void SciQLopPlotContainer::removeWidget(QWidget* widget, bool destroy)
{
    if (auto* plot = qobject_cast<SciQLopPlotInterface*>(widget); plot)
        removePlot(plot, destroy);
    else
    {
        widget->setParent(nullptr);
        if (destroy)
            widget->deleteLater();
    }
}

void SciQLopPlotContainer::clear()
{
    _plots->clear();
    while (QSplitter::count())
    {
        auto* widget = this->widget(0);
        widget->setParent(nullptr);
        widget->deleteLater();
    }
}

void SciQLopPlotContainer::organize_plots()
{
    auto _sizes = sizes();
    const auto total_height = std::accumulate(std::cbegin(_sizes), std::cend(_sizes), 0);
    const auto per_widget_height = total_height / std::size(_sizes);
    std::transform(std::cbegin(_sizes), std::cend(_sizes), std::begin(_sizes),
        [per_widget_height](int height) { return per_widget_height; });
    setSizes(_sizes);
}
