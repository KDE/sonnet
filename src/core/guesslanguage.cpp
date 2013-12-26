/*  This file is part of the KDE libraries
    Copyright (c) 2006 Jacob R Rideout <kde@jacobrideout.net>
    Copyright (c) 2009 Jakub Stachowski <qbast@go2.pl>
    Copyright (c) 2013 Martin Sandsmark <martin.sandsmark@kde.org>
 
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.
 
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.
 
    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QStandardPaths>
#include <QtCore/QTime>

#include "guesslanguage.h"

/*
All language tags should be valid according to IETF BCP 47, as codefied in RFC 4646.
ISO 639-1 codes should be used for the language part except for cases where there
exists no code, then 639-3 codes should be used. Country codes should only be used
in special cases. Scripts can be differentiated by IANA subtags, availble here:
http://www.iana.org/assignments/language-subtag-registry
The script tags corresond to ISO 15924

An overview of the best practices concerning language tagging is available here:
http://www.w3.org/International/articles/language-tags/Overview.en.php

lang tags should use underscores (_) rather than hypens (-) to sepereate subsections. 

EXCEPTIONS:
For cases of known differences from the above tagging scheme and major
spellcheckers such aspell/hunspell/myspell, the scheme used by the spell checkers
shall be used. All exception shall be noted here:

BCP        SPELLCHECK
az-Latn    az


*/

namespace Sonnet
{

class GuessLanguagePrivate
{
public:
    GuessLanguagePrivate();
    //            language       trigram  score
    static QHash< QString, QHash<QString, int> > m_models;

    void loadModels( );
    QList< QChar::Script > findRuns(const QString& text);
    QList<QString> createOrderedModel(const QString& content);
    int distance( const QList<QString>& model, const QHash<QString,int>& knownModel );
    QStringList check(const QString & sample, const QStringList& langs);
    QStringList identify(const QString& sample, const QList< QChar::Script >& scripts);

    static QStringList BASIC_LATIN;
    static QStringList EXTENDED_LATIN;
    static QStringList ALL_LATIN;
    static QStringList CYRILLIC;
    static QStringList ARABIC;
    static QStringList DEVANAGARI;
    static QStringList PT;
    static QHash<QChar::Script, QString> s_singletons;

