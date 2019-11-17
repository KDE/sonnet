/*  SPDX-License-Identifier: LGPL-2.0-or-later

    Copyright (C) 2019 Christoph Cullmann <cullmann@kde.org>

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
