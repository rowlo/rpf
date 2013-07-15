/*
 * RPF - Rowlo Persistence Framework: Basic persistence layer for data model elements.
 *
 * File:    libRowloPersistence/include/persistence/Error.h
 * Brief:   Most generic error codes and Error interface.
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
#ifndef ERROR_H
#define ERROR_H

#include <QList>
#include <QObject>
#include <QSharedPointer>
#include <QString>

#include "libRowloPersistence_global.h"

namespace rowlo
{
namespace errorcodes
{
    const int MUST_NOT_SET_READONLY_FLAG = 1001;
    const int MUST_NOT_CHANGE_READONLY_PROPERTY = 1002;
    const int PROPERTY_MUST_NOT_BE_NULL = 1003;
    const int PROPERTY_TABLENAME_IS_NO_QSTRING = 1004;
    const int PROPERTY_ID_IS_NO_INT = 1005;
} // namespace errorcodes
} // namespace rowlo


namespace rowlo
{
namespace persistence
{
class LIBROWLOPERSISTENCESHARED_EXPORT Error
{

public:
    Error(const QString &errorMessage = QObject::tr("No error."), qint32 errorCode = 0);
    ~Error();
    QString errorMessage() const;
    qint32 errorCode() const;
    bool isError() const;

private:
    QString m_errorMessage;
    qint32 m_errorCode;
};
} // namespace persistence
} // namespace rowlo

Q_DECLARE_METATYPE(rowlo::persistence::Error)
Q_DECLARE_METATYPE(QSharedPointer<rowlo::persistence::Error>)
Q_DECLARE_METATYPE(QList<rowlo::persistence::Error>)
Q_DECLARE_METATYPE(QList<QSharedPointer<rowlo::persistence::Error> >)

#endif // ERROR_H
