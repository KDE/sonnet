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

#include <QtCore/QCoreApplication>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QStandardPaths>
#include <QtCore/QTime>
#include <QtCore/QDataStream>

#include "guesslanguage.h"
#include "loader_p.h"
#include "speller.h"
#include "tokenizer_p.h"
#include "core_debug.h"

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

// Amount of trigrams in each file
static const int MAXGRAMS = 300;

class GuessLanguagePrivate
{
public:
    GuessLanguagePrivate();
    //            language       trigram  score
    static QHash< QString, QHash<QString, int> > s_knownModels;

    void loadModels( );
    QList< QChar::Script > findRuns(const QString& text);
    QList<QString> createOrderedModel(const QString& content);
    int distance( const QList<QString>& model, const QHash<QString,int>& knownModel );
    QStringList guessFromTrigrams(const QString & sample, const QStringList& langs);
    QStringList identify(const QString& sample, const QList< QChar::Script >& scripts);
    QString guessFromDictionaries(const QString& sentence, const QStringList& candidates);

    static QStringList BASIC_LATIN;
    static QStringList EXTENDED_LATIN;
    static QStringList ALL_LATIN;
    static QStringList CYRILLIC;
    static QStringList ARABIC;
    static QStringList DEVANAGARI;
    static QStringList PT;
    static QStringList HAN;
    static QHash<QChar::Script, QString> s_singletons;

    const int MIN_LENGTH;
    int m_maxItems;
    double m_minConfidence;
};

QHash< QString, QHash<QString,int> > GuessLanguagePrivate::s_knownModels;
QStringList GuessLanguagePrivate::BASIC_LATIN;
QStringList GuessLanguagePrivate::EXTENDED_LATIN;
QStringList GuessLanguagePrivate::ALL_LATIN;
QStringList GuessLanguagePrivate::CYRILLIC;
QStringList GuessLanguagePrivate::ARABIC;
QStringList GuessLanguagePrivate::DEVANAGARI;
QStringList GuessLanguagePrivate::HAN;
QStringList GuessLanguagePrivate::PT;
QHash<QChar::Script, QString> GuessLanguagePrivate::s_singletons;


