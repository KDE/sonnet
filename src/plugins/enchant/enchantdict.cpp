/*
 * SPDX-FileCopyrightText: 2006 Zack Rusin <zack@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */
#include "enchantdict.h"
#include "enchantclient.h"

#include <QTextCodec>
#include <QDebug>

using namespace Sonnet;

QSpellEnchantDict::QSpellEnchantDict(QSpellEnchantClient *client, EnchantBroker *broker,
                                     EnchantDict *dict, const QString &language)
    : SpellerPlugin(language)
    , m_broker(broker)
    , m_dict(dict)
    , m_client(client)
{
    qDebug() << "Enchant dict for" << language << dict;
}

QSpellEnchantDict::~QSpellEnchantDict()
{
    //Enchant caches dictionaries, so it will always return the same one.
    // therefore we do not want to delete the EnchantDict here but in the
    // client when it knows that nothing is using it anymore
    m_client->removeDictRef(m_dict);
}

bool QSpellEnchantDict::isCorrect(const QString &word) const
{
    int wrong = enchant_dict_check(m_dict, word.toUtf8().constData(),
                                   word.toUtf8().length());
    return !wrong;
}

QStringList QSpellEnchantDict::suggest(const QString &word) const
{
    /* Needed for Unicode conversion */
    QTextCodec *codec = QTextCodec::codecForName("utf8");

    size_t number = 0;
    char **suggestions
        = enchant_dict_suggest(m_dict, word.toUtf8().constData(), word.toUtf8().length(),
                               &number);

    QStringList qsug;
    for (size_t i = 0; i < number; ++i) {
        qsug.append(codec->toUnicode(suggestions[i]));
    }

    if (suggestions && number) {
        enchant_dict_free_string_list(m_dict, suggestions);
    }
    return qsug;
}

bool QSpellEnchantDict::storeReplacement(const QString &bad, const QString &good)
{
    enchant_dict_store_replacement(m_dict,
                                   bad.toUtf8().constData(), bad.toUtf8().length(),
                                   good.toUtf8().constData(), good.toUtf8().length());
    return true;
}

bool QSpellEnchantDict::addToPersonal(const QString &word)
{
    qDebug() << "QSpellEnchantDict::addToPersonal: word = "
             << word;
    enchant_dict_add_to_pwl(m_dict, word.toUtf8().constData(),
                            word.toUtf8().length());
    return true;
}

bool QSpellEnchantDict::addToSession(const QString &word)
{
    enchant_dict_add_to_session(m_dict, word.toUtf8().constData(),
                                word.toUtf8().length());
    return true;
}
