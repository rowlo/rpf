/*
 * RPF - Rowlo Persistence Framework: Basic persistence layer for data model elements.
 *
 * File:    libRowloPersistence/include/persistence/IDataBaseFacade.h
 * Brief:   Common interface for database facades.
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
#ifndef DATABASEFACADE_H
#define DATABASEFACADE_H

#include <QList>
#include <QSharedPointer>
#include <QtSql/QSqlDatabase>

#include "libRowloPersistence_global.h"
#include "persistence/Error.h"
#include "persistence/PersistableModelElement.h"
#include "persistence/IModelElementFactory.h"

namespace rowlo
{
namespace persistence
{

class LIBROWLOPERSISTENCESHARED_EXPORT IDataBaseFacade
{
public:
    virtual QSharedPointer<Error> connectToDb() = 0;
    virtual QSharedPointer<Error> disconnectFromDb() = 0;
    virtual QSharedPointer<Error> resetDb() = 0;

    virtual bool isSqlDataBase() = 0;
    virtual QSharedPointer<QSqlDatabase> connectedSqlDataBase() = 0;

    virtual QSharedPointer<Error> store(
            const QList<QSharedPointer<PersistableModelElement> > &elements) = 0;

    virtual QSharedPointer<Error> find(
            const QSharedPointer<PersistableModelElement> &dummy,
            QList<QSharedPointer<PersistableModelElement> > &results) = 0;

    virtual QSharedPointer<Error> update(
            const QList<QSharedPointer<PersistableModelElement> > &elements) = 0;

    virtual QSharedPointer<Error> remove(
            const QList<QSharedPointer<PersistableModelElement> > &elements) = 0;

    virtual void registerModelElementFactory(
            const QSharedPointer<IModelElementFactory> &factory) = 0;
};

} // namespace persistence
} // namespace rowlo

#endif // DATABASEFACADE_H
