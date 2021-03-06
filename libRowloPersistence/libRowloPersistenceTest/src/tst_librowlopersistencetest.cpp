/*
 * RPF - Rowlo Persistence Framework: Unit tests of basic persistence layer for data model
 * elements.
 *
 * File:    libRowloPersistenceTest/src/tst_librowlopersistencetest.cpp
 * Brief:   TestsLib definition.
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
#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "persistence//ErrorTest.h"
#include "persistence/PersistableModelElementTest.h"
#include "persistence/PersistableModelElementFactoryTest.h"

int main(int argc, char** argv)
{
    int result = 0;
    QApplication app(argc, argv);

    Q_UNUSED(app);

    rowlo::persistence::test::ErrorTest errorTest;
    result += QTest::qExec(&errorTest, argc, argv);

    rowlo::persistence::test::PersistableModelElementTest persistableModelElementTest;
    result += QTest::qExec(&persistableModelElementTest, argc, argv);

    rowlo::persistence::test::PersistableModelElementFactoryTest persistableModelElementFactoryTest;
    result += QTest::qExec(&persistableModelElementFactoryTest, argc, argv);

    qDebug() << "Failed tests: " << result;

    return result;
}
