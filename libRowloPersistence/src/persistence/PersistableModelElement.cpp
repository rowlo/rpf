/*
 * RPF - Rowlo Persistence Framework: Basic persistence layer for data model elements.
 *
 * File:    libRowloPersistence/src/persistence/PersistableModelElement.cpp
 * Brief:   Basic data class that can be persisted without deep database knowledge.
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
#include "persistence/PersistableModelElement.h"
#include "persistence/Error.h"

#include <QSharedPointer>

namespace rowlo
{
namespace persistence
{

PersistableModelElement::PersistableModelElement()
{
}

QString PersistableModelElement::getClassifier() const
{
    return "rowlo::persistence::PersistableModelElement";
}

QStringList PersistableModelElement::propertyNames()
{
    return m_properties.keys();
}

QVariant PersistableModelElement::getProperty(const QString &property) const
{
    if (not m_properties.contains(property))
    {
        return QVariant();
    }
    return m_properties[property];
}

QSharedPointer<Error> PersistableModelElement::setProperty(
        const QString &property, const QVariant &value, bool readOnly)
{
    QSharedPointer<Error> error(new Error());
    if (property.isNull())
    {
        error = QSharedPointer<Error>(new Error(QObject::tr("Property must not be null!"),
                                                rowlo::errorcodes::PROPERTY_MUST_NOT_BE_NULL));
        return error;
    }
    if (property.startsWith("_readOnly_"))
    {
        error = QSharedPointer<Error>(new Error(
                                          QObject::tr("Must not set _readOnly_ flag: %1")
                                          .arg(property),
                                          rowlo::errorcodes::MUST_NOT_SET_READONLY_FLAG));
        return error;
    }

    QString readOnlyKey = QString("_readOnly_%1").arg(property);

    if (value.isNull())
    {
        if (m_properties.contains(property))
        {
            m_properties.remove(property);
            if (m_properties.contains(readOnlyKey))
            {
                m_properties.remove(readOnlyKey);
            }
        }
        return error;
    }

    // value is not null beyond that line

    if (property == "_tableName_" && not value.canConvert<QString>())
    {
        error = QSharedPointer<Error>(new Error(
                                          QObject::tr("Property _tableName_ has no QString value!"),
                                          rowlo::errorcodes::PROPERTY_TABLENAME_IS_NO_QSTRING));
        return error;
    }
    if (property == "_id_" && not value.canConvert<int>())
    {
        error = QSharedPointer<Error>(new Error(
                                          QObject::tr("Property _id_ has no int value!"),
                                          rowlo::errorcodes::PROPERTY_ID_IS_NO_INT));
        return error;
    }

    if (m_properties.contains(readOnlyKey))
    {
        error = QSharedPointer<Error>(new Error(
                                          QObject::tr("Must not change readOnly property: %1")
                                          .arg(property),
                                          rowlo::errorcodes::MUST_NOT_CHANGE_READONLY_PROPERTY));
        return error;
    }

    m_properties[property] = value;
    if (readOnly)
    {
        m_properties[readOnlyKey] = true;
    }
    return error;
}

bool PersistableModelElement::isPersistable()
{
    return m_properties.contains("_tableName_") && m_properties.contains("_id_");
}

} // namespace persistence
} // namespace rowlo
