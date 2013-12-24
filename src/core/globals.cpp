/*
 * Copyright (C) 2008  Zack Rusin <zack@kde.org>
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
#include "globals.h"

#include "speller.h"

#include "filter_p.h"
#include "loader_p.h"
#include "settings_p.h"
#include "tokenizer_p.h"

#include <QMap>
#include <QVector>
#include <QSet>
#include <QDebug>

#include <memory>

namespace Sonnet
{
/* a very silly implementation: uses all available dictionaries
 * to check the text, the one with the least amount of misspelling
 * is likely the language we're looking for. (unless of course
 * someone is a real terrible speller).
 */
QString detectLanguage(const QString& sentence, const QStringList& candidates)
{
    QList<Speller> spellers;
    Q_FOREACH (const QString& lang, candidates) {
        Speller sp;
        sp.setLanguage(lang);
        if (sp.isValid()) spellers << sp;
    }

    if (spellers.isEmpty()) return QString();

    QMap<QString, int> correctHits;

    WordTokenizer t(sentence);
    while (t.hasNext()) {
        QStringRef word = t.next();
        if (!t.isSpellcheckable()) continue;
        for (int i = 0; i < spellers.count(); ++i) {
            if (spellers[i].isValid() && spellers[i].isCorrect(word.toString()))
                correctHits[spellers[i].language()] += 1;
        }
    }

    if (correctHits.isEmpty()) return QString();

    QMap<QString, int>::const_iterator max = correctHits.constBegin();
    for (QMap<QString, int>::const_iterator itr = correctHits.constBegin();
         itr != correctHits.constEnd(); ++itr) {
        if (itr.value() > max.value())
            max = itr;
         }
         return max.key();
}

QString detectLanguage(const QString &sentence)
{
    Speller speller;
    QMap<QString, QString> dicts = speller.availableDictionaries();
    QSet<QString> seenLangs;
    {
        QMap<QString, QString>::const_iterator itr = dicts.constBegin();
        for (int i = 0; i < dicts.count(); ++i) {
            seenLangs.insert(itr.value());
            ++itr;
        }
    }

    return detectLanguage(sentence, seenLangs.toList());
}

// SLOW!!!
// TODO: cache this value! And then use some dbus signal to notify all apps when
// changing the default language changes.
QString defaultLanguageName()
{
    Loader *loader = Loader::openLoader();
    loader->settings()->restore();

    return loader->languageNameForCode(loader->settings()->defaultLanguage());
}

}
