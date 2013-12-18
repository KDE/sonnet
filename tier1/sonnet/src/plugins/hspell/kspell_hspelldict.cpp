/**
 * kspell_hspelldict.cpp
 *
 * Copyright (C)  2003  Zack Rusin <zack@kde.org>
 * Copyright (C)  2005  Mashrab Kuvatov <kmashrab@uni-bremen.de>
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

#include "kspell_hspelldict.h"
#include <QDebug>

#include <QtCore/QTextCodec>

using namespace Sonnet;

HSpellDict::HSpellDict( const QString& lang )
    : SpellerPlugin( lang )
{
   int int_error = hspell_init( &m_speller, HSPELL_OPT_DEFAULT );
   if ( int_error == -1 ) {
      qDebug() << "HSpellDict::HSpellDict: Init failed";
    /* hspell understans only iso8859-8-i            */
    codec = QTextCodec::codecForName( "iso8859-8-i" );
    initialized = false;
  } else {
    initialized = true;
  }
}

HSpellDict::~HSpellDict()
{
    /* It exists in =< hspell-0.8 */
    if (initialized)
      hspell_uninit( m_speller );
}

bool HSpellDict::isCorrect( const QString& word ) const
{
    qDebug() << "HSpellDict::check word = " << word;
    int preflen;
    QByteArray wordISO = codec->fromUnicode( word );
    /* returns 1 if the word is correct, 0 otherwise */
    int correct = hspell_check_word ( m_speller,
		                      wordISO,
                                      &preflen); //this going to be removed
                                                 //in next hspell releases
    /* I do not really understand what gimatria is   */
    if( correct != 1 ){
        if( hspell_is_canonic_gimatria( wordISO ) != 0 )
	correct = 1;
    }
    return correct == 1;
}

QStringList HSpellDict::suggest( const QString& word ) const
{
    QStringList qsug;
    struct corlist cl;
    int n_sugg;
    corlist_init( &cl );
    hspell_trycorrect( m_speller, codec->fromUnicode( word ), &cl );
    for( n_sugg = 0; n_sugg < corlist_n( &cl ); n_sugg++){
	    qsug.append( codec->toUnicode( corlist_str( &cl, n_sugg) ) );
    }
    corlist_free( &cl );
    return qsug;
}

bool HSpellDict::storeReplacement( const QString& bad,
                                   const QString& good )
{
    // hspell-0.9 cannot do this
    qDebug() << "HSpellDict::storeReplacement: Sorry, cannot.";
    return false;
}

bool HSpellDict::addToPersonal( const QString& word )
{
    // hspell-0.9 cannot do this
    qDebug() << "HSpellDict::addToPersonal: Sorry, cannot.";
    return false;
}

bool HSpellDict::addToSession( const QString& word )
{
    // hspell-0.9 cannot do this
    qDebug() << "HSpellDict::addToSession: Sorry, cannot.";
    return false;
}