GuessLanguagePrivate::GuessLanguagePrivate()
        : MIN_LENGTH(5), m_maxItems(1), m_minConfidence(0)
{
    if (!BASIC_LATIN.isEmpty())
        return;

    BASIC_LATIN << QStringLiteral("en")
                << QStringLiteral("ha")
                << QStringLiteral("so")
                << QStringLiteral("id")
                << QStringLiteral("la")
                << QStringLiteral("sw")
                << QStringLiteral("eu")
                << QStringLiteral("nr")
                << QStringLiteral("nso")
                << QStringLiteral("zu")
                << QStringLiteral("xh")
                << QStringLiteral("ss")
                << QStringLiteral("st")
                << QStringLiteral("tn")
                << QStringLiteral("ts");

    EXTENDED_LATIN << QStringLiteral("cs")
                   << QStringLiteral("af")
                   << QStringLiteral("pl")
                   << QStringLiteral("hr")
                   << QStringLiteral("ro")
                   << QStringLiteral("sk")
                   << QStringLiteral("sl")
                   << QStringLiteral("tr")
                   << QStringLiteral("hu")
                   << QStringLiteral("az")
                   << QStringLiteral("et")
                   << QStringLiteral("sq")
                   << QStringLiteral("ca")
                   << QStringLiteral("es")
                   << QStringLiteral("fr")
                   << QStringLiteral("de")
                   << QStringLiteral("nl")
                   << QStringLiteral("it")
                   << QStringLiteral("da")
                   << QStringLiteral("is")
                   << QStringLiteral("nb")
                   << QStringLiteral("sv")
                   << QStringLiteral("fi")
                   << QStringLiteral("lv")
                   << QStringLiteral("pt")
                   << QStringLiteral("ve")
                   << QStringLiteral("lt")
                   << QStringLiteral("tl")
                   << QStringLiteral("cy");

    ALL_LATIN << BASIC_LATIN << EXTENDED_LATIN;

    CYRILLIC << QStringLiteral("ru")
             << QStringLiteral("uk")
             << QStringLiteral("kk")
             << QStringLiteral("uz")
             << QStringLiteral("mn")
             << QStringLiteral("sr")
             << QStringLiteral("mk")
             << QStringLiteral("bg")
             << QStringLiteral("ky");

    ARABIC << QStringLiteral("ar")
           << QStringLiteral("fa")
           << QStringLiteral("ps")
           << QStringLiteral("ur");

    DEVANAGARI << QStringLiteral("hi")
               << QStringLiteral("ne");

    PT << QStringLiteral("pt_BR")
       << QStringLiteral("pt_PT");

    HAN << QStringLiteral("zh")
        << QStringLiteral("ja");

    // NOTE mn appears twice, once for mongolian script and once for CYRILLIC
    s_singletons[QChar::Script_Armenian] = QStringLiteral("hy");
    s_singletons[QChar::Script_Hebrew] = QStringLiteral("he");
    s_singletons[QChar::Script_Bengali] = QStringLiteral("bn");
    s_singletons[QChar::Script_Gurmukhi] = QStringLiteral("pa");
    s_singletons[QChar::Script_Greek] = QStringLiteral("el");
    s_singletons[QChar::Script_Gujarati] = QStringLiteral("gu");
    s_singletons[QChar::Script_Hangul] = QStringLiteral("ko");
    s_singletons[QChar::Script_Oriya] = QStringLiteral("or");
    s_singletons[QChar::Script_Tamil] = QStringLiteral("ta");
    s_singletons[QChar::Script_Telugu] = QStringLiteral("te");
    s_singletons[QChar::Script_Kannada] = QStringLiteral("kn");
    s_singletons[QChar::Script_Malayalam] = QStringLiteral("ml");
    s_singletons[QChar::Script_Sinhala] = QStringLiteral("si");
    s_singletons[QChar::Script_Thai] = QStringLiteral("th");
    s_singletons[QChar::Script_Lao] = QStringLiteral("lo");
    s_singletons[QChar::Script_Tibetan] = QStringLiteral("bo");
    s_singletons[QChar::Script_Myanmar] = QStringLiteral("my");
    s_singletons[QChar::Script_Georgian] = QStringLiteral("ka");
    s_singletons[QChar::Script_Mongolian] = QStringLiteral("mn");
    s_singletons[QChar::Script_Khmer] = QStringLiteral("km");
    s_singletons[QChar::Script_TaiViet] = QStringLiteral("blt");
    s_singletons[QChar::Script_Greek] = QStringLiteral("el");
    s_singletons[QChar::Script_Coptic] = QStringLiteral("el");
    s_singletons[QChar::Script_Katakana] = QStringLiteral("ja");
    s_singletons[QChar::Script_Hiragana] = QStringLiteral("ja");
    s_singletons[QChar::Script_Bopomofo] = QStringLiteral("zh");
    s_singletons[QChar::Script_Yi] = QStringLiteral("zh");
}

GuessLanguage::GuessLanguage() :
        d(new GuessLanguagePrivate)
{
}

GuessLanguage::~GuessLanguage()
{
    delete d;
}

