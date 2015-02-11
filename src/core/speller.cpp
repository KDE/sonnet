/**
 *
 * Copyright (C)  2007  Zack Rusin <zack@kde.org>
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
#include "speller.h"

#include "loader_p.h"
#include "settings_p.h"
#include "spellerplugin_p.h"

#include <QLocale>
#include <QCache>
#include <QSet>
#include <QDebug>

#define MAXLANGUAGES 5

namespace Sonnet
{

class Speller::Private
{
public:
    Private() : dictCache(MAXLANGUAGES), dict(0), settings(0)
    {
    }

    ~Private()
    {
        // no need to delete dict - dictCache will take care of that
    }
    void init(const QString &lang)
    {
        Loader *loader = Loader::openLoader();
        settings = loader->settings();

        language = lang;

        if (language.isEmpty()) {
            language = settings->defaultLanguage();
        }
        updateDict();
    }

    void updateDict() {
        if (dictCache.contains(language)) {
            dict = dictCache.object(language);
        } else {
            dict = Loader::openLoader()->createSpeller(language);
            dictCache.insert(language, dict);
        }
    }
    bool isValid()
    {
        if (settings->modified()) {
            recreateDict();
            settings->setModified(false);
        }
        return dict;
    }
    void recreateDict()
    {
        dictCache.clear();
        updateDict();
    }

    QCache<QString,SpellerPlugin> dictCache;
    SpellerPlugin *dict;
    Settings      *settings;

    QString        language;
};

Speller::Speller(const QString &lang)
    : d(new Private)
{
    d->init(lang);
}

Speller::~Speller()
{
    //qDebug()<<"deleting "<<this;
    delete d;
}

Speller::Speller(const Speller &speller)
    : d(new Private)
{
    d->language = speller.language();
    d->init(d->language);
}

Speller &Speller::operator=(const Speller &speller)
{
    d->language = speller.language();
    d->recreateDict();
    return *this;
}

bool Speller::isCorrect(const QString &word) const
{
    if (!d->isValid()) {
        return true;
    }
    return d->dict->isCorrect(word);
}

bool Speller::isMisspelled(const QString &word) const
{
    if (!d->isValid()) {
        return false;
    }
    return d->dict->isMisspelled(word);
}

QStringList Speller::suggest(const QString &word) const
{
    if (!d->isValid()) {
        return QStringList();
    }
    return d->dict->suggest(word);
}

bool Speller::checkAndSuggest(const QString &word,
                              QStringList &suggestions) const
{
    if (!d->isValid()) {
        return true;
    }
    return d->dict->checkAndSuggest(word, suggestions);
}

bool Speller::storeReplacement(const QString &bad,
                               const QString &good)
{
    if (!d->isValid()) {
        return false;
    }
    return d->dict->storeReplacement(bad, good);
}

bool Speller::addToPersonal(const QString &word)
{
    if (!d->isValid()) {
        return false;
    }
    return d->dict->addToPersonal(word);
}

bool Speller::addToSession(const QString &word)
{
    if (!d->isValid()) {
        return false;
    }
    return d->dict->addToSession(word);
}

QString Speller::language() const
{
    if (!d->isValid()) {
        return QString();
    }
    return d->dict->language();
}

void Speller::save()
{
    if (d->settings) {
        d->settings->save();
    }
}

void Speller::restore()
{
    if (d->settings) {
        d->settings->restore();
        d->recreateDict();
    }
}

QStringList Speller::availableBackends() const
{
    Loader *l = Loader::openLoader();
    return l->clients();
}

QStringList Speller::availableLanguages() const
{
    Loader *l = Loader::openLoader();
    return l->languages();
}

QStringList Speller::availableLanguageNames() const
{
    Loader *l = Loader::openLoader();
    return l->languageNames();
}

void Speller::setDefaultLanguage(const QString &lang)
{
    d->settings->setDefaultLanguage(lang);
}

QString Speller::defaultLanguage() const
{
    return d->settings->defaultLanguage();
}

void Speller::setDefaultClient(const QString &client)
{
    d->settings->setDefaultClient(client);
}

QString Speller::defaultClient() const
{
    return d->settings->defaultClient();
}

void Speller::setAttribute(Attribute attr, bool b)
{
    switch (attr) {
    case CheckUppercase:
        d->settings->setCheckUppercase(b);
        break;
    case SkipRunTogether:
        d->settings->setSkipRunTogether(b);
        break;
    case AutoDetectLanguage:
        d->settings->setAutodetectLanguage(b);
        break;
    }
}


bool Speller::testAttribute(Attribute attr) const
{
    switch (attr) {
    case CheckUppercase:
        return d->settings->checkUppercase();
    case SkipRunTogether:
        return d->settings->skipRunTogether();
    case AutoDetectLanguage:
        return d->settings->autodetectLanguage();
    }
    return false;
}

bool Speller::isValid() const
{
    return d->dict;
}

void Speller::setLanguage(const QString &lang)
{
    d->language = lang;
    d->updateDict();
}

QMap<QString, QString> Sonnet::Speller::availableDictionaries() const
{
    Loader *l = Loader::openLoader();
    QStringList lst = l->languages();
    QMap<QString, QString> langs;

    Q_FOREACH (const QString &tag, lst) {
        langs.insert(l->languageNameForCode(tag), tag);
    }

    return langs;
}

}
