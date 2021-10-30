/*
 *  SPDX-FileCopyrightText: 2003 Ingo Kloecker <kloecker@kde.org>
 *  SPDX-FileCopyrightText: 2008 Tom Albers <tomalbers@kde.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#ifndef SONNET_DICTIONARYCOMBOBOX_H
#define SONNET_DICTIONARYCOMBOBOX_H

#include "sonnetui_export.h"

#include <QComboBox>

namespace Sonnet
{
class DictionaryComboBoxPrivate;
/**
 * @short A combo box for selecting the dictionary used for spell checking.
 * @author Ingo Kloecker <kloecker@kde.org>
 * @author Tom Albers <tomalbers@kde.nl>
 * @since 4.2
 **/

class SONNETUI_EXPORT DictionaryComboBox : public QComboBox
{
    Q_OBJECT
public:
    /**
     * Constructor
     */
    explicit DictionaryComboBox(QWidget *parent = nullptr);

    /**
     * Destructor
     */
    ~DictionaryComboBox() override;

    /**
     * Clears the widget and reloads the dictionaries from Sonnet.
     * Remember to set the dictionary you want selected after calling this function.
     */
    void reloadCombo();

    /**
     * Returns the current dictionary name, for example "German (Switzerland)"
     */
    QString currentDictionaryName() const;

    /**
     * Returns the current dictionary, for example "de_CH"
     */
    QString currentDictionary() const;

    /**
     * Sets the current dictionaryName to the given dictionaryName
     */
    void setCurrentByDictionaryName(const QString &dictionaryName);

    /**
     * Sets the current dictionary to the given dictionary
     * Return true if dictionary was found.
     * @since 5.40
     * TODO merge with previous method in kf6
     */
    bool assignByDictionnary(const QString &dictionary);

    /**
     * Sets the current dictionaryName to the given dictionaryName
     * Return true if dictionary was found.
     * @since 5.40
     * TODO merge with previous method in kf6
     */
    bool assignDictionnaryName(const QString &name);

    /**
     * Sets the current dictionary to the given dictionary.
     */
    void setCurrentByDictionary(const QString &dictionary);

Q_SIGNALS:
    /**
     * @em Emitted whenever the current dictionary changes. Either
     * by user intervention or on setCurrentByDictionaryName() or on
     * setCurrentByDictionary(). For example "de_CH".
     */
    void dictionaryChanged(const QString &dictionary);

    /**
     * @em Emitted whenever the current dictionary changes. Either
     * by user intervention or on setCurrentByDictionaryName() or on
     * setCurrentByDictionary(). For example "German (Switzerland)".
     */
    void dictionaryNameChanged(const QString &dictionaryName);

private:
    DictionaryComboBoxPrivate *const d;
    Q_PRIVATE_SLOT(d, void slotDictionaryChanged(int))
};
}

#endif
