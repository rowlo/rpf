/*
 * RPF - Rowlo Persistence Framework: Basic persistence layer for data model elements.
 *
 * File:    libRowloPersistence/include/persistence/PersistableModelElement.h
 * Brief:   Base class for persistable data classes. Maybe subclassed but can be used as is.
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
#ifndef PERSISTABLEMODELELEMENT_H
#define PERSISTABLEMODELELEMENT_H

#include "libRowloPersistence_global.h"
#include "persistence/Error.h"

#include <QVariant>
#include <QMap>
#include <QList>
#include <QSharedPointer>
#include <QString>
#include <QStringList>

namespace rowlo
{
namespace persistence
{

class LIBROWLOPERSISTENCESHARED_EXPORT PersistableModelElement
{
public:
    PersistableModelElement();
    virtual ~PersistableModelElement();

    QStringList propertyNames() const;
    QVariant getProperty(const QString &property) const;
    QSharedPointer<Error> setProperty(
            const QString &property, const QVariant &value, bool readOnly = false);
    virtual QString getClassifier() const;
    bool isPersistable() const;

private:
    QMap<QString, QVariant> m_properties;
};

} // namespace persistence
} // namespace rowlo

Q_DECLARE_METATYPE(rowlo::persistence::PersistableModelElement)
Q_DECLARE_METATYPE(QSharedPointer<rowlo::persistence::PersistableModelElement>)
Q_DECLARE_METATYPE(QList<rowlo::persistence::PersistableModelElement>)
Q_DECLARE_METATYPE(QList<QSharedPointer<rowlo::persistence::PersistableModelElement> >)

#endif // PERSISTABLEMODELELEMENT_H
