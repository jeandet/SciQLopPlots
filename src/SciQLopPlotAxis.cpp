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
#include "SciQLopPlots/SciQLopPlotAxis.hpp"

#include "qcustomplot.h"

SciQLopPlotAxis::SciQLopPlotAxis(QCPAxis* axis, QObject* parent)
        : SciQLopPlotAxisInterface(parent), m_axis(axis)
{
    connect(axis, QOverload<const QCPRange&>::of(&QCPAxis::rangeChanged), this,
        [this](const QCPRange& range) { Q_EMIT range_changed(range.lower, range.upper); });
}

void SciQLopPlotAxis::set_range(double lower, double upper) noexcept
{
    if (!m_axis.isNull() && (m_axis->range().lower != lower || m_axis->range().upper != upper))
    {
        m_axis->setRange(lower, upper);
        m_axis->parentPlot()->replot(QCustomPlot::rpQueuedReplot);
    }
}

void SciQLopPlotAxis::set_visible(bool visible) noexcept
{
    if (!m_axis.isNull() && m_axis->visible() != visible)
    {
        m_axis->setVisible(visible);
        m_axis->parentPlot()->replot(QCustomPlot::rpQueuedReplot);
        Q_EMIT visible_changed(visible);
    }
}

void SciQLopPlotAxis::set_log(bool log) noexcept
{
    if (!m_axis.isNull() && m_axis->scaleType() == QCPAxis::stLogarithmic != log)
    {
        if (log)
        {
            m_axis->setScaleType(QCPAxis::stLogarithmic);
            m_axis->setTicker(QSharedPointer<QCPAxisTickerLog>(new QCPAxisTickerLog));
        }
        else
        {
            m_axis->setScaleType(QCPAxis::stLinear);
            m_axis->setTicker(QSharedPointer<QCPAxisTicker>(new QCPAxisTicker));
        }
        m_axis->parentPlot()->replot(QCustomPlot::rpQueuedReplot);
        Q_EMIT log_changed(log);
    }
}

void SciQLopPlotAxis::set_label(const QString& label) noexcept
{
    if (!m_axis.isNull() && m_axis->label() != label)
    {
        m_axis->setLabel(label);
        m_axis->parentPlot()->replot(QCustomPlot::rpQueuedReplot);
        Q_EMIT label_changed(label);
    }
}

std::pair<double, double> SciQLopPlotAxis::range() const noexcept
{
    if (m_axis.isNull())
        return std::make_pair(std::nan(""), std::nan(""));
    return std::make_pair(m_axis->range().lower, m_axis->range().upper);
}

bool SciQLopPlotAxis::visible() const noexcept
{
    if (m_axis.isNull())
        return false;
    return m_axis->visible();
}

bool SciQLopPlotAxis::log() const noexcept
{
    if (m_axis.isNull())
        return false;
    return m_axis->scaleType() == QCPAxis::stLogarithmic;
}

QString SciQLopPlotAxis::label() const noexcept
{
    if (m_axis.isNull())
        return QString();
    return m_axis->label();
}

Qt::Orientation SciQLopPlotAxis::orientation() const noexcept
{
    if (m_axis.isNull())
        return Qt::Horizontal;
    return m_axis->orientation();
}

Qt::Axis SciQLopPlotAxis::axis() const noexcept
{
    if (m_axis.isNull())
        return Qt::XAxis;
    if (orientation() == Qt::Horizontal)
        return Qt::XAxis;
    return Qt::YAxis;
}

Qt::AnchorPoint SciQLopPlotAxis::anchor() const noexcept
{
    if (m_axis.isNull())
        return Qt::AnchorBottom;
    switch (m_axis->axisType())
    {
        case QCPAxis::AxisType::atLeft:
            return Qt::AnchorLeft;
            break;
        case QCPAxis::AxisType::atRight:
            return Qt::AnchorRight;
            break;
        case QCPAxis::AxisType::atTop:
            return Qt::AnchorTop;
            break;
        case QCPAxis::AxisType::atBottom:
            return Qt::AnchorBottom;
            break;
        default:
            break;
    }
    return Qt::AnchorBottom;
}

bool SciQLopPlotAxis::selected() const noexcept
{
    if (m_axis.isNull())
        return false;
    return m_axis->selectedParts().testFlag(QCPAxis::spAxis);
}

void SciQLopPlotAxis::rescale() noexcept
{
    if (!m_axis.isNull())
    {
        m_axis->rescale(true);
        m_axis->parentPlot()->replot(QCustomPlot::rpQueuedReplot);
    }
}

QCPAxis* SciQLopPlotAxis::qcp_axis() const noexcept
{
    return m_axis.data();
}

void SciQLopPlotDummyAxis::set_range(double lower, double upper) noexcept
{
    if (m_range.first != lower || m_range.second != upper)
    {
        m_range = { lower, upper };
        Q_EMIT this->range_changed(lower, upper);
    }
}
