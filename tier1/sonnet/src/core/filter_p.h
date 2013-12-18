// -*- Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil; -*-
/*
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

#ifndef SONNET_FILTER_P_H
#define SONNET_FILTER_P_H

#include <QtCore/QTextBoundaryFinder>
#include <QtCore/QString>
#include <sonnet/sonnetcore_export.h>

namespace Sonnet
{
    class Settings;

    /**
     * Structure abstracts the word and its position in the
     * parent text.
     *
     * @author Zack Rusin <zack@kde.org>
     * @short struct represents word
     */
    struct SONNETCORE_EXPORT Word
    {
        Word() : start( 0 ), end( true )
            {}

        Word( const QString& w, int st, bool e = false )
            : word( w ), start( st ), end( e )
            {}
        Word( const Word& other )
            : word( other.word ), start( other.start ),
              end( other.end )
            {}

        QString word;
        int    start;
        bool    end;
    };

    /**
     * Filter is used to split text into words which
     * will be spell checked.
     *
     * @author Zack Rusin <zack@kde.org>
     * @short used to split text into words
     */
    class SONNETCORE_EXPORT Filter
    {
    public:
        static Filter *defaultFilter();
    public:
        Filter();
        virtual ~Filter();

        static Word end();

        /**
         * Sets the Settings object for this Filter
         */
        void setSettings(Settings*);

        /**
         * Returns currently used Settings object
         */
        Settings *settings() const;

        bool atEnd() const;

        void setBuffer( const QString& buffer );
        QString buffer() const;

        void restart();

        virtual Word nextWord() const;
        virtual Word wordAtPosition( unsigned int pos ) const;

        virtual void setCurrentPosition( int );
        virtual int currentPosition() const;
        virtual void replace( const Word& w, const QString& newWord );

        /**
         * Should return the sentence containing the current word
         */
        virtual QString context() const;
    protected:
        bool trySkipLinks() const;
        bool ignore( const QString& word ) const;
        bool shouldBeSkipped( bool wordWasUppercase, bool wordWasRunTogether,
                              const QString& foundWord ) const;

    protected:
        QString m_buffer;
        mutable QTextBoundaryFinder m_finder;

    private:
        class Private;
        Private* const d;
    };

}

#endif // SONNET_FILTER_P_H
