/*
 * RPF - Rowlo Persistence Framework: Unit tests of basic persistence layer for data model
 * elements.
 *
 * File:    libRowloPersistenceTest/include/persistence/PersistableModelElementTest.h
 * Brief:   Interface to test PersistableModelElement.
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
#ifndef PERSISTABLEMODELELEMENTTEST_H
#define PERSISTABLEMODELELEMENTTEST_H

#include <QObject>

namespace rowlo
{
namespace persistence
{
    class PersistableModelElement;
} // namespace persistence
} // namespace rowlo

namespace rowlo
{
namespace persistence
{
namespace test
{

class PersistableModelElementTest : public QObject
{
    Q_OBJECT
public:
    explicit PersistableModelElementTest(QObject *parent = 0);
    
private slots:
    void initTestCase();
    void init();
    void cleanup();
    void cleanupTestCase();
    void testConstructor();
    void testGetClassifier();
    void testGetPropertyNames_data();
    void testGetPropertyNames();
    void testSetProperties_data();
    void testSetProperties();
    void testGetProperties_data();
    void testGetProperties();
    void testIsPersistable();

private:
    rowlo::persistence::PersistableModelElement* m_element;
};

} // namespace test
} // namespace persistence
} // namespace rowlo

#endif // PERSISTABLEMODELELEMENTTEST_H
