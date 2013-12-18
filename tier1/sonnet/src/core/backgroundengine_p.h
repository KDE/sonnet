/*
 * backgroundengine.h
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
#ifndef SONNET_BACKGROUNDENGINE_P_H
#define SONNET_BACKGROUNDENGINE_P_H

#include "speller.h"
#include "loader_p.h"

#include <QtCore/QObject>
#include <QtCore/QStringList>

namespace Sonnet
{
    class Filter;
    class Loader;
    class SpellerPlugin;
    class BackgroundEngine : public QObject
    {
        Q_OBJECT
    public:
        explicit BackgroundEngine(QObject *parent);
        ~BackgroundEngine();

        void setSpeller(const Speller &speller);
        Speller speller() const { return m_dict; }

        void setText(const QString &);
        QString text() const;

        void changeLanguage(const QString &);
        QString language() const;

        void setFilter(Filter *filter);
        Filter *filter() const { return m_filter; }

        void start();
        void continueChecking();
        void stop();

        bool        checkWord(const QString &word);
        QStringList suggest(const QString &word);
        bool        addWord(const QString &word);
    Q_SIGNALS:

        /**
         * Emitted when a misspelling is found.
         */
        void misspelling(const QString&, int);

        /**
         * Emitted when all words have been checked.
         */
        void done();

    protected Q_SLOTS:
        void checkNext();
    private:
        Filter            *m_filter;
        Speller            m_dict;
    };
}

#endif // SONNET_BACKGROUNDENGINE_P_H
