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
#include "SciQLopPlots/Plotables/SciQLopGraphInterface.hpp"

SciQLopGraphInterface::SciQLopGraphInterface(QObject* parent) : QObject(parent)
{
    connect(this, &QObject::objectNameChanged, this, &SciQLopGraphInterface::name_changed);
}

void SciQLopGraphInterface::set_range(double lower, double upper)
{
    if (m_range.first != lower || m_range.second != upper)
    {
        m_range = { lower, upper };
        Q_EMIT range_changed(lower, upper);
    }
}
