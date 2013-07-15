/*
 * RPF - Rowlo Persistence Framework: Sqlite persistence layer.
 *
 * File:    libRowloSqlitePersistence/src/persistence/sqlite/SQLiteDataBaseFacade.cpp
 * Brief:   Database access to sqlite data bases.
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
#include "persistence/sqlite/SQLiteDataBaseFacade.h"
#include "persistence/sqlite/ErrorCodes.h"

#include <QDir>
#include <QObject>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlResult>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>

#ifndef ROWLO_TRACE_DB_RESULTS
//#define ROWLO_TRACE_DB_RESULTS true
#include <QDebug>
#endif

namespace rowlo
{
namespace persistence
{
namespace sqlite
{

SQLiteDataBaseFacade::SQLiteDataBaseFacade() :
    m_sqlDataBase(QSharedPointer<QSqlDatabase>()),
    m_connectionName("IconBrowserConnection"),
    m_dataBaseFilePath(QDir::currentPath() + QDir::separator() + "data.db.sql3")
{
}

QSharedPointer<Error> SQLiteDataBaseFacade::connectToDb()
{
    QSharedPointer<Error> result(new Error());
    if (m_sqlDataBase.isNull())
    {
        if (QSqlDatabase::contains(m_connectionName))
        {
            result = QSharedPointer<Error>(
                        new Error(QObject::tr("SQL database was added already."),
                                  rowlo::errorcodes::DB_ADDED_ALREADY));
            return result;
        }
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", m_connectionName);
        m_sqlDataBase = QSharedPointer<QSqlDatabase>(new QSqlDatabase(db));
        m_sqlDataBase->setDatabaseName(m_dataBaseFilePath);
    }

    if (not m_sqlDataBase->open())
    {
        result = QSharedPointer<Error>(
                    new Error(QObject::tr("An error occurred opening the database: %1")
                              .arg(m_sqlDataBase->lastError().text()),
                              rowlo::errorcodes::DB_CANNOT_BE_OPENED));
        return result;
    }

    return result;
}

QSharedPointer<Error> SQLiteDataBaseFacade::disconnectFromDb()
{
    QSharedPointer<Error> result(new Error());
    if (m_sqlDataBase && m_sqlDataBase->isOpen())
    {
        m_sqlDataBase->close();
        m_sqlDataBase.clear();
        QSqlDatabase::removeDatabase(m_connectionName);
    }
    return result;
}

QSharedPointer<Error> SQLiteDataBaseFacade::resetDb()
{
    QSharedPointer<Error> result(new Error());
    disconnectFromDb();
    QFile file(m_dataBaseFilePath);
    if (file.exists() && not file.remove())
    {
        result = QSharedPointer<Error>(
                    new Error(QObject::tr("Failed to remove database file: %1")
                              .arg(m_dataBaseFilePath),
                              rowlo::errorcodes::DB_FILE_CANNOT_BE_REMOVED));
        return result;
    }
    return result;
}

bool SQLiteDataBaseFacade::isSqlDataBase()
{
    return true;
}

QSharedPointer<QSqlDatabase> SQLiteDataBaseFacade::connectedSqlDataBase()
{
    return m_sqlDataBase;
}

QSharedPointer<Error> SQLiteDataBaseFacade::store(
        const QSharedPointer<PersistableModelElement> &element)
{
    QList<QSharedPointer<PersistableModelElement> > elements;
    elements << element;
    return store(elements);
}

QSharedPointer<Error> SQLiteDataBaseFacade::store(
        const QList<QSharedPointer<PersistableModelElement> > &elements)
{
    QSharedPointer<Error> error(new Error());
    if (m_sqlDataBase.isNull() || not m_sqlDataBase->isOpen())
    {
        error = QSharedPointer<Error>(new Error(QObject::tr("Database is not connected!"),
                                                 rowlo::errorcodes::DB_IS_NOT_CONNECTED));
        return error;
    }

    m_sqlDataBase->transaction();
    foreach (QSharedPointer<PersistableModelElement> element, elements)
    {
        if (not element)
        {
            error = QSharedPointer<Error>(new Error(QObject::tr("Cannot store null element!"),
                                                     rowlo::errorcodes::DB_CANNOT_STORE_NULL_ELEMENT));
        }
        else if (not element->isPersistable())
        {
            error = QSharedPointer<Error>(new Error(
                     QObject::tr("Element is not persistable (has no _tableName_ and/or _id_ property)!"),
                                               rowlo::errorcodes::DB_ELEMENT_NOT_PERSISTABLE));
        }
        else
        {
            error = storeWithinTransaction(element);
        }

        if (not error.isNull() && error->errorCode() > 0)
        {
            m_sqlDataBase->rollback();
            return error;
        }
    }
    m_sqlDataBase->commit();

    return error;
}

QSharedPointer<Error> SQLiteDataBaseFacade::storeWithinTransaction(
        const QSharedPointer<PersistableModelElement> &element)
{
    QSharedPointer<Error> error(new Error());

    QString tableName = element->getProperty("_tableName_").toString();
    QStringList columnList;
    QStringList columnBindList;
    makeStoreColumnLists(element, columnList, columnBindList);
    QString columnsAsList = columnList.join(", ");
    QString columnBindsAsList = columnBindList.join(", ");

    QSqlQuery query(*(m_sqlDataBase));
    QString sQuery = QString("INSERT INTO %1 (%2) VALUES (%3)")
            .arg(tableName, columnsAsList, columnBindsAsList);
    if (not query.prepare(sQuery))
    {
        QString errorText = m_sqlDataBase->lastError().text();
        error = QSharedPointer<Error>(
                    new Error(QObject::tr("Failed to prepare query: |%1| DB error: %2")
                              .arg(sQuery).arg(errorText),
                              rowlo::errorcodes::DB_FAILED_TO_PREPARE_QUERY));
    }
    else
    {
        bindStoreValues(element, columnList, columnBindList, query);
#ifdef ROWLO_TRACE_DB_RESULTS
        qDebug() << QString("insert query: |%1|").arg(sQuery).toLocal8Bit().data();
#endif
        if (not query.exec())
        {
            QString errorText = m_sqlDataBase->lastError().text();
            error = QSharedPointer<Error>(
                        new Error(QObject::tr("Failed to insert element: |%1| DB error: %2")
                                  .arg(sQuery).arg(errorText),
                                  rowlo::errorcodes::DB_FAILED_TO_INSERT_ELEMENT));
        }
    }
    return error;
}

QSharedPointer<Error> SQLiteDataBaseFacade::update(
        const QSharedPointer<PersistableModelElement> &element)
{
    QList<QSharedPointer<PersistableModelElement> > elements;
    elements << element;
    return update(elements);
}

QSharedPointer<Error> SQLiteDataBaseFacade::update(
        const QList<QSharedPointer<PersistableModelElement> > &elements)
{
    QSharedPointer<Error> error(new Error());
    if (m_sqlDataBase.isNull() || not m_sqlDataBase->isOpen())
    {
        error = QSharedPointer<Error>(new Error(QObject::tr("Database is not connected!"),
                                                 rowlo::errorcodes::DB_IS_NOT_CONNECTED));
        return error;
    }

    m_sqlDataBase->transaction();
    foreach (QSharedPointer<PersistableModelElement> element, elements)
    {
        if (not element)
        {
            error = QSharedPointer<Error>(new Error(QObject::tr("Cannot store null element!"),
                                                     rowlo::errorcodes::DB_CANNOT_STORE_NULL_ELEMENT));
        }
        else if (not element->isPersistable())
        {
            error = QSharedPointer<Error>(new Error(
                     QObject::tr("Element is not persistable (has no _tableName_ and/or _id_ property)!"),
                                               rowlo::errorcodes::DB_ELEMENT_NOT_PERSISTABLE));
        }
        else if (not elementExistsInDatabase(element))
        {
            error = QSharedPointer<Error>(new Error(QObject::tr("Cannot update non existing element!"),
                                                     rowlo::errorcodes::DB_CANNOT_UPDATE_NON_EXISTING_ELEMENT));
        }
        else
        {
            error = updateWithinTransaction(element);
        }

        if (not error.isNull() && error->errorCode() > 0)
        {
            m_sqlDataBase->rollback();
            return error;
        }
    }
    m_sqlDataBase->commit();

    return error;
}

QSharedPointer<Error> SQLiteDataBaseFacade::updateWithinTransaction(
        const QSharedPointer<PersistableModelElement> &element)
{
    QSharedPointer<Error> error(new Error());

    QString tableName = element->getProperty("_tableName_").toString();
    QStringList columnList;
    QStringList columnBindList;
    makeStoreColumnLists(element, columnList, columnBindList, true);
    QStringList updateFields;
    for (int p = 0, max = columnList.size(); p < max; p++)
    {
        updateFields.append(QString("%1 = %2").arg(columnList.at(p), columnBindList.at(p)));
    }

    int elementId = element->getProperty("_id_").toInt();
    QSqlQuery query(*(m_sqlDataBase));
    QString sQuery = QString("UPDATE %1 SET %2 WHERE _id_=%3")
            .arg(tableName, updateFields.join(" ,"))
            .arg(elementId);
    if (not query.prepare(sQuery))
    {
        QString errorText = m_sqlDataBase->lastError().text();
        error = QSharedPointer<Error>(
                    new Error(QObject::tr("Failed to prepare query: |%1| DB error: %2")
                              .arg(sQuery).arg(errorText),
                              rowlo::errorcodes::DB_FAILED_TO_PREPARE_QUERY));
    }
    else
    {
        bindStoreValues(element, columnList, columnBindList, query);
#ifdef ROWLO_TRACE_DB_RESULTS
        qDebug() << QString("update query: |%1|").arg(sQuery).toLocal8Bit().data();
#endif
        if (not query.exec())
        {
            QString errorText = m_sqlDataBase->lastError().text();
            error = QSharedPointer<Error>(
                        new Error(QObject::tr("Failed to update element: |%1| DB error: %2")
                                  .arg(sQuery).arg(errorText),
                                  rowlo::errorcodes::DB_FAILED_TO_UPDATE_ELEMENT));
        }
    }
    return error;
}

bool SQLiteDataBaseFacade::elementExistsInDatabase(
        const QSharedPointer<PersistableModelElement> &element)
{
    bool result = false;
    // By creating PersistableModelElement for the find query it's possible to skip all real
    // properties for the WHERE clause and it allows createModelElement() to create a base
    // element instead of the real type represented by a table. Technically, real types like
    // Person, Address, etc exist only to make programmers' lifes easier. ;-)
    QSharedPointer<PersistableModelElement> queryElement =
            QSharedPointer<PersistableModelElement>(new PersistableModelElement());
    queryElement->setProperty("_tableName_", element->getProperty("_tableName_"));
    queryElement->setProperty("_id_", element->getProperty("_id_"));
    QList<QSharedPointer<PersistableModelElement> > results;

    find(queryElement, results);
    result = not results.isEmpty();

    return result;
}

void SQLiteDataBaseFacade::makeStoreColumnLists(
        const QSharedPointer<PersistableModelElement> &element,
        QStringList &columnList, QStringList &columnBindList,
        bool skipId)
{
    if (not element.isNull())
    {
        QStringList columnListAll = element->propertyNames();
        foreach (QString columnName, columnListAll)
        {
            if ("_tableName_" == columnName
                    || columnName.startsWith("_readOnly_"))
            {
                continue;
            }
            if ("_id_" == columnName && skipId)
            {
                continue;
            }
            columnList << columnName;
            columnBindList << QString(":%1").arg(columnName);
        }
    }
}

void SQLiteDataBaseFacade::bindStoreValues(
        const QSharedPointer<PersistableModelElement> &element,
        const QStringList &columnList, const QStringList &columnBindList,
        QSqlQuery &query)
{
    if (not element.isNull())
    {
        for (int p = 0, max = columnList.size(); p < max; p++)
        {
            QString property = columnList.at(p);
            QVariant value = element->getProperty(property);
            if (property == "_id_" && value.toInt() < 0)
            {
                value = QVariant();
            }
#ifdef ROWLO_TRACE_DB_RESULTS
            QString sValue = value.toString();
        qDebug() << QString("binding value: %1 to |%2| (value type: %3 %4)")
                    .arg(columnBindList.at(p)).arg(sValue).arg(value.type()).arg(value.typeName())
                    .toLocal8Bit().data();
#endif
            query.bindValue(columnBindList.at(p), value);
        }
    }
}

QSharedPointer<Error> SQLiteDataBaseFacade::find(
        const QSharedPointer<PersistableModelElement> &element,
        QList<QSharedPointer<PersistableModelElement> > &results)
{
    QSharedPointer<Error> error(new Error());
    if (m_sqlDataBase.isNull() || not m_sqlDataBase->isOpen())
    {
        error = QSharedPointer<Error>(new Error(QObject::tr("Database is not connected!"),
                                                 rowlo::errorcodes::DB_IS_NOT_CONNECTED));
        return error;
    }
    if (element.isNull())
    {
        error = QSharedPointer<Error>(new Error(QObject::tr("Cannot find table for null element!"),
                                                 rowlo::errorcodes::DB_CANNOT_FIND_NULL_ELEMENT));
        return error;
    }
    if (not element->isPersistable())
    {
        error = QSharedPointer<Error>(new Error(
                 QObject::tr("Element is not persistable (has no _tableName_ and/or _id_ property)!"),
                                           rowlo::errorcodes::DB_ELEMENT_NOT_PERSISTABLE));
        return error;
    }

    QString tableName = element->getProperty("_tableName_").toString();

    QStringList whereParts;
    foreach (const QString &property, element->propertyNames())
    {
        if (property.isNull() || property.isEmpty() || property == "_tableName_")
        {
            continue;
        }
        QVariant propertyValue = element->getProperty(property);
        if (property == "_id_" && (propertyValue.isNull() || propertyValue.toInt() < 0))
        {
            continue;
        }
        if (propertyValue.type() == QVariant::String)
        {
            QString wherePart = QString("%1 LIKE '%%2%'").arg(property)
                    .arg(propertyValue.toString());
            whereParts.append(wherePart);
        }
        else
        {
            QString wherePart = QString("%1 = %2").arg(property).arg(propertyValue.toString());
            whereParts.append(wherePart);
        }
    }

    QSqlQuery query(*(m_sqlDataBase));
    QString sQuery = QString("SELECT * FROM %1").arg(tableName);
    if (not whereParts.isEmpty())
    {
        QString whereClause = QString(" WHERE %1").arg(whereParts.join(" AND "));
        sQuery = sQuery.append(whereClause);
#ifdef ROWLO_TRACE_DB_RESULTS
        qDebug() << QString("select query: |%1|").arg(sQuery).toLocal8Bit().data();
#endif
    }

    if (not query.prepare(sQuery))
    {
        QString errorText = m_sqlDataBase->lastError().text();
        error = QSharedPointer<Error>(
                    new Error(QObject::tr("Failed to prepare query: |%1| DB error: %2")
                              .arg(sQuery).arg(errorText),
                              rowlo::errorcodes::DB_FAILED_TO_PREPARE_QUERY));
        return error;
    }
    if (not query.exec(sQuery))
    {
        QString errorText = m_sqlDataBase->lastError().text();
        error = QSharedPointer<Error>(
                    new Error(QObject::tr("Failed to execute query: |%1| DB error: %2")
                              .arg(sQuery).arg(errorText),
                              rowlo::errorcodes::DB_FAILED_TO_EXECUTE_QUERY));
        return error;
    }

    if (query.seek(0))
    {
        do
        {
            QSqlRecord record = query.record();
            QList<QPair<QString, QVariant> > elementValues;

            for (int v = 0, max = record.count(); v < max; ++v)
            {
                QString fieldName = record.fieldName(v);
                QVariant value = record.value(v);
                elementValues.append(QPair<QString, QVariant>(fieldName, value));
            }

            QSharedPointer<PersistableModelElement> modelElement;
            error = createModelElement(element, elementValues, modelElement);
            if (not error.isNull() && error->isError())
            {
                return error;
            }
            if (not modelElement.isNull())
            {
                results.append(modelElement);
            }
        }
        while (query.next());
    }

    return error;
}

QSharedPointer<Error> SQLiteDataBaseFacade::createModelElement(
        const QSharedPointer<PersistableModelElement> &element,
        const QList<QPair<QString, QVariant> >& elementValues,
        QSharedPointer<PersistableModelElement> &result)
{
    QSharedPointer<Error> error(new Error());
    if (element.isNull())
    {
        error = QSharedPointer<Error>(
                    new Error(QObject::tr("Cannot create model element for null template!"),
                              rowlo::errorcodes::DB_CANNOT_CREATE_MODEL_ELEMENT_NULL_TEMPLATE));
        return error;
    }
    if (not element->isPersistable())
    {
        error = QSharedPointer<Error>(new Error(
                 QObject::tr("Template has no persistence properties!"),
                                          rowlo::errorcodes::DB_TEMPLATE_WITHOUT_PERSISTENCE_INFO));
        return error;
    }

    QString classifier = element->getClassifier();

    bool factoryFound = false;
    foreach (const QSharedPointer<IModelElementFactory> factory, m_modelElementFactories)
    {
        QSharedPointer<PersistableModelElement> factored =
                factory->createModelElement(classifier);
        if (not factored.isNull())
        {
            factoryFound = true;
            result = factored;
            break;
        }
    }
    if (not factoryFound)
    {
        error = QSharedPointer<Error>(new Error(
                 QObject::tr("Template has unknown type: %1!").arg(classifier),
                                          rowlo::errorcodes::DB_TEMPLATE_HAS_UNKNOWN_TYPE));
        return error;
    }

    result->setProperty("_id_", QVariant(-1));
    result->setProperty("_tableName_", element->getProperty("_tableName_"));
    typedef QPair<QString, QVariant> PairStringVariant;
    foreach (PairStringVariant elementValue, elementValues)
    {
        QString fieldName = elementValue.first;
        QVariant value = elementValue.second;

        result->setProperty(fieldName, value);
    }

    return error;
}

QSharedPointer<Error> SQLiteDataBaseFacade::remove(
        const QSharedPointer<PersistableModelElement> &element)
{
    QList<QSharedPointer<PersistableModelElement> > elements;
    elements << element;
    return remove(elements);
}

QSharedPointer<Error> SQLiteDataBaseFacade::remove(
        const QList<QSharedPointer<PersistableModelElement> > &elements)
{
    QSharedPointer<Error> error(new Error());
    if (m_sqlDataBase.isNull() || not m_sqlDataBase->isOpen())
    {
        error = QSharedPointer<Error>(new Error(QObject::tr("Database is not connected!"),
                                                 rowlo::errorcodes::DB_IS_NOT_CONNECTED));
        return error;
    }

    foreach(QSharedPointer<PersistableModelElement> element, elements)
    {
        if (element.isNull())
        {
            error = QSharedPointer<Error>(new Error(QObject::tr("Cannot find table for null element!"),
                                                     rowlo::errorcodes::DB_CANNOT_REMOVE_NULL_ELEMENT));
            return error;
        }
        if (not element->isPersistable())
        {
            error = QSharedPointer<Error>(new Error(
                     QObject::tr("Element is not persistable (has no _tableName_ and/or _id_ property)!"),
                                               rowlo::errorcodes::DB_ELEMENT_NOT_PERSISTABLE));
            return error;
        }

        QString tableName = element->getProperty("_tableName_").toString();
        int elementId = element->getProperty("_id_").toInt();
        QSqlQuery query(*(m_sqlDataBase));
        QString sQuery = QString("DELETE FROM %1 WHERE _id_=%2").arg(tableName).arg(elementId);

        if (not query.prepare(sQuery))
        {
            QString errorText = m_sqlDataBase->lastError().text();
            error = QSharedPointer<Error>(
                        new Error(QObject::tr("Failed to prepare query: |%1| DB error: %2")
                                  .arg(sQuery).arg(errorText),
                                  rowlo::errorcodes::DB_FAILED_TO_PREPARE_QUERY));
            return error;
        }
        if (not query.exec(sQuery))
        {
            QString errorText = m_sqlDataBase->lastError().text();
            error = QSharedPointer<Error>(
                        new Error(QObject::tr("Failed to execute query: |%1| DB error: %2")
                                  .arg(sQuery).arg(errorText),
                                  rowlo::errorcodes::DB_FAILED_TO_EXECUTE_QUERY));
            return error;
        }
    }

    return error;
}

void SQLiteDataBaseFacade::registerModelElementFactory(
        const QSharedPointer<IModelElementFactory> &factory)
{
    if (not factory.isNull())
    {
        m_modelElementFactories.append(factory);
    }
}

} // namespace sqlite
} // namespace persistence
} // namespace rowlo
