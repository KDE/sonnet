// -*- Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil; -*-
/**
 *
 * Copyright (C)  2003  Zack Rusin <zack@kde.org>
 * Copyright (C)  2006  Laurent Montel <montel@kde.org>
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
#include "settings_p.h"

#include "loader_p.h"

#include <QtCore/QMap>
#include <QtCore/QMutableStringListIterator>
#include <QtCore/QLocale>
#include <QtCore/QSettings>

namespace Sonnet
{
class Settings::Private
{
public:
    Loader*  loader; //can't be a Ptr since we don't want to hold a ref on it
    bool     modified;

    QString defaultLanguage;
    QString defaultClient;

    bool checkUppercase;
    bool skipRunTogether;
    bool backgroundCheckerEnabled;
    bool checkerEnabledByDefault;

    int disablePercentage;
    int disableWordCount;

    QMap<QString, bool> ignore;
};

Settings::Settings(Loader *loader)
	:d(new Private)
{
    d->loader = loader;

    d->modified = false;
    d->checkerEnabledByDefault = false;
    restore();
}

Settings::~Settings()
{
    delete d;
}

void Settings::setDefaultLanguage(const QString &lang)
{
    const QStringList cs = d->loader->languages();
    if (cs.indexOf(lang) != -1 &&
        d->defaultLanguage != lang) {
        d->defaultLanguage = lang;
        d->modified = true;
        d->loader->changed();
    }
}

QString Settings::defaultLanguage() const
{
    return d->defaultLanguage;
}

void Settings::setDefaultClient(const QString &client)
{
    //Different from setDefaultLanguage because
    //the number of clients can't be even close
    //as big as the number of languages
    if (d->loader->clients().contains(client)) {
        d->defaultClient = client;
        d->modified = true;
        d->loader->changed();
    }
}

QString Settings::defaultClient() const
{
    return d->defaultClient;
}

void Settings::setCheckUppercase(bool check)
{
    if (d->checkUppercase != check) {
        d->modified = true;
        d->checkUppercase = check;
    }
}

bool Settings::checkUppercase() const
{
    return d->checkUppercase;
}

void Settings::setSkipRunTogether(bool skip)
{
    if (d->skipRunTogether != skip) {
        d->modified = true;
        d->skipRunTogether = skip;
    }
}

bool Settings::skipRunTogether() const
{
    return d->skipRunTogether;
}

void Settings::setCheckerEnabledByDefault(bool check)
{
    if (d->checkerEnabledByDefault != check) {
        d->modified = true;
        d->checkerEnabledByDefault = check;
    }
}

bool Settings::checkerEnabledByDefault() const
{
    return d->checkerEnabledByDefault;
}

void Settings::setBackgroundCheckerEnabled(bool enable)
{
    if (d->backgroundCheckerEnabled != enable) {
        d->modified = true;
        d->backgroundCheckerEnabled = enable;
    }
}

bool Settings::backgroundCheckerEnabled() const
{
    return d->backgroundCheckerEnabled;
}

void Settings::setCurrentIgnoreList(const QStringList &ignores)
{
    setQuietIgnoreList(ignores);
    d->modified = true;
}

void Settings::setQuietIgnoreList(const QStringList &ignores)
{
    d->ignore = QMap<QString, bool>();//clear out
    for (QStringList::const_iterator itr = ignores.begin();
         itr != ignores.end(); ++itr) {
        d->ignore.insert(*itr, true);
    }
}

QStringList Settings::currentIgnoreList() const
{
    return d->ignore.keys();
}

void Settings::addWordToIgnore(const QString &word)
{
    if (!d->ignore.contains(word)) {
        d->modified = true;
        d->ignore.insert( word, true );
    }
}

bool Settings::ignore( const QString& word )
{
    return d->ignore.contains( word );
}

int Settings::disablePercentageWordError() const
{
    return d->disablePercentage;
}

int Settings::disableWordErrorCount() const
{
    return d->disableWordCount;
}

void Settings::save()
{
    QSettings settings("KDE", "Sonnet");
    settings.setValue("defaultClient", d->defaultClient);
    settings.setValue("defaultLanguage", d->defaultLanguage);
    settings.setValue("checkUppercase", d->checkUppercase);
    settings.setValue("skipRunTogether", d->skipRunTogether);
    settings.setValue("backgroundCheckerEnabled", d->backgroundCheckerEnabled);
    settings.setValue("checkerEnabledByDefault", d->checkerEnabledByDefault);
    QString defaultLanguage = QString::fromLatin1( "ignore_%1" ).arg(d->defaultLanguage);
    if(settings.contains(defaultLanguage) && d->ignore.isEmpty())
        settings.remove(defaultLanguage);
    else if(!d->ignore.isEmpty())
        settings.setValue(defaultLanguage, QStringList(d->ignore.keys()));
}

void Settings::restore()
{
    QSettings settings("KDE", "Sonnet");
    d->defaultClient = settings.value("defaultClient", QString()).toString();
    d->defaultLanguage = settings.value("defaultLanguage", QLocale::system().bcp47Name()).toString();

    //same defaults are in the default filter (filter.cpp)
    d->checkUppercase = settings.value("checkUppercase", true).toBool();
    d->skipRunTogether = settings.value("skipRunTogether", true).toBool();
    d->backgroundCheckerEnabled = settings.value("backgroundCheckerEnabled", true).toBool();
    d->checkerEnabledByDefault = settings.value("checkerEnabledByDefault", false).toBool();
    d->disablePercentage = settings.value("Sonnet_AsYouTypeDisablePercentage", 42).toInt();
    d->disableWordCount = settings.value("Sonnet_AsYouTypeDisableWordCount", 100).toInt();

    const QString ignoreEntry = QString::fromLatin1( "ignore_%1" ).arg(d->defaultLanguage);
    const QStringList ignores = settings.value(ignoreEntry, QStringList()).toStringList();
    setQuietIgnoreList(ignores);
}


bool Settings::modified() const
{
    return d->modified;
}

void Settings::setModified(bool modified)
{
    d->modified = modified;
}

}

