/*
    SPDX-FileCopyrightText: 2019 Christoph Cullmann <cullmann@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KSPELL_ISPELLCHECKCLIENT_H
#define KSPELL_ISPELLCHECKCLIENT_H

#include "client_p.h"

#include <windows.h>
#include <spellcheck.h>

namespace Sonnet {
class SpellerPlugin;
}
using Sonnet::SpellerPlugin;

class ISpellCheckerClient : public Sonnet::Client
{
    Q_OBJECT
    Q_INTERFACES(Sonnet::Client)
    Q_PLUGIN_METADATA(IID "org.kde.Sonnet.ISpellCheckerClient")
public:
    explicit ISpellCheckerClient(QObject *parent = nullptr);
    ~ISpellCheckerClient() override;

    int reliability() const override
    {
        return 40;
    }

    SpellerPlugin *createSpeller(const QString &language) override;

    QStringList languages() const override;

    QString name() const override
    {
        return QStringLiteral("ISpellChecker");
    }

private:
    bool m_wasCOMInitialized = false;
    ISpellCheckerFactory* m_spellCheckerFactory = nullptr;
    QStringList m_languages;
};

#endif
