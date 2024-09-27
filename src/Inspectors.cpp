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
#include "SciQLopPlots/Inspector/Inspectors.hpp"
#include "SciQLopPlots/Debug.hpp"
#include "SciQLopPlots/Inspector/InspectorBase.hpp"

InspectorBase* Inspectors::inspector(const QObject* obj)
{
    auto metaObject = obj->metaObject();
    InspectorBase* _inspector = nullptr;
    do
    {
        _inspector = inspector(metaObject->className());
        metaObject = metaObject->superClass();
    } while (_inspector == nullptr && metaObject != nullptr);
    if (_inspector)
    {
        DEBUG_MESSAGE("Inspector found:" << _inspector->metaObject()->className());
    }
    return _inspector;
}