    const int MIN_LENGTH;
    int m_maxItems;
    double m_minConfidence;
};

QHash< QString, QHash<QString,int> > GuessLanguagePrivate::m_models;
QStringList GuessLanguagePrivate::BASIC_LATIN;
QStringList GuessLanguagePrivate::EXTENDED_LATIN;
QStringList GuessLanguagePrivate::ALL_LATIN;
QStringList GuessLanguagePrivate::CYRILLIC;
QStringList GuessLanguagePrivate::ARABIC;
QStringList GuessLanguagePrivate::DEVANAGARI;
QHash<QChar::Script, QString> GuessLanguagePrivate::s_singletons;
QStringList GuessLanguagePrivate::PT;


GuessLanguagePrivate::GuessLanguagePrivate()
        : MIN_LENGTH(20), m_maxItems(1), m_minConfidence(0)
{
    if (!BASIC_LATIN.isEmpty())
        return;

    BASIC_LATIN << "en"  << "ha" << "so"  << "id" << "la" << "sw" << "eu" << "nr" << "nso" << "zu" << "xh" << "ss" << "st" << "tn" << "ts";

    EXTENDED_LATIN << "cs" << "af" << "pl" << "hr" << "ro" << "sk" << "sl" << "tr" << "hu" << "az" << "et" << "sq" << "ca" << "es" << "fr" << "de" << "nl" << "it" << "da" << "is" << "nb" << "sv" << "fi" << "lv" << "pt" << "ve" << "lt" << "tl" << "cy" ;

    ALL_LATIN << BASIC_LATIN << EXTENDED_LATIN;

    CYRILLIC << "ru" << "uk" << "kk" << "uz" << "mn" << "sr" << "mk" << "bg" << "ky";

    ARABIC << "ar" << "fa" << "ps" << "ur";

    DEVANAGARI << "hi" << "ne";

    PT << "pt_BR" << "pt_PT";

    // NOTE mn appears twice, once for mongolian script and once for CYRILLIC
    s_singletons[QChar::Script_Armenian] = "hy";
    s_singletons[QChar::Script_Hebrew] = "he";
    s_singletons[QChar::Script_Bengali] = "bn";
    s_singletons[QChar::Script_Gurmukhi] = "pa";
    s_singletons[QChar::Script_Greek] = "el";
    s_singletons[QChar::Script_Gujarati] = "gu";
    s_singletons[QChar::Script_Hangul] = "ko";
    s_singletons[QChar::Script_Oriya] = "or";
    s_singletons[QChar::Script_Tamil] = "ta";
    s_singletons[QChar::Script_Telugu] = "te";
    s_singletons[QChar::Script_Kannada] = "kn";
    s_singletons[QChar::Script_Malayalam] = "ml";
    s_singletons[QChar::Script_Sinhala] = "si";
    s_singletons[QChar::Script_Thai] = "th";
    s_singletons[QChar::Script_Lao] = "lo";
    s_singletons[QChar::Script_Tibetan] = "bo";
    s_singletons[QChar::Script_Myanmar] = "my";
    s_singletons[QChar::Script_Georgian] = "ka";
    s_singletons[QChar::Script_Mongolian] = "mn";
    s_singletons[QChar::Script_Khmer] = "km";
    s_singletons[QChar::Script_TaiViet] = "blt";
    s_singletons[QChar::Script_Greek] = "el";
    s_singletons[QChar::Script_Coptic] = "el";
    s_singletons[QChar::Script_Katakana] = "ja";
    s_singletons[QChar::Script_Hiragana] = "ja";
    s_singletons[QChar::Script_Bopomofo] = "zh";
    s_singletons[QChar::Script_Yi] = "zh";
    s_singletons[QChar::Script_Han] = "zh";


    if (m_models.isEmpty())
        loadModels();
}

GuessLanguage::GuessLanguage() :
        d(new GuessLanguagePrivate)
{
}

GuessLanguage::~GuessLanguage()
{
    delete d;
}

QStringList GuessLanguage::identify(const QString & text) const
{

    if (text.isEmpty())
        return QList<QString>();

    return d->identify( text, d->findRuns(text) );
}

void GuessLanguage::setLimits(int n, double dx)
{
    d->m_maxItems=n;
    d->m_minConfidence=dx;
}

void GuessLanguagePrivate::loadModels()
{
    QString triMapFile = QStandardPaths::locate(QStandardPaths::GenericDataLocation, "sonnet/trigrams.map");

    QFile sin(triMapFile);
    if (!sin.open(QIODevice::ReadOnly)) {
        qWarning() << "Sonnet: Unable to load trigram models from file" << triMapFile;
        return;
    }

    QDataStream in(&sin);
    in >> m_models;
}

QList<QChar::Script> GuessLanguagePrivate::findRuns(const QString & text)
{
    const QChar *c  = text.constData();
    QChar::Script script = QChar::Script_Unknown;
    QChar::Script prevScript = script;
    QHash<QChar::Script, int> scriptCounts;

    int count = 0;
    int totalCount = 0;


    while (!c->isNull())
    {
        script = c->script();
        if (script == QChar::Script_Common)
            continue;

        if (c->isLetter())
        {
            ++count;
            ++totalCount;
            if ( prevScript != script && script != QChar::Script_Inherited)
            {

                prevScript = script;

                scriptCounts[script] += count;
                count = 0;
            }
        }
        ++c;
    }

    // add last count
    scriptCounts[script] += count;

    QList<QChar::Script> relevantScripts;

    if (totalCount == 0) return relevantScripts;


    foreach(const QChar::Script &script, scriptCounts.keys()) {
        // return run types that used for 40% or more of the string
        if (scriptCounts[script] * 100 / totalCount >= 40)
            relevantScripts << script;

        // always return basic latin if found more than 15%.
        else if (script == QChar::Script_Latin && scriptCounts[script] * 100 / totalCount >= 15)
            relevantScripts << script;
    }

    return relevantScripts;
}

QStringList GuessLanguagePrivate::identify(const QString& sample, const QList<QChar::Script>& scripts)
{
    if (sample.size() < 3)
        return QStringList();


    if ( scripts.contains(QChar::Script_Cyrillic) )
        return check( sample, CYRILLIC );

    if ( scripts.contains(QChar::Script_Arabic) || scripts.contains(QChar::Script_OldSouthArabian))
        return check( sample, ARABIC );

    if ( scripts.contains(QChar::Script_Devanagari))
        return check( sample, DEVANAGARI );


    // Try languages with unique scripts
    foreach(const QChar::Script &script, s_singletons.keys()) {
        if (scripts.contains(script))
            return QStringList(s_singletons.value(script));
    }

    //if ( scripts.contains("Latin Extended Additional") )
    //    return QStringList("vi");

    /*if ( scripts.contains("Extended Latin") )
    {
        QStringList latinLang = check( sample, EXTENDED_LATIN );
        //FIXME
        if (latinLang.first == "pt")
            return check(sample, PT);
        else
            return latinLang;
    }*/
    if (scripts.contains(QChar::Script_Latin))
        return check( sample, ALL_LATIN );

    return QStringList();
}

QStringList GuessLanguagePrivate::check(const QString & sample, const QStringList& languages)
{
    QStringList ret;
    if (sample.size() < MIN_LENGTH)
        return ret;

    QMap<int,QString> scores;
    const QList<QString> sampleTrigrams = createOrderedModel(sample);

    foreach (const QString &language, languages )
    {
        const QString languageLowercase = language.toLower();

        //qDebug() << "modelkey:" << lkey;
        if ( m_models.contains(languageLowercase) )
        {
            scores.insert(distance(sampleTrigrams, m_models[languageLowercase]), language);
        }
    }

    if ( scores.isEmpty() )
        return ret;

    int counter=0;
    double confidence=0;
    QMapIterator<int,QString> it(scores);
    it.next();
    QString prev_item=it.value();
    int prev_score=it.key();
    
    while (it.hasNext() && counter<m_maxItems && confidence<m_minConfidence) {
        it.next();
        counter++;
        confidence += (it.key() - prev_score)/(double)it.key();
        ret += prev_item;
        qDebug() << "Adding " << prev_item << " at " << prev_score << " cc " << confidence;
        prev_item=it.value();
        prev_score=it.key();
    }
    return ret;
}


QList<QString> GuessLanguagePrivate::createOrderedModel(const QString& content)
{
    QHash<QString,int> trigramCounts;
    QMap<int,QString> orderedTrigrams;

    for ( int i = 0; i < (content.size() - 2) ; ++i )
    {
        QString tri = content.mid( i, 3 ).toLower();
        trigramCounts[tri]++;
    }

    foreach (const QString &key, trigramCounts.keys())
    {
        const QChar* data=key.constData();
        bool hasTwoSpaces=(data[1].isSpace() && (data[0].isSpace() || data[2].isSpace()));
        
        if (!hasTwoSpaces) orderedTrigrams.insertMulti( - trigramCounts[key], key);
    }

    return orderedTrigrams.values();
}

int GuessLanguagePrivate::distance( const QList<QString>& model, const QHash<QString,int>& knownModel )
{
    int counter = -1;
    int dist = 0;

    static const int MAXGRAMS = 300;

    Q_FOREACH(const QString& trigram, model ) 
    {
        if ( knownModel.contains(trigram) )
            dist += qAbs( ++counter - knownModel.value(trigram) );
        else
            dist += MAXGRAMS;
        if (counter==(MAXGRAMS-1))
            break;
    }

    return dist;
}


}
