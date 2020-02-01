/*
 * SPDX-FileCopyrightText: 2003 Zack Rusin <zack@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */
#ifndef SONNET_SETTINGS_P_H
#define SONNET_SETTINGS_P_H

#include <QStringList>
#include <QString>
#include "sonnetcore_export.h"

namespace Sonnet {
class Loader;
class SettingsPrivate;
/**
 * Settings class
 */
class SONNETCORE_EXPORT Settings
{
public:
    ~Settings();

    Settings(const Settings &) = delete;
    Settings &operator=(const Settings &) = delete;

    bool modified() const;
    void setModified(bool modified);

    bool setDefaultLanguage(const QString &lang);
    QString defaultLanguage() const;

    bool setPreferredLanguages(const QStringList &lang);
    QStringList preferredLanguages() const;

    bool setDefaultClient(const QString &client);
    QString defaultClient() const;

    bool setCheckUppercase(bool);
    bool checkUppercase() const;

    bool setAutodetectLanguage(bool);
    bool autodetectLanguage() const;

    bool setSkipRunTogether(bool);
    bool skipRunTogether() const;

    bool setBackgroundCheckerEnabled(bool);
    bool backgroundCheckerEnabled() const;

    bool setCheckerEnabledByDefault(bool);
    bool checkerEnabledByDefault() const;

    bool setCurrentIgnoreList(const QStringList &ignores);
    bool addWordToIgnore(const QString &word);
    QStringList currentIgnoreList() const;
    bool ignore(const QString &word);

    void save();
    void restore();

    int disablePercentageWordError() const;
    int disableWordErrorCount() const;

private:
    void readIgnoreList();
    bool setQuietIgnoreList(const QStringList &ignores);

private:
    friend class Loader;
    explicit Settings(Loader *loader);
private:
    SettingsPrivate *const d;
};
}

#endif // SONNET_SETTINGS_P_H
