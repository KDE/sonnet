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

#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QDir>
#include <QTextCodec>
#include <QTextStream>
#include <QStringBuilder>
#include <QCoreApplication>

using namespace Sonnet;

static const QString composeDictName(const QByteArray &dirPath, const QString &lang)
{
    return QFile::decodeName(dirPath+'/') + lang + QStringLiteral(".dic");
}

HunspellDict::HunspellDict(const QString &lang)
    : SpellerPlugin(lang)
    , m_speller(0)
    , m_codec(0)
{
    qCDebug(SONNET_HUNSPELL) << " HunspellDict::HunspellDict( const QString& lang ):" << lang;

    QByteArray dirPath = QByteArrayLiteral(HUNSPELL_MAIN_DICT_PATH);
    QString dic = composeDictName(dirPath, lang);

    if (!QFileInfo::exists(dic)) {
        dirPath = QFile::encodeName(QCoreApplication::applicationDirPath()) + QByteArrayLiteral("/../share/hunspell");
        dic = composeDictName(dirPath, lang);
    }

#if defined(Q_OS_MAC) || defined(Q_OS_WIN)
    if (!QFileInfo::exists(dic)) {
#ifdef Q_OS_MAC
        dirPath = QByteArrayLiteral("/Applications/LibreOffice.app/Contents/Resources/extensions/dict-") + lang.leftRef(2).toLatin1();
#endif
#ifdef Q_OS_WIN
        dirPath = QByteArrayLiteral("C:/Program Files (x86)/LibreOffice 5/share/extensions/dict-") + lang.leftRef(2).toLatin1();
#endif
        dic = composeDictName(dirPath, lang);
        if (lang.length()==5 && !QFileInfo::exists(dic)) {
            dirPath += '-' + lang.midRef(3,2).toLatin1();
            dic = composeDictName(dirPath, lang);
        }
    }
#endif

    if (QFileInfo::exists(dic)) {
        m_speller = new Hunspell(QByteArray(dirPath + "/" + lang.toLatin1() + ".aff").constData(), dic.toLatin1().constData());
        m_codec = QTextCodec::codecForName(m_speller->get_dic_encoding());
        if (!m_codec) {
            qWarning() << "Failed to find a text codec for name" << m_speller->get_dic_encoding() << "defaulting to locale text codec";
            m_codec = QTextCodec::codecForLocale();
            Q_ASSERT(m_codec);
        }
    }
    QString userDic = QDir::home().filePath(QLatin1String(".hunspell_") % lang);
    QFile userDicFile(userDic);
    if (userDicFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCDebug(SONNET_HUNSPELL) << "Load a user dictionary" << userDic;
        QTextStream userDicIn(&userDicFile);
        while (!userDicIn.atEnd()) {
            QString word = userDicIn.readLine();
            if (word.contains(QLatin1Char('/'))) {
                QStringList wordParts = word.split(QLatin1Char('/'));
                m_speller->add_with_affix(toDictEncoding(wordParts.at(0)).constData(), toDictEncoding(wordParts.at(1)).constData());
            } if (word.at(0) == QLatin1Char('*')) {
                m_speller->remove(toDictEncoding(word.mid(1)).constData());
            } else {
                m_speller->add(toDictEncoding(word).constData());
            }
        }
        userDicFile.close();
    }
    qCDebug(SONNET_HUNSPELL) << " dddddd " << m_speller;
}

HunspellDict::~HunspellDict()
{
    delete m_speller;
}

QByteArray HunspellDict::toDictEncoding(const QString& word) const
{
    return m_codec->fromUnicode(word);
}

bool HunspellDict::isCorrect(const QString &word) const
{
    qCDebug(SONNET_HUNSPELL) << " isCorrect :" << word;
    if (!m_speller) {
        return false;
    }
    int result = m_speller->spell(toDictEncoding(word).constData());
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
    int nbWord = m_speller->suggest(&selection, toDictEncoding(word).constData());
    for (int i = 0; i < nbWord; ++i) {
        lst << m_codec->toUnicode(selection[i]);
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
    m_speller->add(toDictEncoding(word).constData());
    QString userDic = QDir::home().filePath(QLatin1String(".hunspell_") % language());
    QFile userDicFile(userDic);
    if (userDicFile.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&userDicFile);
        out << word << '\n';
        userDicFile.close();
        return true;
    }

    return false;
}

bool HunspellDict::addToSession(const QString &word)
{
    if (!m_speller) {
        return false;
    }
    int r = m_speller->add(toDictEncoding(word).constData());
    return r == 0;
}
