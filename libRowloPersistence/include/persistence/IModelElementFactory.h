/*
 * RPF - Rowlo Persistence Framework: Basic persistence layer for data model elements.
 *
 * File:    libRowloPersistence/include/persistence/IModelElementFactory.h
 * Brief:   Model element factories are used to create specific instances for classifiers.
 *          Implementations need to be registered at IDataBaseFacades!
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
#ifndef IMODELELEMENTFACTORY_H
#define IMODELELEMENTFACTORY_H

#include "libRowloPersistence_global.h"
#include "persistence/PersistableModelElement.h"
#include <QString>
#include <QSharedPointer>

namespace rowlo
{
namespace persistence
{

class LIBROWLOPERSISTENCESHARED_EXPORT IModelElementFactory
{
public:
    virtual ~IModelElementFactory() {}
    virtual QSharedPointer<PersistableModelElement> createModelElement(
            const QString& classifier) = 0;
};

} // namespace persistence
} // namespace rowlo

#endif // IMODELELEMENTFACTORY_H
