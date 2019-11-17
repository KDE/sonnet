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

#include "ispellcheckerdict.h"
#include "ispellcheckerdebug.h"

using namespace Sonnet;

ISpellCheckerDict::ISpellCheckerDict(ISpellCheckerFactory *spellCheckerFactory, const QString &language)
    : SpellerPlugin(language)
{
    // try to init checker
    if (!SUCCEEDED(spellCheckerFactory->CreateSpellChecker(language.toStdWString().c_str(), &m_spellChecker))) {
        m_spellChecker = nullptr;
    }
}

ISpellCheckerDict::~ISpellCheckerDict()
{
    // release com if needed
    if (m_spellChecker) {
        m_spellChecker->Release();
    }
}

bool ISpellCheckerDict::isCorrect(const QString &word) const
{
    // check if we are incorrect, we only need to check one enum entry for that, only empty enum means OK
    bool ok = true;
    IEnumSpellingError* enumSpellingError = nullptr;
    if (m_spellChecker && SUCCEEDED(m_spellChecker->Check(word.toStdWString().c_str(), &enumSpellingError))) {
        ISpellingError *spellingError = nullptr;
        if (S_OK == enumSpellingError->Next(&spellingError)) {
            ok = false;
            spellingError->Release();
        }
        enumSpellingError->Release();
    }
    return ok;
}

QStringList ISpellCheckerDict::suggest(const QString &word) const
{
    // query suggestions
    QStringList replacements;
    IEnumString* words = nullptr;
    if (m_spellChecker && SUCCEEDED(m_spellChecker->Suggest(word.toStdWString().c_str(), &words))) {
        HRESULT hr = S_OK;
        while (S_OK == hr) {
            LPOLESTR string = nullptr;
            hr = words->Next(1, &string, nullptr);
            if (S_OK == hr) {
                replacements.push_back(QString::fromWCharArray(string));
                CoTaskMemFree(string);
            }
        }
        words->Release();
    }
    return replacements;
}

bool ISpellCheckerDict::storeReplacement(const QString &bad, const QString &good)
{
    Q_UNUSED(bad);
    Q_UNUSED(good);
    qCDebug(SONNET_ISPELLCHECKER) << "ISpellCheckerDict::storeReplacement not implemented";
    return false;
}

bool ISpellCheckerDict::addToPersonal(const QString &word)
{
    // add word "permanently" to the dictionary
    return m_spellChecker && SUCCEEDED(m_spellChecker->Add(word.toStdWString().c_str()));
}

bool ISpellCheckerDict::addToSession(const QString &word)
{
    // ignore word for this session
    return m_spellChecker && SUCCEEDED(m_spellChecker->Ignore(word.toStdWString().c_str()));
}
