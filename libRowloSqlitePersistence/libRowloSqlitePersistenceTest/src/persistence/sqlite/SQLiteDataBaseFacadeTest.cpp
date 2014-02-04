/*
 * RPF - Rowlo Persistence Framework: Unit tests for sqlite persistence layer.
 *
 * File:    libRowloSqlitePersistenceTest/src/persistence/sqlite/SQLiteDataBaseFacadeTest.cpp
 * Brief:   Unit test of sqlite database facade.
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
#include "persistence/sqlite/SQLiteDataBaseFacadeTest.h"
#include "persistence/PersistableModelElement.h"
#include "persistence/PersistableModelElementFactory.h"
#include "persistence/sqlite/SQLiteDataBaseFacade.h"
#include "persistence/Error.h"
#include "persistence/sqlite/ErrorCodes.h"
#include "LibRowloSqlitePersistenceMain.h"

#include <QFile>
#include <QList>
#include <QSharedPointer>
#include <QSqlQuery>
#include <QString>
#include <QTextStream>
#include <QtTest>

using rowlo::persistence::Error;
using rowlo::persistence::PersistableModelElement;
using rowlo::persistence::PersistableModelElementFactory;
using rowlo::persistence::sqlite::SQLiteDataBaseFacade;

namespace rowlo
{
namespace persistence
{
namespace sqlite
{
namespace test
{

SQLiteDataBaseFacadeTest::SQLiteDataBaseFacadeTest(QObject *parent) :
    QObject(parent),
    m_dbFacade(0),
    m_dataBaseFilePath(QDir::currentPath() + QDir::separator() + "data.db.sql3")
{
}

void SQLiteDataBaseFacadeTest::initTestCase()
{
    LibRowloSqlitePersistence libRowloSqlitePersistence;
    Q_UNUSED(libRowloSqlitePersistence);
}

void SQLiteDataBaseFacadeTest::init()
{
    m_dbFacade = new SQLiteDataBaseFacade();
    QSharedPointer<IModelElementFactory> factory(new PersistableModelElementFactory());
    m_dbFacade->registerModelElementFactory(factory);
}

void SQLiteDataBaseFacadeTest::cleanup()
{
    m_dbFacade->resetDb();
    delete m_dbFacade;
}

void SQLiteDataBaseFacadeTest::cleanupTestCase()
{
}

void SQLiteDataBaseFacadeTest::testConstructor()
{
    QVERIFY2(m_dbFacade, "SQLiteDataBaseFacade was not created!");
    QCOMPARE(m_dbFacade->isSqlDataBase(), true);
    QVERIFY2(m_dbFacade->connectedSqlDataBase().isNull(), "Database should not be set!");
}

void SQLiteDataBaseFacadeTest::testConnectToDb()
{
    QFile file(m_dataBaseFilePath);
    file.remove();
    bool result = file.open(QFile::WriteOnly);
    QCOMPARE(result, true);
    QTextStream fileInputStream(&file);
    fileInputStream << "some none sqlite3 content";
    endl(fileInputStream);
    file.close();
    QCOMPARE(file.exists(), true);

    result = file.setPermissions(0);
    QCOMPARE(result, true);
    QSharedPointer<Error> error = m_dbFacade->connectToDb();
    QCOMPARE(error.isNull(), false);
    QCOMPARE(error->isError(), true);
    QCOMPARE(error->errorCode(), rowlo::errorcodes::DB_CANNOT_BE_OPENED);
    file.setPermissions(QFileDevice::ReadOwner|QFileDevice::WriteOwner);

    file.remove();
    QCOMPARE(file.exists(), false);

    error = m_dbFacade->connectToDb();
    QCOMPARE(error.isNull(), false);
    QCOMPARE(error->isError(), false);
    QCOMPARE(error->errorCode(), 0);

    SQLiteDataBaseFacade* secondDbFacade = new SQLiteDataBaseFacade();
    QVERIFY2(secondDbFacade, "SQLiteDataBaseFacade was not created!");
    QCOMPARE(secondDbFacade->isSqlDataBase(), true);
    QVERIFY2(secondDbFacade->connectedSqlDataBase().isNull(), "Database should not be set!");

    error = secondDbFacade->connectToDb();
    QCOMPARE(error.isNull(), false);
    QCOMPARE(error->isError(), true);
    QCOMPARE(error->errorCode(), rowlo::errorcodes::DB_ADDED_ALREADY);

    delete secondDbFacade;
}

void SQLiteDataBaseFacadeTest::testDisconnectFromDb()
{
    QVERIFY(m_dbFacade->connectedSqlDataBase().isNull());
    m_dbFacade->connectToDb();
    QVERIFY(not m_dbFacade->connectedSqlDataBase().isNull());

    QSharedPointer<Error> error = m_dbFacade->disconnectFromDb();
    QCOMPARE(error.isNull(), false);
    QCOMPARE(error->isError(), false);
    QCOMPARE(error->errorCode(), 0);
}

void SQLiteDataBaseFacadeTest::testResetDb()
{
    m_dbFacade->connectToDb();
    QSharedPointer<Error> error = m_dbFacade->resetDb();
    QCOMPARE(error.isNull(), false);
    QCOMPARE(error->isError(), false);
    QCOMPARE(error->errorCode(), 0);
    QFile file(m_dataBaseFilePath);
    QCOMPARE(file.exists(), false);
}

void SQLiteDataBaseFacadeTest::testStore()
{
    QSharedPointer<PersistableModelElement> element =
            QSharedPointer<PersistableModelElement>();
    m_dbFacade->connectToDb();

    insertTestTable();

    // check for DB_IS_NOT_CONNECTED error
    {
        SQLiteDataBaseFacade* secondDbFacade = new SQLiteDataBaseFacade();
        QSharedPointer<Error> error = secondDbFacade->store(element);
        QCOMPARE(error.isNull(), false);
        QCOMPARE(error->isError(), true);
        QCOMPARE(error->errorCode(), rowlo::errorcodes::DB_IS_NOT_CONNECTED);
        QVERIFY(secondDbFacade->connectedSqlDataBase().isNull());
        delete secondDbFacade;
    }

    // check for DB_CANNOT_STORE_NULL_ELEMENT error
    {
        QSharedPointer<Error> error = m_dbFacade->store(element);
        QCOMPARE(error.isNull(), false);
        QCOMPARE(error->isError(), true);
        QCOMPARE(error->errorCode(), rowlo::errorcodes::DB_CANNOT_STORE_NULL_ELEMENT);
    }

    // check for DB_ELEMENT_NOT_PERSISTABLE error
    {
        element = QSharedPointer<PersistableModelElement>(new PersistableModelElement());
        element->setProperty("foo", QVariant("bar"));
        QSharedPointer<Error> error = m_dbFacade->store(element);
        QCOMPARE(error.isNull(), false);
        QCOMPARE(error->isError(), true);
        QCOMPARE(error->errorCode(), rowlo::errorcodes::DB_ELEMENT_NOT_PERSISTABLE);
    }

    // check for DB_FAILED_TO_PREPARE_QUERY error
    {
        element->setProperty("_tableName_", QVariant("test_pme"), true);
        element->setProperty("_id_", QVariant(-1), true);
        element->setProperty("bam;", QVariant("exploit"));
        QSharedPointer<Error> error = m_dbFacade->store(element);
        QCOMPARE(error.isNull(), false);
        QCOMPARE(error->isError(), true);
        QCOMPARE(error->errorCode(), rowlo::errorcodes::DB_FAILED_TO_PREPARE_QUERY);
        element->setProperty("bam;", QVariant());
    }

    // check for successful store
    {
        element = QSharedPointer<PersistableModelElement>(new PersistableModelElement());
        element->setProperty("foo", QVariant("bar"));
        element->setProperty("_tableName_", QVariant("test_pme"), true);
        element->setProperty("_id_", QVariant(-1), true);

        QSharedPointer<Error> error = m_dbFacade->store(element);
        QCOMPARE(error.isNull(), false);
        QCOMPARE(error->isError(), false);
        QCOMPARE(error->errorCode(), 0);
    }

    // check for DB_FAILED_TO_INSERT_ELEMENT error
    {
        // column foo is UNIQUE trying to set same value again
        QSharedPointer<Error> error = m_dbFacade->store(element);
        QCOMPARE(error.isNull(), false);
        QCOMPARE(error->isError(), true);
        QCOMPARE(error->errorCode(), rowlo::errorcodes::DB_FAILED_TO_INSERT_ELEMENT);
    }
}

void SQLiteDataBaseFacadeTest::testFind()
{
    m_dbFacade->connectToDb();

    insertTestTable();
    insertTestData();
//    QFile::copy(m_dataBaseFilePath, QString(m_dataBaseFilePath).append(".backup"));

    QSharedPointer<PersistableModelElement> element;
    QList<QSharedPointer<PersistableModelElement> > results;
    QSharedPointer<Error> error;

    // check for DB_IS_NOT_CONNECTED
    {
        element = QSharedPointer<PersistableModelElement>();
        m_dbFacade->disconnectFromDb();
        error = m_dbFacade->find(element, results);
        QCOMPARE(error.isNull(), false);
        QCOMPARE(error->isError(), true);
        QCOMPARE(error->errorCode(), rowlo::errorcodes::DB_IS_NOT_CONNECTED);
        QCOMPARE(results.size(), 0);
    }

    // check for DB_CANNOT_FIND_NULL_ELEMENT
    {
        results.clear();
        element = QSharedPointer<PersistableModelElement>();
        m_dbFacade->connectToDb();
        error = m_dbFacade->find(element, results);
        QCOMPARE(error.isNull(), false);
        QCOMPARE(error->isError(), true);
        QCOMPARE(error->errorCode(), rowlo::errorcodes::DB_CANNOT_FIND_NULL_ELEMENT);
        QCOMPARE(results.size(), 0);
    }

    // check for DB_ELEMENT_NOT_PERSISTABLE
    {
        results.clear();
        element = QSharedPointer<PersistableModelElement>(new PersistableModelElement());
        element->setProperty("_tableName_", QVariant("test_pme"));
        error = m_dbFacade->find(element, results);
        QCOMPARE(error.isNull(), false);
        QCOMPARE(error->isError(), true);
        QCOMPARE(error->errorCode(), rowlo::errorcodes::DB_ELEMENT_NOT_PERSISTABLE);
        QCOMPARE(results.size(), 0);
    }

    // check for DB_FAILED_TO_PREPARE_QUERY
    {
        results.clear();
        element = QSharedPointer<PersistableModelElement>(new PersistableModelElement());
        element->setProperty("_tableName_", QVariant("test_pme;'&"));
        element->setProperty("_id_", QVariant(-1));
        error = m_dbFacade->find(element, results);
        QCOMPARE(error.isNull(), false);
        QCOMPARE(error->isError(), true);
        QCOMPARE(error->errorCode(), rowlo::errorcodes::DB_FAILED_TO_PREPARE_QUERY);
        QCOMPARE(results.size(), 0);
    }

    // check for finding all elements
    {
        results.clear();
        element = QSharedPointer<PersistableModelElement>(new PersistableModelElement());
        element->setProperty("_tableName_", QVariant("test_pme"));
        element->setProperty("_id_", QVariant(-1));
        error = m_dbFacade->find(element, results);
        QCOMPARE(error.isNull(), false);
        QCOMPARE(error->isError(), false);
        QCOMPARE(error->errorCode(), 0);

        QCOMPARE(results.size(), 2);

        QSharedPointer<PersistableModelElement> resultElement = results.at(0);
        QVERIFY(not resultElement.isNull());
        QCOMPARE(resultElement->getClassifier(),
                 QString("rowlo::persistence::PersistableModelElement"));
        QCOMPARE(resultElement->getProperty("_tableName_").toString(),
                 QString("test_pme"));
        QCOMPARE(resultElement->getProperty("_id_").toInt(), 1);
        QCOMPARE(resultElement->getProperty("foo").toString(),
                 QString("value of foo"));

        resultElement = results.at(1);
        QVERIFY(not resultElement.isNull());
        QCOMPARE(resultElement->getClassifier(),
                 QString("rowlo::persistence::PersistableModelElement"));
        QCOMPARE(resultElement->getProperty("_tableName_").toString(),
                 QString("test_pme"));
        QCOMPARE(resultElement->getProperty("_id_").toInt(), 2);
        QCOMPARE(resultElement->getProperty("foo").toString(),
                 QString("value of bar"));
    }

    // check find element by id
    {
        results.clear();
        element = QSharedPointer<PersistableModelElement>(new PersistableModelElement());
        element->setProperty("_tableName_", QVariant("test_pme"));
        element->setProperty("_id_", QVariant(1));
        error = m_dbFacade->find(element, results);
        QCOMPARE(error.isNull(), false);
        QCOMPARE(error->isError(), false);
        QCOMPARE(error->errorCode(), 0);

        QCOMPARE(results.size(), 1);

        QSharedPointer<PersistableModelElement> resultElement = results.at(0);
        QVERIFY(not resultElement.isNull());
        QCOMPARE(resultElement->getProperty("_id_").toInt(), 1);
    }
}

void SQLiteDataBaseFacadeTest::testUpdate()
{
    QSharedPointer<PersistableModelElement> element =
            QSharedPointer<PersistableModelElement>();
    m_dbFacade->connectToDb();

    insertTestTable();

    // check for DB_IS_NOT_CONNECTED error
    {
        SQLiteDataBaseFacade* secondDbFacade = new SQLiteDataBaseFacade();
        QSharedPointer<Error> error = secondDbFacade->update(element);
        QCOMPARE(error.isNull(), false);
        QCOMPARE(error->isError(), true);
        QCOMPARE(error->errorCode(), rowlo::errorcodes::DB_IS_NOT_CONNECTED);
        QVERIFY(secondDbFacade->connectedSqlDataBase().isNull());
        delete secondDbFacade;
    }

    // check for DB_CANNOT_STORE_NULL_ELEMENT error
    {
        QSharedPointer<Error> error = m_dbFacade->update(element);
        QCOMPARE(error.isNull(), false);
        QCOMPARE(error->isError(), true);
        QCOMPARE(error->errorCode(), rowlo::errorcodes::DB_CANNOT_STORE_NULL_ELEMENT);
    }

    // check for DB_ELEMENT_NOT_PERSISTABLE error
    {
        element = QSharedPointer<PersistableModelElement>(new PersistableModelElement());
        element->setProperty("foo", QVariant("bar"));
        QSharedPointer<Error> error = m_dbFacade->update(element);
        QCOMPARE(error.isNull(), false);
        QCOMPARE(error->isError(), true);
        QCOMPARE(error->errorCode(), rowlo::errorcodes::DB_ELEMENT_NOT_PERSISTABLE);
    }

    // check for DB_CANNOT_UPDATE_NON_EXISTING_ELEMENT error
    {
        element->setProperty("_tableName_", QVariant("test_pme"), true);
        element->setProperty("_id_", QVariant(-1), true);
        QSharedPointer<Error> error = m_dbFacade->update(element);
        QCOMPARE(error.isNull(), false);
        QCOMPARE(error->isError(), true);
        QCOMPARE(error->errorCode(), rowlo::errorcodes::DB_CANNOT_UPDATE_NON_EXISTING_ELEMENT);

        // store element for further tests
        error = m_dbFacade->store(element);
        QCOMPARE(error.isNull(), false);
        QCOMPARE(error->isError(), false);
        QCOMPARE(error->errorCode(), 0);

        // fetch element from DB
        QList<QSharedPointer<PersistableModelElement> > results;
        element = QSharedPointer<PersistableModelElement>(new PersistableModelElement());
        element->setProperty("_tableName_", QVariant("test_pme"));
        element->setProperty("_id_", QVariant(-1));
        error = m_dbFacade->find(element, results);
        QCOMPARE(error.isNull(), false);
        QCOMPARE(error->isError(), false);
        QCOMPARE(error->errorCode(), 0);
        QCOMPARE(results.size(), 1);
        QSharedPointer<PersistableModelElement> resultElement = results.at(0);
        QVERIFY(not resultElement.isNull());
        QCOMPARE(resultElement->getClassifier(),
                 QString("rowlo::persistence::PersistableModelElement"));
        QCOMPARE(resultElement->getProperty("_tableName_").toString(),
                 QString("test_pme"));
        QCOMPARE(resultElement->getProperty("foo").toString(),
                 QString("bar"));
        element = resultElement;
    }

    // check for DB_FAILED_TO_PREPARE_QUERY error
    {
        element->setProperty("bam;", QVariant("exploit"));
        QSharedPointer<Error> error = m_dbFacade->update(element);
        QCOMPARE(error.isNull(), false);
        QCOMPARE(error->isError(), true);
        QCOMPARE(error->errorCode(), rowlo::errorcodes::DB_FAILED_TO_PREPARE_QUERY);
        element->setProperty("bam;", QVariant());
    }

    // check for successful update
    {
        element->setProperty("foo", QVariant("no bar"));

        QSharedPointer<Error> error = m_dbFacade->update(element);
        QCOMPARE(error.isNull(), false);
        QCOMPARE(error->isError(), false);
        QCOMPARE(error->errorCode(), 0);

        QList<QSharedPointer<PersistableModelElement> > results;
        error = m_dbFacade->find(element, results);
        QCOMPARE(error.isNull(), false);
        QCOMPARE(error->isError(), false);
        QCOMPARE(error->errorCode(), 0);
        QCOMPARE(results.size(), 1);
        QSharedPointer<PersistableModelElement> resultElement = results.at(0);
        QVERIFY(not resultElement.isNull());
        QCOMPARE(resultElement->getProperty("foo").toString(), QString("no bar"));
    }

    // check for DB_FAILED_TO_UPDATE_ELEMENT error
    {
        // store dummy element for further tests
        QSharedPointer<PersistableModelElement> dummy =
                QSharedPointer<PersistableModelElement>(new PersistableModelElement());
        dummy->setProperty("_tableName_", QVariant("test_pme"));
        dummy->setProperty("_id_", QVariant(-1));
        dummy->setProperty("foo", QVariant("fuz"));
        QSharedPointer<Error> error = m_dbFacade->store(dummy);
        QCOMPARE(error.isNull(), false);
        QCOMPARE(error->isError(), false);
        QCOMPARE(error->errorCode(), 0);

        // column foo is UNIQUE trying to update to a value that's already present
        element->setProperty("foo", QVariant("fuz"));
        error = m_dbFacade->update(element);
        QCOMPARE(error.isNull(), false);
        QCOMPARE(error->isError(), true);
        QCOMPARE(error->errorCode(), rowlo::errorcodes::DB_FAILED_TO_UPDATE_ELEMENT);
    }
}

void SQLiteDataBaseFacadeTest::testRemove()
{
    QSharedPointer<PersistableModelElement> element =
            QSharedPointer<PersistableModelElement>();
    m_dbFacade->connectToDb();

    insertTestTable();
    insertTestData();

    m_dbFacade->disconnectFromDb();
    QList<QSharedPointer<PersistableModelElement> > results;
    QSharedPointer<Error> error = m_dbFacade->remove(element);
    QCOMPARE(error.isNull(), false);
    QCOMPARE(error->isError(), true);
    QCOMPARE(error->errorCode(), rowlo::errorcodes::DB_IS_NOT_CONNECTED);
    QCOMPARE(results.size(), 0);

    results.clear();
    m_dbFacade->connectToDb();
    error = m_dbFacade->remove(element);
    QCOMPARE(error.isNull(), false);
    QCOMPARE(error->isError(), true);
    QCOMPARE(error->errorCode(), rowlo::errorcodes::DB_CANNOT_REMOVE_NULL_ELEMENT);
    QCOMPARE(results.size(), 0);

    results.clear();
    element = QSharedPointer<PersistableModelElement>(new PersistableModelElement());
    element->setProperty("_tableName_", QVariant("test_pme"));
    error = m_dbFacade->remove(element);
    QCOMPARE(error.isNull(), false);
    QCOMPARE(error->isError(), true);
    QCOMPARE(error->errorCode(), rowlo::errorcodes::DB_ELEMENT_NOT_PERSISTABLE);
    QCOMPARE(results.size(), 0);

    results.clear();
    element->setProperty("_tableName_", QVariant("test_pme;'&"));
    element->setProperty("_id_", QVariant(-1));
    error = m_dbFacade->remove(element);
    QCOMPARE(error.isNull(), false);
    QCOMPARE(error->isError(), true);
    QCOMPARE(error->errorCode(), rowlo::errorcodes::DB_FAILED_TO_PREPARE_QUERY);
    QCOMPARE(results.size(), 0);

    results.clear();
    element->setProperty("_tableName_", QVariant("test_pme"));
    element->setProperty("_id_", QVariant(1));
    error = m_dbFacade->find(element, results);
    QCOMPARE(error.isNull(), false);
    QCOMPARE(error->isError(), false);
    QCOMPARE(error->errorCode(), 0);
    QCOMPARE(results.size(), 1);
    QSharedPointer<PersistableModelElement> resultElement = results.at(0);
    QVERIFY(not resultElement.isNull());
    QCOMPARE(resultElement->getProperty("_id_").toInt(), 1);
    error = m_dbFacade->remove(element);
    QCOMPARE(error.isNull(), false);
    QCOMPARE(error->isError(), false);
    QCOMPARE(error->errorCode(), 0);
    results.clear();
    error = m_dbFacade->find(element, results);
    QCOMPARE(error.isNull(), false);
    QCOMPARE(error->isError(), false);
    QCOMPARE(error->errorCode(), 0);
    QCOMPARE(results.size(), 0);
}

void SQLiteDataBaseFacadeTest::insertTestTable()
{
    QVERIFY(not m_dbFacade->connectedSqlDataBase().isNull());
    QString testTable = "test_pme";
    QString testTableFields = "_id_ INTEGER, foo TEXT UNIQUE, PRIMARY KEY(_id_ ASC)";
    // check if table exists
    QSqlQuery checkQuery(*(m_dbFacade->connectedSqlDataBase()));
    checkQuery.prepare(
                QString("SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name='%1'")
                .arg(testTable));
    if (not (checkQuery.exec() && checkQuery.last() && checkQuery.value(0).toInt() > 0))
    {
        QSqlQuery query(*(m_dbFacade->connectedSqlDataBase()));
        QString sQuery = QString("CREATE TABLE IF NOT EXISTS %1 (%2)")
                .arg(testTable).arg(testTableFields);
        query.prepare(sQuery);
        bool test = query.exec();
        QCOMPARE(test, true);
    }
}

void SQLiteDataBaseFacadeTest::insertTestData()
{
    QVERIFY(not m_dbFacade->connectedSqlDataBase().isNull());

    QSharedPointer<PersistableModelElement> element =
            QSharedPointer<PersistableModelElement>(new PersistableModelElement());
    element->setProperty("foo", QVariant("value of foo"));
    element->setProperty("_tableName_", QVariant("test_pme"), true);
    element->setProperty("_id_", QVariant(-1), true);
    QSharedPointer<Error> error = m_dbFacade->store(element);
    QCOMPARE(error.isNull(), false);
    if (error->isError())
    {
        qDebug() << error->errorMessage();
    }
    QCOMPARE(error->isError(), false);
    QCOMPARE(error->errorCode(), 0);

    element = QSharedPointer<PersistableModelElement>(new PersistableModelElement());
    element->setProperty("foo", QVariant("value of bar"));
    element->setProperty("_tableName_", QVariant("test_pme"), true);
    element->setProperty("_id_", QVariant(-1), true);
    error = m_dbFacade->store(element);
    QCOMPARE(error.isNull(), false);
    if (error->isError())
    {
        qDebug() << error->errorMessage();
    }
    QCOMPARE(error->isError(), false);
    QCOMPARE(error->errorCode(), 0);
}

} // namespace test
} // namespace sqlite
} // namespace persistence
} // namespace rowlo
