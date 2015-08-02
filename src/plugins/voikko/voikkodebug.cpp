/**
 * voikkodebug.cpp
 *
 * Copyright (C)  2015  Jesse Jaara <jesse.jaara@gmail.com>
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

#include "voikkodebug.h"

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
// logging category for this framework, default: log stuff >= warning
Q_LOGGING_CATEGORY(SONNET_VOIKKO, "sonnet.plugins.voikko", QtWarningMsg)
#else
Q_LOGGING_CATEGORY(SONNET_VOIKKO, "sonnet.plugins.voikko")
#endif
