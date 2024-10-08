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
#include <QObject>
#include <QPointer>
class QCPAxis;


class SciQLopPlotAxisInterface : public QObject
{
    Q_OBJECT
public:
    SciQLopPlotAxisInterface(QObject* parent = nullptr) : QObject(parent) { }
    virtual ~SciQLopPlotAxisInterface() = default;

    virtual void set_range(double lower, double upper) noexcept { }
    virtual void set_visible(bool visible) noexcept { }
    virtual void set_log(bool log) noexcept { }
    virtual void set_label(const QString& label) noexcept { }

    inline virtual std::pair<double, double> range() const noexcept
    {
        return std::make_pair(std::nan(""), std::nan(""));
    }
    virtual bool visible() const noexcept { return false; }
    virtual bool log() const noexcept { return false; }
    virtual QString label() const noexcept { return QString(); }
    virtual Qt::Orientation orientation() const noexcept { return Qt::Horizontal; }
    virtual Qt::Axis axis() const noexcept { return Qt::XAxis; }
    virtual Qt::AnchorPoint anchor() const noexcept { return Qt::AnchorBottom; }

    virtual bool selected() const noexcept { return false; }

    virtual void rescale() noexcept { }

#ifndef BINDINGS_H
    Q_SIGNAL void range_changed(double lower, double upper);
    Q_SIGNAL void visible_changed(bool visible);
    Q_SIGNAL void log_changed(bool log);
    Q_SIGNAL void label_changed(const QString& label);
#endif
};

class SciQLopPlotDummyAxis : public SciQLopPlotAxisInterface
{
    Q_OBJECT
    std::pair<double, double> m_range;

public:
    explicit SciQLopPlotDummyAxis(QObject* parent = nullptr) : SciQLopPlotAxisInterface(parent) { }
    virtual ~SciQLopPlotDummyAxis() = default;

    void set_range(double lower, double upper) noexcept override;
    inline std::pair<double, double> range() const noexcept override { return m_range; }
};

class SciQLopPlotAxis : public SciQLopPlotAxisInterface
{
    Q_OBJECT
    QPointer<QCPAxis> m_axis;

public:
    explicit SciQLopPlotAxis(QCPAxis* axis, QObject* parent = nullptr);
    virtual ~SciQLopPlotAxis() = default;

    void set_range(double lower, double upper) noexcept override;
    void set_visible(bool visible) noexcept override;
    void set_log(bool log) noexcept override;
    void set_label(const QString& label) noexcept override;
    std::pair<double, double> range() const noexcept override;
    bool visible() const noexcept override;
    bool log() const noexcept override;
    QString label() const noexcept override;
    Qt::Orientation orientation() const noexcept override;
    Qt::Axis axis() const noexcept override;
    Qt::AnchorPoint anchor() const noexcept override;
    bool selected() const noexcept override;

    void rescale() noexcept override;

    QCPAxis* qcp_axis() const noexcept;
};
