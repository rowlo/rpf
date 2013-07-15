/*
 * RPF - Rowlo Persistence Framework: Unit tests for sqlite persistence layer.
 *
 * File:    libRowloSqlitePersistenceTest/include/persistence/sqlite/SQLiteDataBaseFacadeTest.h
 * Brief:   Interface of sqlite database facade unit test.
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
#ifndef SQLITEDATABASEFACADETEST_H
#define SQLITEDATABASEFACADETEST_H

#include <QObject>
#include <QString>

namespace rowlo
{
namespace persistence
{
namespace sqlite
{
    class SQLiteDataBaseFacade;
} // namespace sqlite
} // namespace persistence
} // namespace rowlo

namespace rowlo
{
namespace persistence
{
namespace sqlite
{
namespace test
{

class SQLiteDataBaseFacadeTest : public QObject
{
    Q_OBJECT
public:
    explicit SQLiteDataBaseFacadeTest(QObject *parent = 0);
    
private slots:
    void initTestCase();
    void init();
    void cleanup();
    void cleanupTestCase();
    void testConstructor();
    void testConnectToDb();
    void testDisconnectFromDb();
    void testResetDb();
    void testStore();
    void testFind();
    void testUpdate();
    void testRemove();
private:
    SQLiteDataBaseFacade* m_dbFacade;
    QString m_dataBaseFilePath;

    void insertTestTable();
    void insertTestData();
};

} // namespace test
} // namespace sqlite
} // namespace persistence
} // namespace rowlo

#endif // SQLITEDATABASEFACADETEST_H
