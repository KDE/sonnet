/**
 * kspell_hunspellclient.cpp
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
#include "hunspellclient.h"
#include "hunspelldict.h"
#include "hunspelldebug.h"

#include <QDir>
#include <QString>
#include <QCoreApplication>
#include <QStandardPaths>

using namespace Sonnet;

HunspellClient::HunspellClient(QObject *parent)
    : Client(parent)
{
    qCDebug(SONNET_HUNSPELL) << " HunspellClient::HunspellClient";

    QStringList dirList;
    const QString AFF_MASK = QStringLiteral("*.aff");

#if defined(Q_OS_MAC) || defined(Q_OS_WIN)
#ifdef Q_OS_MAC
    QDir lodir(QStringLiteral("/Applications/LibreOffice.app/Contents/Resources/extensions"));
#endif
#ifdef Q_OS_WIN
    QDir lodir(QStringLiteral("C:/Program Files (x86)/LibreOffice 5/share/extensions"));
#endif
    const QString DIR_MASK = QStringLiteral("dict-*");
    if (lodir.exists()) {
        foreach (const QString &d, lodir.entryList(QStringList(DIR_MASK), QDir::Dirs)) {
            dirList.append(lodir.absoluteFilePath(d));
        }
    }
#endif

    // search QStandardPaths
    dirList.append(QStandardPaths::locateAll(
                       QStandardPaths::GenericDataLocation,
                       QStringLiteral("hunspell"),
                       QStandardPaths::LocateDirectory));

    dirList.append(QStringLiteral(HUNSPELL_MAIN_DICT_PATH));
    dirList.append(QStringLiteral("%1/../share/hunspell").arg(
                       QCoreApplication::applicationDirPath()));

    for (const QString &dirString : dirList) {
        QDir dir(dirString);
        for (const QFileInfo &dict : dir.entryInfoList(QStringList(AFF_MASK), QDir::Files)) {
            m_languagePaths.insert(dict.baseName(), dict.canonicalPath());
        }
    }
}

HunspellClient::~HunspellClient()
{
}

SpellerPlugin *HunspellClient::createSpeller(const QString &language)
{
    qCDebug(SONNET_HUNSPELL)
    << " SpellerPlugin *HunspellClient::createSpeller(const QString &language) ;" << language;
    HunspellDict *ad = new HunspellDict(language, m_languagePaths.value(language));
    return ad;
}

QStringList HunspellClient::languages() const
{
    return m_languagePaths.keys();
}
