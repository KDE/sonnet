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
#include "kspell_hunspelldict.h"
#include <QFileInfo>

#include <QDebug>

#include <QtCore/QTextCodec>

using namespace Sonnet;

HunspellDict::HunspellDict( const QString& lang )
    : SpellerPlugin(lang), m_speller(0)
{
    qDebug()<<" HunspellDict::HunspellDict( const QString& lang ):"<<lang;
        QString dic=QString("/usr/share/myspell/dicts/%1.dic").arg(lang);
    if (QFileInfo(dic).exists())
        m_speller = new Hunspell(QString("/usr/share/myspell/dicts/%1.aff").arg(lang).toUtf8().constData(),dic.toUtf8().constData());
    else
        m_speller = 0;
    qDebug()<<" dddddd "<<m_speller;

}

HunspellDict::~HunspellDict()
{
    delete m_speller;
}

bool HunspellDict::isCorrect(const QString &word) const
{
    qDebug()<<" isCorrect :"<<word;
    if(!m_speller)
        return false;
    int result = m_speller->spell(word.toUtf8());
    qDebug()<<" result :"<<result;
    return (result != 0) ;
}

QStringList HunspellDict::suggest(const QString &word) const
{
    if(!m_speller)
        return QStringList();
    char ** selection;
    QStringList lst;
    int nbWord = m_speller->suggest(&selection, word.toUtf8());
    for(int i = 0; i <nbWord;++i)
    {
        lst << QString::fromUtf8(selection[i]);
    }
    m_speller->free_list(&selection, nbWord);
    return lst;
}


bool HunspellDict::storeReplacement( const QString& bad,
                                   const QString& good )
{
    if (!m_speller)
        return false;
    qDebug()<<"HunspellDict::storeReplacement not implemented";
    return false;
}

bool HunspellDict::addToPersonal( const QString& word )
{
    if (!m_speller)
        return false;
    m_speller->add(word.toUtf8());
    return false;
}

bool HunspellDict::addToSession( const QString& word )
{
    if (!m_speller)
        return false;
    qDebug()<<" bool HunspellDict::addToSession not implemented";
    return false;
}
