/**
 * SPDX-FileCopyrightText: 2006 Zack Rusin <zack@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */
#include "enchantclient.h"
#include "enchantdict.h"

#include <QDebug>

using namespace Sonnet;

static void enchantDictDescribeFn(const char *const lang_tag, const char *const provider_name,
                                  const char *const provider_desc, const char *const provider_file,
                                  void *user_data)
{
    QSpellEnchantClient *client
        = reinterpret_cast<QSpellEnchantClient *>(user_data);
    //qDebug()<<lang_tag<<provider_name<<provider_desc<<provider_file;
    Q_UNUSED(provider_name);
    Q_UNUSED(provider_desc);
    Q_UNUSED(provider_file);
    client->addLanguage(QString::fromLatin1(lang_tag));
}

QSpellEnchantClient::QSpellEnchantClient(QObject *parent)
    : Client(parent)
{
    m_broker = enchant_broker_init();
    enchant_broker_list_dicts(m_broker,
                              enchantDictDescribeFn,
                              this);
}

QSpellEnchantClient::~QSpellEnchantClient()
{
    enchant_broker_free(m_broker);
}

SpellerPlugin *QSpellEnchantClient::createSpeller(
    const QString &language)
{
    EnchantDict *dict = enchant_broker_request_dict(m_broker,
                                                    language.toUtf8().constData());

    if (!dict) {
#ifndef NDEBUG
        const char *err = enchant_broker_get_error(m_broker);
        qDebug() << "Couldn't create speller for" << language << ": " << err;
#endif
        return 0;
    } else {
        //Enchant caches dictionaries, so it will always return the same one.
        int refs = m_dictRefs[dict];
        ++refs;
        m_dictRefs[dict] = refs;
        return new QSpellEnchantDict(this, m_broker, dict, language);
    }
}

QStringList QSpellEnchantClient::languages() const
{
    return m_languages.toList();
}

void QSpellEnchantClient::addLanguage(const QString &lang)
{
    m_languages.insert(lang);
}

void QSpellEnchantClient::removeDictRef(EnchantDict *dict)
{
    int refs = m_dictRefs[dict];
    --refs;
    m_dictRefs[dict] = refs;
    if (refs <= 0) {
        m_dictRefs.remove(dict);
        enchant_broker_free_dict(m_broker, dict);
    }
}
