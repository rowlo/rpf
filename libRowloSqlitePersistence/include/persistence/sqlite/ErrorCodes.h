/*
 * RPF - Rowlo Persistence Framework: Sqlite persistence layer.
 *
 * File:    libRowloSqlitePersistence/include/persistence/sqlite/ErrorCodes.h
 * Brief:   Error codes used by Sqlite data base facade.
 *
 * Copyright (C) 2013  Robert Wloch (robert@rowlo.de)
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
#ifndef SQLITE_ERRORCODES_H
#define SQLITE_ERRORCODES_H

namespace rowlo
{
namespace errorcodes
{
    const int DB_ADDED_ALREADY = 2001;
    const int DB_CANNOT_BE_OPENED = 2002;
    const int DB_FILE_CANNOT_BE_REMOVED = 2003;
    const int DB_IS_NOT_CONNECTED = 2004;
    const int DB_CANNOT_STORE_NULL_ELEMENT = 2005;
    const int DB_ELEMENT_NOT_PERSISTABLE = 2006;
    const int DB_FAILED_TO_PREPARE_QUERY = 2007;
    const int DB_FAILED_TO_INSERT_ELEMENT = 2008;
    const int DB_CANNOT_FIND_NULL_ELEMENT = 2009;
    const int DB_FAILED_TO_EXECUTE_QUERY = 2010;
    const int DB_CANNOT_CREATE_MODEL_ELEMENT_NULL_TEMPLATE = 2011;
    const int DB_TEMPLATE_WITHOUT_PERSISTENCE_INFO = 2012;
    const int DB_TEMPLATE_HAS_UNKNOWN_TYPE = 2013;
    const int DB_CANNOT_REMOVE_NULL_ELEMENT = 2014;
    const int DB_FAILED_TO_UPDATE_ELEMENT = 2015;
    const int DB_CANNOT_UPDATE_NON_EXISTING_ELEMENT = 2016;
} // namespace errorcodes
} // namespace rowlo

#endif // SQLITE_ERRORCODES_H
