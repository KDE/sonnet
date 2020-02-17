/*
 * kspell_aspelldict.h
 *
 * SPDX-FileCopyrightText: 2009 Montel Laurent <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */
#ifndef KSPELL_HUNSPELLDICT_H
#define KSPELL_HUNSPELLDICT_H

#include "spellerplugin_p.h"
#include "hunspell.hxx"

class HunspellDict : public Sonnet::SpellerPlugin
{
public:
    explicit HunspellDict(const QString &lang, QString path);
    ~HunspellDict() override;
    bool isCorrect(const QString &word) const override;

    QStringList suggest(const QString &word) const override;

    bool storeReplacement(const QString &bad, const QString &good) override;

    bool addToPersonal(const QString &word) override;
    bool addToSession(const QString &word) override;

private:
    QByteArray toDictEncoding(const QString &word) const;

    Hunspell *m_speller = nullptr;
    QTextCodec *m_codec = nullptr;
};

#endif
