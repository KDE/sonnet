/*
 * highlighter.h
 *
 * Copyright (C)  2004  Zack Rusin <zack@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */
#ifndef SONNET_HIGHLIGHTER_H
#define SONNET_HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QtCore/QStringList>
#include <sonnet/sonnetui_export.h>

class QTextEdit;

namespace Sonnet
{
    /// The Sonnet Highlighter
    class SONNETUI_EXPORT Highlighter : public QSyntaxHighlighter
    {
        Q_OBJECT
    public:
        explicit Highlighter(QTextEdit *textEdit,
                             const QString &configFile = QString(),
                             const QColor &col=QColor());
        ~Highlighter();

        bool spellCheckerFound() const;

        QString currentLanguage() const;

        static QStringList personalWords();

        /**
         * @short Enable/Disable spell checking.
         *
         * If @p active is true then spell checking is enabled; otherwise it
         * is disabled. Note that you have to disable automatic (de)activation
         * with @ref setAutomatic() before you change the state of spell
         * checking if you want to persistently enable/disable spell
         * checking.
         *
         * @param active if true, then spell checking is enabled
         *
         * @see isActive(), setAutomatic()
         */
        void setActive(bool active);

        /**
         * Returns the state of spell checking.
         *
         * @return true if spell checking is active
         *
         * @see setActive()
         */
        bool isActive() const;

        bool automatic() const;

        void setAutomatic(bool automatic);

        /**
         * Adds the given word permanently to the dictionary. It will never
         * be marked as misspelled again, even after restarting the application.
         *
         * @param word the word which will be added to the dictionary
         * @since 4.1
         */
        void addWordToDictionary(const QString &word);

        /**
         * Ignores the given word. This word will not be marked misspelled for
         * this session. It will again be marked as misspelled when creating
         * new highlighters.
         *
         * @param word the word which will be ignored
         * @since 4.1
         */
        void ignoreWord(const QString &word);

        /**
         * Returns a list of suggested replacements for the given misspelled word.
         * If the word is not misspelled, the list will be empty.
         *
         * @param word the misspelled word
         * @param max at most this many suggestions will be returned. If this is
         *            -1, as many suggestions as the spell backend supports will
         *            be returned.
         * @return a list of suggested replacements for the word
         * @since 4.1
         */
        QStringList suggestionsForWord(const QString &word, int max = 10 );

        /**
         * Checks if a given word is marked as misspelled by the highlighter.
         *
         * @param word the word to be checked
         * @return true if the given word is misspelled.
         * @since 4.1
         */
        bool isWordMisspelled(const QString &word);

        /**
         * Sets the color in which the highlighter underlines misspelled words.
         * @since 4.2
         */
        void setMisspelledColor(const QColor &color);

        /**
         * Return true if checker is enabled by default
         * @since 4.5
         */
        bool checkerEnabledByDefault() const;

    Q_SIGNALS:

        /**
         * Emitted when as-you-type spell checking is enabled or disabled.
         *
         * @param description is a i18n description of the new state,
         *        with an optional reason
         */
        void activeChanged(const QString &description);

        /**
         *
         * @param originalWord missspelled word
         *
         * @param suggestions list of word which can replace missspelled word
         *
         * @deprecated use isWordMisspelled() and suggestionsForWord() instead.
         */
        QT_MOC_COMPAT void newSuggestions(const QString &originalWord, const QStringList &suggestions);

    protected:

        virtual void highlightBlock(const QString &text);
        virtual void setMisspelled(int start, int count);
        virtual void unsetMisspelled(int start,  int count);

        bool eventFilter(QObject *o, QEvent *e);
        bool intraWordEditing() const;
        void setIntraWordEditing(bool editing);

    public Q_SLOTS:
        void setCurrentLanguage(const QString &lang);
        void slotAutoDetection();
        void slotRehighlight();

    private:
        virtual void connectNotify(const QMetaMethod& signal);
        virtual void disconnectNotify(const QMetaMethod& signal);
        class Private;
        Private *const d;
        Q_DISABLE_COPY( Highlighter )
    };

}

#endif
