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

#ifndef KSPELL_ISPELLCHECKDICT_H
#define KSPELL_ISPELLCHECKDICT_H

#include "spellerplugin_p.h"

#include "ispellcheckerclient.h"

class ISpellCheckerDict : public Sonnet::SpellerPlugin
{
public:
    explicit ISpellCheckerDict(ISpellCheckerFactory *spellCheckerFactory, const QString &language);
    ~ISpellCheckerDict() override;
    bool isCorrect(const QString &word) const override;

    QStringList suggest(const QString &word) const override;

    bool storeReplacement(const QString &bad, const QString &good) override;

    bool addToPersonal(const QString &word) override;
    bool addToSession(const QString &word) override;

private:
    // spell checker com object
    ISpellChecker *m_spellChecker = nullptr;
};

#endif
