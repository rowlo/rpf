/*
 * RPF - Rowlo Persistence Framework: Basic persistence layer for data model elements.
 *
 * File:    libRowloPersistence/src/persistence/Error.cpp
 * Brief:   Basic Error class used throughout the RPF libraries.
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
#include "persistence/Error.h"

namespace rowlo
{
namespace persistence
{

Error::Error(const QString &errorMessage, qint32 errorCode) :
    m_errorMessage(errorMessage),
    m_errorCode(errorCode)
{
}

Error::~Error()
{
}

QString Error::errorMessage() const
{
    return m_errorMessage;
}

qint32 Error::errorCode() const
{
    return m_errorCode;
}

bool Error::isError() const
{
    return not (m_errorCode == 0);
}

} // namespace persistence
} // namespace rowlo
