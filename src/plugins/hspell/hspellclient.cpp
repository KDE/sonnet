/*
 * kspell_hspellclient.cpp
 *
 * SPDX-FileCopyrightText: 2003 Zack Rusin <zack@kde.org>
 * SPDX-FileCopyrightText: 2005 Mashrab Kuvatov <kmashrab@uni-bremen.de>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */
#include "hspellclient.h"

#include "hspelldict.h"

using namespace Sonnet;

HSpellClient::HSpellClient(QObject *parent)
    : Client(parent)
{
}

HSpellClient::~HSpellClient()
{
}

SpellerPlugin *HSpellClient::createSpeller(const QString &language)
{
    HSpellDict *ad = new HSpellDict(language);
    return ad;
}

QStringList HSpellClient::languages() const
{
    QStringList langs;
    HSpellDict ad(QStringLiteral("he"));
    if (ad.isInitialized()) {
        langs.append(QStringLiteral("he"));
    }
    return langs;
}
