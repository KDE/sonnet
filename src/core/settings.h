/*
 * SPDX-FileCopyrightText: 2020 Benjamin Port <benjamin.port@enioka.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */
#ifndef SONNET_SETTINGS_H
#define SONNET_SETTINGS_H

#include <QStringList>
#include <QString>
#include <QtCore/QObject>

#include "sonnetcore_export.h"

namespace Sonnet {
class Loader;
class SettingsImpl;
class SettingsPrivate;

class SONNETCORE_EXPORT Settings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool skipUppercase READ skipUppercase WRITE setSkipUppercase)
    Q_PROPERTY(bool autodetectLanguage READ autodetectLanguage WRITE setAutodetectLanguage)
    Q_PROPERTY(bool backgroundCheckerEnabled READ backgroundCheckerEnabled WRITE setBackgroundCheckerEnabled)
    Q_PROPERTY(bool checkerEnabledByDefault READ checkerEnabledByDefault WRITE setCheckerEnabledByDefault)
    Q_PROPERTY(bool skipRunTogether READ skipRunTogether WRITE setSkipRunTogether)
    Q_PROPERTY(QStringList currentIgnoreList READ currentIgnoreList WRITE setCurrentIgnoreList)
    Q_PROPERTY(QStringList preferredLanguages READ preferredLanguages WRITE setPreferredLanguages)
    Q_PROPERTY(QString defaultLanguage READ defaultLanguage WRITE setDefaultLanguage)

public:
    explicit Settings(QObject *parent = nullptr);
    ~Settings() override;

    void setDefaultLanguage(const QString &lang);
    QString defaultLanguage() const;

    void setPreferredLanguages(const QStringList &lang);
    QStringList preferredLanguages() const;

    void setDefaultClient(const QString &client);
    QString defaultClient() const;

    void setSkipUppercase(bool);
    bool skipUppercase() const;

    void setAutodetectLanguage(bool);
    bool autodetectLanguage() const;

    void setSkipRunTogether(bool);
    bool skipRunTogether() const;

    void setBackgroundCheckerEnabled(bool);
    bool backgroundCheckerEnabled() const;

    void setCheckerEnabledByDefault(bool);
    bool checkerEnabledByDefault() const;

    void setCurrentIgnoreList(const QStringList &ignores);
    bool addWordToIgnore(const QString &word);
    QStringList currentIgnoreList() const;
    bool ignore(const QString &word);

    QStringList clients() const;
    bool modified() const;

    void save();

    static QStringList defaultIgnoreList();
    static bool defaultSkipUppercase();
    static bool defaultAutodetectLanguage();
    static bool defaultBackgroundCheckerEnabled();
    static bool defaultCheckerEnabledByDefault();
    static bool defauktSkipRunTogether();
    static QString defaultDefaultLanguage();
    static QStringList defaultPreferredLanguages();

private:
    friend class Loader;
    SettingsPrivate *const d;
};
}

#endif //SONNET_SETTINGS_H
