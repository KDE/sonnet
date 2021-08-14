/*
 * SPDX-FileCopyrightText: 2020 Benjamin Port <benjamin.port@enioka.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */
#include "settingsimpl_p.h"

#include <QLocale>

#include "loader_p.h"
#include "settings.h"

namespace Sonnet
{
class SettingsPrivate
{
public:
    Loader *loader = nullptr;
};

Settings::Settings(QObject *parent)
    : QObject(parent)
    , d(new SettingsPrivate)
{
    d->loader = Loader::openLoader();
}

Settings::~Settings()
{
    delete d;
}

void Settings::setDefaultLanguage(const QString &lang)
{
    d->loader->settings()->setDefaultLanguage(lang);
}

QString Settings::defaultLanguage() const
{
    return d->loader->settings()->defaultLanguage();
}

void Settings::setPreferredLanguages(const QStringList &lang)
{
    d->loader->settings()->setPreferredLanguages(lang);
}

QStringList Settings::preferredLanguages() const
{
    return d->loader->settings()->preferredLanguages();
}

void Settings::setDefaultClient(const QString &client)
{
    d->loader->settings()->setDefaultClient(client);
}

QString Settings::defaultClient() const
{
    return d->loader->settings()->defaultClient();
}

void Settings::setSkipUppercase(bool skip)
{
    d->loader->settings()->setCheckUppercase(!skip);
}

bool Settings::skipUppercase() const
{
    return !d->loader->settings()->checkUppercase();
}

void Settings::setAutodetectLanguage(bool detect)
{
    d->loader->settings()->setAutodetectLanguage(detect);
}

bool Settings::autodetectLanguage() const
{
    return d->loader->settings()->autodetectLanguage();
}

void Settings::setSkipRunTogether(bool skip)
{
    d->loader->settings()->setSkipRunTogether(skip);
}

bool Settings::skipRunTogether() const
{
    return d->loader->settings()->skipRunTogether();
}

void Settings::setCheckerEnabledByDefault(bool check)
{
    d->loader->settings()->setCheckerEnabledByDefault(check);
}

bool Settings::checkerEnabledByDefault() const
{
    return d->loader->settings()->checkerEnabledByDefault();
}

void Settings::setBackgroundCheckerEnabled(bool enable)
{
    d->loader->settings()->setBackgroundCheckerEnabled(enable);
}

bool Settings::backgroundCheckerEnabled() const
{
    return d->loader->settings()->backgroundCheckerEnabled();
}

void Settings::setCurrentIgnoreList(const QStringList &ignores)
{
    d->loader->settings()->setCurrentIgnoreList(ignores);
}

QStringList Settings::currentIgnoreList() const
{
    return d->loader->settings()->currentIgnoreList();
}

QStringList Settings::clients() const
{
    return d->loader->clients();
}

void Settings::save()
{
    d->loader->settings()->save();
}

bool Settings::modified() const
{
    return d->loader->settings()->modified();
}

// default values
// A static list of KDE specific words that we want to recognize
QStringList Settings::defaultIgnoreList()
{
    QStringList l;
    l.append(QStringLiteral("KMail"));
    l.append(QStringLiteral("KOrganizer"));
    l.append(QStringLiteral("KAddressBook"));
    l.append(QStringLiteral("KHTML"));
    l.append(QStringLiteral("KIO"));
    l.append(QStringLiteral("KJS"));
    l.append(QStringLiteral("Konqueror"));
    l.append(QStringLiteral("Sonnet"));
    l.append(QStringLiteral("Kontact"));
    l.append(QStringLiteral("Qt"));
    l.append(QStringLiteral("Okular"));
    l.append(QStringLiteral("KMix"));
    l.append(QStringLiteral("Amarok"));
    l.append(QStringLiteral("KDevelop"));
    l.append(QStringLiteral("Nepomuk"));
    return l;
}

bool Settings::defaultSkipUppercase()
{
    return false;
}

bool Settings::defaultAutodetectLanguage()
{
    return true;
}

bool Settings::defaultBackgroundCheckerEnabled()
{
    return true;
}

bool Settings::defaultCheckerEnabledByDefault()
{
    return false;
}

bool Settings::defauktSkipRunTogether()
{
    return true;
}

QString Settings::defaultDefaultLanguage()
{
    return QLocale::system().name();
}

QStringList Settings::defaultPreferredLanguages()
{
    return QStringList();
}
}
