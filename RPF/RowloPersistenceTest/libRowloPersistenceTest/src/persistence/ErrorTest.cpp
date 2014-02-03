/*
 * RPF - Rowlo Persistence Framework: Unit tests of basic persistence layer for data model
 * elements.
 *
 * File:    libRowloPersistenceTest/src/persistence/ErrorTest.cpp
 * Brief:   Tests Error.
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
#include "persistence/ErrorTest.h"
#include "persistence/Error.h"
#include "LibRowloPersistenceMain.h"

#include <QString>
#include <QtTest>

using rowlo::persistence::Error;

namespace rowlo
{
namespace persistence
{
namespace test
{

ErrorTest::ErrorTest(QObject *parent) :
    QObject(parent)
{
}

void ErrorTest::initTestCase()
{
    LibRowloPersistenceMain libRowloPersistence;
    Q_UNUSED(libRowloPersistence)
}

void ErrorTest::cleanupTestCase()
{
}

void ErrorTest::testConstructor()
{
    QString defaultErrorMessage("No error.");
    qint32 defaultErrorCode = 0;
    QString errorMessage("An error message.");
    qint32 errorCode = 123;

    Error* error = new Error();
    QVERIFY2(error, "Error not created");
    QCOMPARE(error->errorMessage(), defaultErrorMessage);
    QCOMPARE(error->errorCode(), defaultErrorCode);
    delete error;

    error = new Error(errorMessage);
    QVERIFY2(error, "Error not created");
    QCOMPARE(error->errorMessage(), errorMessage);
    QCOMPARE(error->errorCode(), defaultErrorCode);
    delete error;

    error = new Error(errorMessage, errorCode);
    QVERIFY2(error, "Error not created");
    QCOMPARE(error->errorMessage(), errorMessage);
    QCOMPARE(error->errorCode(), errorCode);
    delete error;
}

void ErrorTest::testIsError()
{
    Error* error = new Error();
    QCOMPARE(error->isError(), false);
    delete error;

    error = new Error("An Error", 123);
    QCOMPARE(error->isError(), true);
    delete error;

    error = new Error("Negative Error", -1);
    QCOMPARE(error->isError(), true);
    delete error;
}

} // namespace test
} // namespace persistence
} // namespace rowlo
