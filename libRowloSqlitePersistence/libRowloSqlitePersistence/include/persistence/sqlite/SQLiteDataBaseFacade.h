/*
 * RPF - Rowlo Persistence Framework: Sqlite persistence layer.
 *
 * File:    libRowloSqlitePersistence/include/persistence/sqlite/SQLiteDataBaseFacade.h
 * Brief:   Inteface for SQLite data base access. Realizes IDataBaseFacade.
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
#ifndef SQLITEDATABASEFACADE_H
#define SQLITEDATABASEFACADE_H

#include "persistence/IDataBaseFacade.h"
#include "persistence/IModelElementFactory.h"
#include "persistence/PersistableModelElement.h"
#include "libRowloSqlitePersistence_global.h"

#include <QSharedPointer>
#include <QSqlQuery>
#include <QStringList>
#include <QDir>
#include <QList>

namespace rowlo
{
namespace persistence
{
namespace sqlite
{

class LIBROWLOSQLITEPERSISTENCESHARED_EXPORT SQLiteDataBaseFacade :
        virtual public IDataBaseFacade
{
public:
    SQLiteDataBaseFacade(
            const QString &connectionName = "SQLiteDataBaseConnection",
            const QString &pathToDataBase = QDir::currentPath() + QDir::separator(),
            const QString &dataBaseFile = "data.db.sql3");
    virtual ~SQLiteDataBaseFacade();

    QSharedPointer<rowlo::persistence::Error> connectToDb();
    QSharedPointer<rowlo::persistence::Error> disconnectFromDb();
    QSharedPointer<rowlo::persistence::Error> resetDb();

    bool isSqlDataBase();
    QSharedPointer<QSqlDatabase> connectedSqlDataBase();

    QSharedPointer<rowlo::persistence::Error> store(
            const QSharedPointer<rowlo::persistence::PersistableModelElement> &element);
    QSharedPointer<rowlo::persistence::Error> store(
            const QList<QSharedPointer<rowlo::persistence::PersistableModelElement> > &elements);

    QSharedPointer<rowlo::persistence::Error> find(
            const QSharedPointer<rowlo::persistence::PersistableModelElement> &element,
            QList<QSharedPointer<rowlo::persistence::PersistableModelElement> > &results);

    QSharedPointer<rowlo::persistence::Error> update(
            const QSharedPointer<PersistableModelElement> &element);
    QSharedPointer<rowlo::persistence::Error> update(
            const QList<QSharedPointer<rowlo::persistence::PersistableModelElement> > &elements);

    QSharedPointer<rowlo::persistence::Error> remove(
            const QSharedPointer<rowlo::persistence::PersistableModelElement> &element);
    QSharedPointer<rowlo::persistence::Error> remove(
            const QList<QSharedPointer<rowlo::persistence::PersistableModelElement> > &elements);

    void registerModelElementFactory(
            const QSharedPointer<IModelElementFactory> &factory);

protected:
    QString storageName(
            const QSharedPointer<rowlo::persistence::PersistableModelElement> &element);
    QString fieldsAsList(
            const QSharedPointer<rowlo::persistence::PersistableModelElement> &element);
    QSharedPointer<rowlo::persistence::Error> storeWithinTransaction(
            const QSharedPointer<PersistableModelElement> &element);
    QSharedPointer<rowlo::persistence::Error> updateWithinTransaction(
            const QSharedPointer<PersistableModelElement> &element);
    void makeStoreColumnLists(const QSharedPointer<PersistableModelElement> &element,
            QStringList &columnList, QStringList &columnBindList,
            const bool &skipId = false);
    void bindStoreValues(
            const QSharedPointer<PersistableModelElement> &element,
            const QStringList &columnList, const QStringList &columnBindList,
            QSqlQuery &query);
    QSharedPointer<Error> createModelElement(
            const QSharedPointer<PersistableModelElement> &element,
            const QList<QPair<QString, QVariant> > &elementValues,
            QSharedPointer<rowlo::persistence::PersistableModelElement> &result);
    bool elementExistsInDatabase(const QSharedPointer<PersistableModelElement> &element);

private:
    QSharedPointer<QSqlDatabase> m_sqlDataBase;
    QString m_connectionName;
    QString m_dataBaseFilePath;
    QList<QSharedPointer<IModelElementFactory> > m_modelElementFactories;
};

} // namespace sqlite
} // namespace persistence
} // namespace rowlo

#endif // SQLITEDATABASEFACADE_H
