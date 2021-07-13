/*
 * SPDX-FileCopyrightText: 2007 Zack Rusin <zack@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */
#ifndef SONNET_SPELLER_H
#define SONNET_SPELLER_H

#include <QMap>
#include <QString>
#include <QStringList>

#include "sonnetcore_export.h"

namespace Sonnet
{
class SpellerPrivate;
/**
 * Spell checker object.
 *
 * @short class used for actual spell checking
 */
class SONNETCORE_EXPORT Speller
{
public:
    explicit Speller(const QString &lang = QString());
    ~Speller();

    Speller(const Speller &speller);
    Speller &operator=(const Speller &speller);

    /**
     * Returns true if the speller supports currently selected
     * language.
     */
    bool isValid() const;

    /**
     * Sets the language supported by this speller.
     */
    void setLanguage(const QString &lang);

    /**
     * Returns language supported by this speller.
     */
    QString language() const;

    /**
     * Checks the given word.
     * @return false if the word is misspelled. true otherwise
     */
    bool isCorrect(const QString &word) const;

    /**
     * Checks the given word.
     * @return true if the word is misspelled. false otherwise
     */
    bool isMisspelled(const QString &word) const;

    /**
     * Fetches suggestions for the word.
     *
     * @return list of all suggestions for the word
     */
    QStringList suggest(const QString &word) const;

    /**
     * Convenience method calling isCorrect() and suggest()
     * if the word isn't correct.
     */
    bool checkAndSuggest(const QString &word, QStringList &suggestions) const;

    /**
     * Stores user defined good replacement for the bad word.
     * @returns true on success
     */
    bool storeReplacement(const QString &bad, const QString &good);

    /**
     * Adds word to the list of of personal words.
     * @return true on success
     */
    bool addToPersonal(const QString &word);

    /**
     * Adds word to the words recognizable in the current session.
     * @return true on success
     */
    bool addToSession(const QString &word);

public: // Configuration API
    enum Attribute {
        CheckUppercase,
        SkipRunTogether,
        AutoDetectLanguage,
    };
    void save();
    void restore();

    /**
     * Returns names of all supported backends (e.g. ISpell, ASpell)
     */
    QStringList availableBackends() const;

    /**
     * Returns a list of supported languages.
     *
     * Note: use availableDictionaries
     */
    QStringList availableLanguages() const;

    /**
     * Returns a localized list of names of supported languages.
     *
     * Note: use availableDictionaries
     */
    QStringList availableLanguageNames() const;

    /**
     * Returns a map of all available dictionaies with language descriptions and
     * their codes. The key is the description, the code the value.
     */
    QMap<QString, QString> availableDictionaries() const;

    /**
     * Returns a map of user preferred dictionaies with language descriptions and
     * their codes. The key is the description, the code the value.
     * @since 5.54
     */
    QMap<QString, QString> preferredDictionaries() const;

    void setDefaultLanguage(const QString &lang);
    QString defaultLanguage() const;

    void setDefaultClient(const QString &client);
    QString defaultClient() const;

    void setAttribute(Attribute attr, bool b = true);
    bool testAttribute(Attribute attr) const;

private:
    SpellerPrivate *const d;
};
}
#endif
