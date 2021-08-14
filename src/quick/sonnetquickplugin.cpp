// SPDX-FileCopyrightText: 2021 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "sonnetquickplugin.h"
#include "settings.h"
#include "spellcheckhighlighter.h"
#include <QQmlEngine>

void SonnetQuickPlugin::registerTypes(const char *uri)
{
    qmlRegisterType<SpellcheckHighlighter>(uri, 1, 0, "SpellcheckHighlighter");
    qmlRegisterType<Sonnet::Settings>(uri, 1, 0, "Settings");
}