QString GuessLanguage::identify(const QString& text, const QStringList& suggestions) const
{
    if (text.isEmpty())
        return QString();

    // Load the model on demand
    if (d->s_knownModels.isEmpty())
        d->loadModels();

    QStringList candidates = d->identify(text, d->findRuns(text));

    // guesser was sure enough
    if (candidates.size() == 1) {
        return candidates.front();
    }

    // guesser was unable to even narrow it down to 3 candidates. In this case try fallbacks too
    if (candidates.size() == d->m_maxItems || candidates.isEmpty()) {
        candidates += suggestions;
    }
    candidates.removeDuplicates();

    QStringList availableDictionaries = Loader::openLoader()->languages();
    QStringList toDictDetect;

    // make sure that dictionary-based detection only gets languages with installed dictionaries
    Q_FOREACH(const QString& candidate, candidates) {
        if (availableDictionaries.contains(candidate)) {
            toDictDetect += candidate;
        }
    }

    QString ret;

    // only one of candidates has dictionary - use it
    if (toDictDetect.size() == 1) {
        ret = toDictDetect.front();
    } else if (!toDictDetect.isEmpty()) {
        ret = d->guessFromDictionaries(text, toDictDetect);
    }

    // dictionary-based detection did not work, return best guess from previous step
    if (ret.isEmpty() && !candidates.isEmpty()) {
        ret = candidates.front();
    }

    return ret;
}

void GuessLanguage::setLimits(int maxItems, double minConfidence)
{
    d->m_maxItems = maxItems;
    d->m_minConfidence = minConfidence;
}

void GuessLanguagePrivate::loadModels()
{
    QString triMapFile = QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral("kf5/sonnet/trigrams.map"));

    if (triMapFile.isEmpty()) {
        triMapFile = QStringLiteral("%1/../share/kf5/sonnet/trigrams.map").arg(QCoreApplication::applicationDirPath());
    }

    QFile sin(triMapFile);
    if (!sin.open(QIODevice::ReadOnly)) {
        qCWarning(SONNET_LOG_CORE) << "Sonnet: Unable to load trigram models from file" << triMapFile;
        return;
    }

    QDataStream in(&sin);
    in >> s_knownModels;

    // Sanity check
    QHashIterator<QString, QHash<QString, int>> iterator(s_knownModels);
    while (iterator.hasNext()) {
        iterator.next();
        if (iterator.value().count() < MAXGRAMS) {
            qCWarning(SONNET_LOG_CORE) << iterator.key() << "is has only" << iterator.value().count() << "trigrams, expected" << MAXGRAMS;
        }
    }
}

QList<QChar::Script> GuessLanguagePrivate::findRuns(const QString & text)
{
    QChar::Script script = QChar::Script_Unknown;
    QHash<QChar::Script, int> scriptCounts;

    int totalCount = 0;

    foreach (const QChar c, text) {
        script = c.script();

        if (script == QChar::Script_Common || script == QChar::Script_Inherited) {
            continue;
        }

        if (!c.isLetter()) {
            continue;
        }

        scriptCounts[script]++;
        totalCount++;
    }

    QList<QChar::Script> relevantScripts;

    if (totalCount == 0) return relevantScripts;

    foreach(const QChar::Script &script, scriptCounts.keys()) {
        // return run types that used for 40% or more of the string
        if (scriptCounts[script] * 100 / totalCount >= 40) {
            relevantScripts << script;
        // always return basic latin if found more than 15%.
        } else if (script == QChar::Script_Latin && scriptCounts[script] * 100 / totalCount >= 15) {
            relevantScripts << script;
        }
    }

    return relevantScripts;
}

