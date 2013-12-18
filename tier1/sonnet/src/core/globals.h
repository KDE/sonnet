/*
 * Copyright (C) 2008  Zack Rusin <zack@kde.org>
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */
#ifndef SONNET_GLOBALS_H
#define SONNET_GLOBALS_H

#include <QtCore/QString>

#include <sonnet/sonnetcore_export.h>

namespace Sonnet {

    /**
     * The function will return the language code for the
     * language in which it thinks the sentence which
     * has been passed to it is.
     *
     * @since 4.2
     */
    extern SONNETCORE_EXPORT QString detectLanguage(const QString &sentence);

    /**
     * @return the default language which is stored in the sonnetrc config file.
     *         This is the language the user has set globally in the control
     *         center.
     *         Note that this returns the localized language name, not the
     *         dictionary name/language code.
     *
     * @since 4.2
     */
    extern SONNETCORE_EXPORT QString defaultLanguageName();
}

#endif
