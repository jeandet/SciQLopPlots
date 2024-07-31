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

#include <QGridLayout>
#include <QScrollArea>
#include <QWidget>

#include "SciQLopPlotCollection.hpp"

class SciQLopPlotContainer;
class SciQLopPlot;

class SciQLopMultiPlotPanel : public QScrollArea, public SciQLopPlotCollectionInterface
{
    Q_OBJECT
    SciQLopPlotContainer* _container = nullptr;

public:
    SciQLopMultiPlotPanel(QWidget* parent = nullptr);

    void addPlot(SciQLopPlot* plot) final;
    void removePlot(SciQLopPlot* plot) final;
    SciQLopPlot* plotAt(int index) final;
    const QList<SciQLopPlot*>& plots() const final;

    virtual void insertPlot(int index, SciQLopPlot* plot) final;
    virtual void movePlot(int from, int to) final;
    virtual void movePlot(SciQLopPlot* plot, int to) final;
    void clear() final;


    bool contains(SciQLopPlot* plot) const final;

    bool empty() const final;
    std::size_t size() const final;

    void addWidget(QWidget* widget);
    void removeWidget(QWidget* widget);

    SciQLopPlot* createPlot(int index = -1);

#ifndef BINDINGS_H
    Q_SIGNAL void plotListChanged(const QList<SciQLopPlot*>& plots);
#endif // BINDINGS_H
};
