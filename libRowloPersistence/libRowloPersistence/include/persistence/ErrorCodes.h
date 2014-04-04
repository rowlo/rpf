/*
 * RPF - Rowlo Persistence Framework: Basic persistence layer for data model elements.
 *
 * File:    libRowloPersistence/include/persistence/ErrorCodes.h
 * Brief:   Error codes used by persistable model element.
 *
 * Copyright (C) 2014  Robert Wloch (robert@rowlo.de)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 * or browse: http://www.gnu.org/licenses/lgpl-2.1.html
*/
#ifndef ERRORCODES_H
#define ERRORCODES_H

namespace rowlo
{
namespace errorcodes
{
    const int MUST_NOT_SET_READONLY_FLAG = 1001;
    const int MUST_NOT_CHANGE_READONLY_PROPERTY = 1002;
    const int PROPERTY_MUST_NOT_BE_NULL = 1003;
    const int PROPERTY_STORAGENAME_IS_NO_QSTRING = 1004;
    const int PROPERTY_ID_IS_NO_INT = 1005;
} // namespace errorcodes
} // namespace rowlo

#endif // ERRORCODES_H
