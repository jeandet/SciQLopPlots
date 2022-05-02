/*------------------------------------------------------------------------------
-- This file is a part of the SciQLop Software
-- Copyright (C) 2020, Plasma Physics Laboratory - CNRS
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
#include "SciQLopPlots/Interfaces/IPlotWidget.hpp"
#include "SciQLopPlots/Interfaces/PlotWidget.hpp"
#include "SciQLopPlots/Qt/Events/Keyboard.hpp"
#include "SciQLopPlots/Qt/Events/Mouse.hpp"
#include "SciQLopPlots/Qt/Events/Wheel.hpp"

#include <QVBoxLayout>

#include <iostream>

namespace SciQLopPlots::interfaces
{


void IPlotWidget::delete_selected_object()
{
    if (m_selected_object)
    {
        delete m_selected_object;
        m_selected_object = nullptr;
    }
}

void IPlotWidget::setWidget(QWidget* widget)
{
    if (!this->layout())
        this->setLayout(new QVBoxLayout);
    this->layout()->addWidget(widget);
}

void IPlotWidget::wheelEvent(QWheelEvent* event)
{
    if (details::handleWheelEvent(event, this))
        event->accept();
}

void IPlotWidget::keyPressEvent(QKeyEvent* event)
{
    if (details::handleKeyboardEvent(event, this))
        event->accept();
}

void IPlotWidget::keyReleaseEvent(QKeyEvent* event)
{
    event->accept();
}

void IPlotWidget::mousePressEvent(QMouseEvent* event)
{
    m_has_moved_since_pouse_press = false;
    if (event->button() == Qt::LeftButton)
    {
        interfaces::GraphicObject* new_selected_object=graphicObjectAt(event->pos());
        this->m_lastMousePress = event->pos();
        if (m_selected_object
            and m_selected_object->contains(
                view::pixel_coordinates<2> { event->pos().x(), event->pos().y() }) and m_selected_object->layer()<=new_selected_object->layer())
        {
            if(m_prev_selected_object==m_selected_object)
            {
                new_selected_object = nextGraphicObjectAt(event->pos(), m_selected_object);
            }
            else
            {
                new_selected_object = m_selected_object;
            }
        }
        if (new_selected_object)
        {
            new_selected_object->set_selected(true);
        }
        if (m_selected_object and m_selected_object!=new_selected_object)
        {
            m_selected_object->set_selected(false);
        }
        m_prev_selected_object=m_selected_object;
        m_selected_object = new_selected_object;
    }

    event->accept();
}

void IPlotWidget::mouseMoveEvent(QMouseEvent* event)
{
    m_prev_selected_object = nullptr;
    m_has_moved_since_pouse_press = true;
    if (details::handleMouseMoveEvent(event, this, this->m_lastMousePress, m_selected_object))
        event->accept();
}

void IPlotWidget::mouseReleaseEvent(QMouseEvent* event)
{
    this->m_lastMousePress = std::nullopt;
    event->accept();
}


}


#include "IPlotWidget.moc"
