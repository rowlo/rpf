/*
 * RPF - Rowlo Persistence Framework: Unit tests of basic persistence layer for data model
 * elements.
 *
 * File:    libRowloPersistenceTest/src/persistence/PersistableModelElementFactoryTest.cpp
 * Brief:   Tests PersistableModelElementFactory.
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
#include "persistence/PersistableModelElementFactoryTest.h"
#include "persistence/PersistableModelElementFactory.h"
#include "persistence/PersistableModelElement.h"
#include "LibRowloPersistenceMain.h"

#include <QString>
#include <QtTest>

using rowlo::persistence::PersistableModelElement;
using rowlo::persistence::PersistableModelElementFactory;

namespace rowlo
{
namespace persistence
{
namespace test
{

PersistableModelElementFactoryTest::PersistableModelElementFactoryTest(QObject *parent) :
    QObject(parent)
{
}

void PersistableModelElementFactoryTest::initTestCase()
{
    LibRowloPersistenceMain libRowloPersistence;
    Q_UNUSED(libRowloPersistence)
}

void PersistableModelElementFactoryTest::init()
{
}

void PersistableModelElementFactoryTest::cleanup()
{
}

void PersistableModelElementFactoryTest::cleanupTestCase()
{
}

void PersistableModelElementFactoryTest::testConstructor()
{
    PersistableModelElementFactory* factory = new PersistableModelElementFactory();
    QVERIFY2(factory, "PersistableModelElementFactory was not created!");
    delete factory;
}

void PersistableModelElementFactoryTest::testCreateModelElement()
{
    PersistableModelElementFactory* factory = new PersistableModelElementFactory();

    QString classifier = "foobar";
    QSharedPointer<PersistableModelElement> element = factory->createModelElement(classifier);
    QVERIFY(element.isNull());

    classifier = "rowlo::persistence::PersistableModelElement";
    element = factory->createModelElement(classifier);
    QVERIFY(not element.isNull());

    delete factory;
}

} // namespace test
} // namespace persistence
} // namespace rowlo
