/*
    SPDX-FileCopyrightText: 2019 Christoph Cullmann <cullmann@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
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
