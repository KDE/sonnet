/**
 * kspell_aspellclient.cpp
 *
 * Copyright (C)  2003  Zack Rusin <zack@kde.org>
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
#include "kspell_aspellclient.h"

#include "kspell_aspelldict.h"

#include <QDebug>
#include <QtPlugin>

using namespace Sonnet;

#ifdef Q_OS_WIN
#define ASPELL_DATA_ROOT "/lib/aspell-0.60/"

QString aspell_data_dir() {
    // TODO: is this still valid on Windows?
    // A generated config-aspell.h (or config-kspell.h, if shared) should be added then, to define CMAKE_INSTALL_PREFIX
    return QLatin1String(CMAKE_INSTALL_PREFIX  ASPELL_DATA_ROOT );
}
#endif

ASpellClient::ASpellClient( QObject *parent)
    : Client( parent )
{
    m_config = new_aspell_config();
#ifdef Q_OS_WIN
    aspell_config_replace( m_config, "data-dir", aspell_data_dir().toLocal8Bit().data());
    aspell_config_replace( m_config, "dict-dir", aspell_data_dir().toLocal8Bit().data());
#endif
}

ASpellClient::~ASpellClient()
{
    delete_aspell_config( m_config );
}

SpellerPlugin *ASpellClient::createSpeller(const QString &language)
{
    ASpellDict *ad = new ASpellDict( language );
    return ad;
}

QStringList ASpellClient::languages() const
{
    AspellDictInfoList *l = get_aspell_dict_info_list( m_config );
    AspellDictInfoEnumeration *el = aspell_dict_info_list_elements( l );

    QStringList langs;
    const AspellDictInfo *di = 0;
    while ( ( di = aspell_dict_info_enumeration_next( el ) ) ) {
        langs.append( QString::fromLatin1(di->name) );
    }

    delete_aspell_dict_info_enumeration( el );

    return langs;
}

