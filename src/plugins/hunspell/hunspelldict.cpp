/**
 * kspell_hunspelldict.cpp
 *
 * Copyright (C)  2009  Montel Laurent <montel@kde.org>
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
#include "hunspelldict.h"
#include "hunspelldebug.h"

#include <QFileInfo>
#include <QDebug>
#include <QTextCodec>
#include <QStringBuilder>

using namespace Sonnet;

HunspellDict::HunspellDict(const QString &lang)
    : SpellerPlugin(lang), m_speller(0)
{
    qCDebug(SONNET_HUNSPELL) << " HunspellDict::HunspellDict( const QString& lang ):" << lang;
#ifdef Q_OS_MAC
    QByteArray dirPath = QByteArrayLiteral("/System/Library/Spelling/");
    QString dic = QLatin1String(dirPath) % lang % QLatin1String(".dic");
    if (!QFileInfo(dic).exists()) {
        dirPath = QByteArrayLiteral("/Applications/LibreOffice.app/Contents/Resources/extensions/dict-") + lang.leftRef(2).toLatin1() ;
        dic = QLatin1String(dirPath) % QLatin1Char('/') % lang % QLatin1String(".dic");
        if (lang.length()==5 && !QFileInfo(dic).exists()) {
            dirPath += '-' + lang.midRef(3,2).toLatin1();
            dic = QLatin1String(dirPath) % QLatin1Char('/') % lang % QLatin1String(".dic");
        }
        dirPath += '/';
    }
#else
    const QByteArray dirPath = QByteArrayLiteral("/usr/share/myspell/dicts/");
    QString dic = QLatin1String(dirPath) % lang % QLatin1String(".dic");
#endif

    if (QFileInfo(dic).exists()) {
        m_speller = new Hunspell(QByteArray(dirPath + lang.toLatin1() + ".aff").constData(), dic.toLatin1().constData());
    }
    qCDebug(SONNET_HUNSPELL) << " dddddd " << m_speller;

}

HunspellDict::~HunspellDict()
{
    delete m_speller;
}

bool HunspellDict::isCorrect(const QString &word) const
{
    qCDebug(SONNET_HUNSPELL) << " isCorrect :" << word;
    if (!m_speller) {
        return false;
    }
    int result = m_speller->spell(word.toUtf8().constData());
    qCDebug(SONNET_HUNSPELL) << " result :" << result;
    return (result != 0);
}

QStringList HunspellDict::suggest(const QString &word) const
{
    if (!m_speller) {
        return QStringList();
    }
    char **selection;
    QStringList lst;
    int nbWord = m_speller->suggest(&selection, word.toUtf8().constData());
    for (int i = 0; i < nbWord; ++i) {
        lst << QString::fromUtf8(selection[i]);
    }
    m_speller->free_list(&selection, nbWord);
    return lst;
}

bool HunspellDict::storeReplacement(const QString &bad,
                                    const QString &good)
{
    Q_UNUSED(bad);
    Q_UNUSED(good);
    if (!m_speller) {
        return false;
    }
    qCDebug(SONNET_HUNSPELL) << "HunspellDict::storeReplacement not implemented";
    return false;
}

bool HunspellDict::addToPersonal(const QString &word)
{
    if (!m_speller) {
        return false;
    }
    m_speller->add(word.toUtf8().constData());
    return false;
}

bool HunspellDict::addToSession(const QString &word)
{
    Q_UNUSED(word);
    if (!m_speller) {
        return false;
    }
    qCDebug(SONNET_HUNSPELL) << " bool HunspellDict::addToSession not implemented";
    return false;
}
