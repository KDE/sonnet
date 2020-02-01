/**
 * SPDX-FileCopyrightText: 2006 Zack Rusin <zack@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */
#ifndef QSPELL_ENCHANTDICT_H
#define QSPELL_ENCHANTDICT_H

#include "spellerplugin_p.h"

#include <enchant.h>

class QSpellEnchantClient;

class QSpellEnchantDict : public Sonnet::SpellerPlugin
{
public:
    ~QSpellEnchantDict();
    virtual bool isCorrect(const QString &word) const;

    virtual QStringList suggest(const QString &word) const;

    virtual bool storeReplacement(const QString &bad, const QString &good);

    virtual bool addToPersonal(const QString &word);
    virtual bool addToSession(const QString &word);
protected:
    friend class QSpellEnchantClient;
    QSpellEnchantDict(QSpellEnchantClient *client, EnchantBroker *broker, EnchantDict *dict,
                      const QString &language);

private:
    EnchantBroker *m_broker;
    EnchantDict *m_dict;
    QSpellEnchantClient *m_client;
};

#endif
