/*
 * SPDX-FileCopyrightText: 2006 Zack Rusin <zack@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */
#ifndef QSPELL_ENCHANTCLIENT_H
#define QSPELL_ENCHANTCLIENT_H

#include "spellerplugin_p.h"
#include "client_p.h"

#include <QSet>
#include <enchant.h>

namespace Sonnet {
class SpellerPlugin;
}
using Sonnet::SpellerPlugin;

class QSpellEnchantClient : public Sonnet::Client
{
    Q_OBJECT
    Q_INTERFACES(Sonnet::Client)
    Q_PLUGIN_METADATA(IID "org.kde.Sonnet.EnchantClient")
public:
    explicit QSpellEnchantClient(QObject *parent = nullptr);
    ~QSpellEnchantClient();

    virtual int reliability() const
    {
        return 30;
    }

    virtual SpellerPlugin *createSpeller(const QString &language);

    virtual QStringList languages() const;

    virtual QString name() const
    {
        return QString::fromLatin1("Enchant");
    }

    void addLanguage(const QString &lang);

    void removeDictRef(EnchantDict *dict);

private:
    EnchantBroker *m_broker;
    QSet<QString> m_languages;
    QHash<EnchantDict *, int> m_dictRefs;
};

#endif