QStringList GuessLanguagePrivate::identify(const QString& sample, const QList<QChar::Script>& scripts)
{
    if (sample.size() < MIN_LENGTH) {
        return QStringList();
    }

    if (scripts.contains(QChar::Script_Cyrillic))
        return guessFromTrigrams(sample, CYRILLIC);

    if (scripts.contains(QChar::Script_Arabic) || scripts.contains(QChar::Script_OldSouthArabian))
        return guessFromTrigrams(sample, ARABIC);

    if (scripts.contains(QChar::Script_Devanagari))
        return guessFromTrigrams(sample, DEVANAGARI);

    if (scripts.contains(QChar::Script_Han))
        return guessFromTrigrams(sample, HAN);


    // Try languages with unique scripts
    foreach(const QChar::Script &script, s_singletons.keys()) {
        if (scripts.contains(script)) {
            return QStringList(s_singletons.value(script));
        }
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
    if (scripts.contains(QChar::Script_Latin)) {
        return guessFromTrigrams(sample, ALL_LATIN);
    }

    return QStringList();
}

QStringList GuessLanguagePrivate::guessFromTrigrams(const QString & sample, const QStringList& languages)
{
    QStringList ret;
    if (sample.size() < MIN_LENGTH) {
        return ret;
    }

    QMap<int,QString> scores;
    const QList<QString> sampleTrigrams = createOrderedModel(sample);

    Q_FOREACH (const QString &language, languages) {
        const QString languageLowercase = language.toLower();

        if (s_knownModels.contains(languageLowercase)) {
            scores.insert(distance(sampleTrigrams, s_knownModels[languageLowercase]), language);
        }
    }

    if (scores.isEmpty()) {
        return ret;
    }

    int counter = 0;
    double confidence = 0;
    QMapIterator<int,QString> it(scores);
    it.next();

    QString prevItem = it.value();
    int prevScore = it.key();

    while (it.hasNext() && counter < m_maxItems && confidence < m_minConfidence) {
        it.next();
        counter++;
        confidence += (it.key() - prevScore)/(double)it.key();
        ret += prevItem;
        prevItem=it.value();
        prevScore=it.key();
    }

    return ret;
}


QList<QString> GuessLanguagePrivate::createOrderedModel(const QString& content)
{
    QHash<QString,int> trigramCounts;
    QMap<int,QString> orderedTrigrams;

    for (int i = 0; i < (content.size() - 2); ++i) {
        QString tri = content.mid(i, 3).toLower();
        trigramCounts[tri]++;
    }

    foreach (const QString &key, trigramCounts.keys()) {
        const QChar* data=key.constData();
        bool hasTwoSpaces=(data[1].isSpace() && (data[0].isSpace() || data[2].isSpace()));

        if (!hasTwoSpaces) orderedTrigrams.insertMulti( - trigramCounts[key], key);
    }

    return orderedTrigrams.values();
}

int GuessLanguagePrivate::distance(const QList<QString>& model, const QHash<QString,int>& knownModel)
{
    int counter = -1;
    int dist = 0;

    Q_FOREACH(const QString& trigram, model) {
        if (knownModel.contains(trigram)) {
            dist += qAbs(++counter - knownModel.value(trigram));
        } else {
            dist += MAXGRAMS;
        }

        if (counter==(MAXGRAMS-1)) {
            break;
        }
    }

    return dist;
}

QString GuessLanguagePrivate::guessFromDictionaries(const QString& sentence, const QStringList& candidates)
{
    // Try to see how many languages we can get spell checking for
    QList<Speller> spellers;
    Q_FOREACH (const QString& lang, candidates) {
        Speller speller(lang);
        if (speller.isValid()) {
            spellers << speller;
        }
    }

    // If there's no spell checkers, give up
    if (spellers.isEmpty()) {
        return QString();
    }

    QMap<QString, int> correctHits;

    WordTokenizer tokenizer(sentence);
    while (tokenizer.hasNext()) {
        QStringRef word = tokenizer.next();
        if (!tokenizer.isSpellcheckable()) continue;

        for (int i = 0; i < spellers.count(); ++i) {
            if (spellers[i].isValid() && spellers[i].isCorrect(word.toString())) {
                correctHits[spellers[i].language()]++;
            }
        }
    }

    if (correctHits.isEmpty()) return QString();

    QMap<QString, int>::const_iterator max = correctHits.constBegin();
    for (QMap<QString, int>::const_iterator itr = correctHits.constBegin();
        itr != correctHits.constEnd(); ++itr) {
        if (itr.value() > max.value()) {
            max = itr;
        }
    }
    return max.key();
}


}
