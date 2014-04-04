/*
 * RPF - Rowlo Persistence Framework: Unit tests of basic persistence layer for data model
 * elements.
 *
 * File:    libRowloPersistenceTest/src/persistence/PersistableModelElementTest.cpp
 * Brief:   Tests PersistableModelElement.
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
#include "persistence/PersistableModelElementTest.h"
#include "persistence/PersistableModelElement.h"
#include "persistence/ErrorCodes.h"
#include "LibRowloPersistenceMain.h"

#include <QString>
#include <QtTest>

using rowlo::persistence::Error;
using rowlo::persistence::PersistableModelElement;

namespace rowlo
{
namespace persistence
{
namespace test
{

PersistableModelElementTest::PersistableModelElementTest(QObject *parent) :
    QObject(parent),
    m_element(0)
{
}

void PersistableModelElementTest::initTestCase()
{
    LibRowloPersistenceMain libRowloPersistence;
    Q_UNUSED(libRowloPersistence)
}

void PersistableModelElementTest::init()
{
    m_element = new PersistableModelElement();
    m_element->setProperty("mFoo", "mFooValue");
    m_element->setProperty("mBar", "mBarValue");
    m_element->setProperty("mBuf", "mBufReadOnlyValue", true);
}

void PersistableModelElementTest::cleanup()
{
    delete m_element;
}

void PersistableModelElementTest::cleanupTestCase()
{
}

void PersistableModelElementTest::testConstructor()
{
    PersistableModelElement* element = new PersistableModelElement();
    QVERIFY2(element, "PersistableModelElement was not created!");
    delete element;
}

void PersistableModelElementTest::testGetClassifier()
{
    QString expected("rowlo::persistence::PersistableModelElement");
    PersistableModelElement* element = new PersistableModelElement();
    QCOMPARE(element->getClassifier(), expected);
    delete element;
}

void PersistableModelElementTest::testGetPropertyNames_data()
{
    QTest::addColumn<QString>("propertyName");
    QTest::addColumn<bool>("expected");

    QTest::newRow("mFoo_ok") << "mFoo" << true;
    QTest::newRow("mBar_ok") << "mBar" << true;
    QTest::newRow("mBuf_ok") << "mBuf" << true;
    QTest::newRow("fwiffo_NOT_OK") << "fwiffo" << false;
}

void PersistableModelElementTest::testGetPropertyNames()
{
    QFETCH(QString, propertyName);
    QFETCH(bool, expected);

    QStringList keys = m_element->propertyNames();
    QCOMPARE(keys.contains(propertyName), expected);
}

void PersistableModelElementTest::testSetProperties_data()
{
    QTest::addColumn<QString>("propertyName");
    QTest::addColumn<QVariant>("propertyValue");
    QTest::addColumn<bool>("propertyIsReadOnly");
    QTest::addColumn<int>("expectedErrorCode");
    QTest::addColumn<int>("expectedErrorCode2");

    QTest::newRow("NullProperty")
            << QString() << QVariant("waddaValue") << false
            << rowlo::errorcodes::PROPERTY_MUST_NOT_BE_NULL << 0;
    QTest::newRow("errorOnReadOnlyFlag")
            << "_readOnly_foo" << QVariant("fooValue") << false
            << rowlo::errorcodes::MUST_NOT_SET_READONLY_FLAG << 0;
    QTest::newRow("errorOnReadOnlyProperty")
            << "bar" << QVariant("barValue") << true
            << 0 << rowlo::errorcodes::MUST_NOT_CHANGE_READONLY_PROPERTY;
    QTest::newRow("NoError") << "buz" << QVariant("buzValue") << false << 0 << 0;
    QTest::newRow("AddNullValue") << "goo" << QVariant() << false << 0 << 0;
    QTest::newRow("ClearExistingValue") << "mBar" << QVariant() << false << 0 << 0;
    QTest::newRow("ClearExistingReadOnlyValue") << "mBuf" << QVariant() << true << 0 << 0;
    QList<QVariant> vList;
    vList << QVariant(1);
    QTest::newRow("errorOnWrongStorageNametype")
            << "_storageName_" << QVariant(vList) << false
            << rowlo::errorcodes::PROPERTY_STORAGENAME_IS_NO_QSTRING << 0;
    QTest::newRow("errorOnWrongIdNametype")
            << "_id_" << QVariant(vList) << false
            << rowlo::errorcodes::PROPERTY_ID_IS_NO_INT << 0;
}

void PersistableModelElementTest::testSetProperties()
{
    QFETCH(QString, propertyName);
    QFETCH(QVariant, propertyValue);
    QFETCH(bool, propertyIsReadOnly);
    QFETCH(int, expectedErrorCode);
    QFETCH(int, expectedErrorCode2);

    QSharedPointer<Error> error =
            m_element->setProperty(propertyName, propertyValue, propertyIsReadOnly);

    QCOMPARE(error->errorCode(), expectedErrorCode);

    if (propertyIsReadOnly)
    {
        error = m_element->setProperty(propertyName, "dummyValue", propertyIsReadOnly);
        QCOMPARE(error->errorCode(), expectedErrorCode2);
    }
}

void PersistableModelElementTest::testGetProperties_data()
{
    QTest::addColumn<QString>("propertyName");
    QTest::addColumn<QVariant>("expectedValue");

    QTest::newRow("getExistingProperty") << "mFoo" << QVariant("mFooValue");
    QTest::newRow("getUnknownProperty") << "fwiffo" << QVariant();
}

void PersistableModelElementTest::testGetProperties()
{
    QFETCH(QString, propertyName);
    QFETCH(QVariant, expectedValue);

    QVariant value = m_element->getProperty(propertyName);

    QCOMPARE(value.isNull(), expectedValue.isNull());
    if (not value.isNull())
    {
        QCOMPARE(value.type(), expectedValue.type());
        if (value.canConvert<QString>())
        {
            QCOMPARE(value.toString(), expectedValue.toString());
        }
    }
}

void PersistableModelElementTest::testIsPersistable()
{
    PersistableModelElement* element = new PersistableModelElement();
    QCOMPARE(element->isPersistable(), false);

    element->setProperty("_storageName_", QVariant("fooBar"), true);
    QCOMPARE(element->isPersistable(), false);
    element->setProperty("_storageName_", QVariant(), true);
    QCOMPARE(element->isPersistable(), false);
    element->setProperty("_id_", QVariant(123456), true);
    QCOMPARE(element->isPersistable(), false);
    element->setProperty("_id_", QVariant(), true);
    QCOMPARE(element->isPersistable(), false);

    element->setProperty("_storageName_", QVariant("fooBar"), true);
    element->setProperty("_id_", QVariant(123456), true);
    QCOMPARE(element->isPersistable(), true);

    delete element;
}

} // namespace test
} // namespace persistence
} // namespace rowlo
