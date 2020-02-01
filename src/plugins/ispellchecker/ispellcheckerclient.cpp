/*
    SPDX-FileCopyrightText: 2019 Christoph Cullmann <cullmann@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ispellcheckerclient.h"
#include "ispellcheckerdict.h"
#include "ispellcheckerdebug.h"

using namespace Sonnet;

ISpellCheckerClient::ISpellCheckerClient(QObject *parent)
    : Client(parent)
{
    qCDebug(SONNET_ISPELLCHECKER) << " ISpellCheckerClient::ISpellCheckerClient";

    // init com if needed
    m_wasCOMInitialized = SUCCEEDED(CoInitializeEx(nullptr, COINIT_MULTITHREADED));

    // get factory
    if (SUCCEEDED(CoCreateInstance(__uuidof(SpellCheckerFactory), nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_spellCheckerFactory)))) {
        // if we have a factory, cache the language names
        IEnumString* enumLanguages = nullptr;
        if (SUCCEEDED(m_spellCheckerFactory->get_SupportedLanguages(&enumLanguages))) {
            HRESULT hr = S_OK;
            while (S_OK == hr) {
                LPOLESTR string = nullptr;
                hr = enumLanguages->Next(1, &string, nullptr);
                if (S_OK == hr) {
                    m_languages.push_back(QString::fromWCharArray(string));
                    CoTaskMemFree(string);
                }
            }
            enumLanguages->Release();
        }
    } else {
        m_spellCheckerFactory = nullptr;
    }
}

ISpellCheckerClient::~ISpellCheckerClient()
{
    // de-init com if needed
    if (m_wasCOMInitialized) {
        CoUninitialize();
    }

    // release factory
    if (m_spellCheckerFactory) {
        m_spellCheckerFactory->Release();
    }
}

SpellerPlugin *ISpellCheckerClient::createSpeller(const QString &language)
{
    // create requested spellchecker, might internally fail to create instance
    qCDebug(SONNET_ISPELLCHECKER) << " SpellerPlugin *ISpellCheckerClient::createSpeller(const QString &language) ;" << language;
    ISpellCheckerDict *ad = new ISpellCheckerDict(m_spellCheckerFactory, language);
    return ad;
}

QStringList ISpellCheckerClient::languages() const
{
    return m_languages;
}
