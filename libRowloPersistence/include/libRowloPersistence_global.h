/*
 * RPF - Rowlo Persistence Framework: Basic persistence layer for data model elements.
 *
 * File:    libRowloPersistence/include/libRowloPersistence_global.h
 * Brief:   Shared library export/import declaration.
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
#ifndef LIBROWLOPERSISTENCE_GLOBAL_H
#define LIBROWLOPERSISTENCE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIBROWLOPERSISTENCE_LIBRARY)
#  define LIBROWLOPERSISTENCESHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIBROWLOPERSISTENCESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LIBROWLOPERSISTENCE_GLOBAL_H