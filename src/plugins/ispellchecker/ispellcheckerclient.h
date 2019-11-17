/*  SPDX-License-Identifier: LGPL-2.0-or-later

    Copyright (C) 2019 Christoph Cullmann <cullmann@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef KSPELL_ISPELLCHECKCLIENT_H
#define KSPELL_ISPELLCHECKCLIENT_H

#include "client_p.h"

#include <windows.h>
#include <spellcheck.h>

namespace Sonnet {
class SpellerPlugin;
}
using Sonnet::SpellerPlugin;

class ISpellCheckerClient : public Sonnet::Client
{
    Q_OBJECT
    Q_INTERFACES(Sonnet::Client)
    Q_PLUGIN_METADATA(IID "org.kde.Sonnet.ISpellCheckerClient")
public:
    explicit ISpellCheckerClient(QObject *parent = nullptr);
    ~ISpellCheckerClient() override;

    int reliability() const override
    {
        return 40;
    }

    SpellerPlugin *createSpeller(const QString &language) override;

    QStringList languages() const override;

    QString name() const override
    {
        return QStringLiteral("ISpellChecker");
    }

private:
    bool m_wasCOMInitialized = false;
    ISpellCheckerFactory* m_spellCheckerFactory = nullptr;
    QStringList m_languages;
};

#endif
