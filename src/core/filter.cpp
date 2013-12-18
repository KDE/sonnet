// -*- Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil; -*-
/**
 * filter.cpp
 *
 * Copyright (C)  2004  Zack Rusin <zack@kde.org>
 * Copyright (C)  2010  Michel Ludwig <michel.ludwig@kdemail.net>
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

#include "filter_p.h"

#include "settings_p.h"

#include <QDebug>


namespace Sonnet
{

static Word endWord;

class Filter::Private
{
public:
    // The reason it's not in the class directly is that
    // I'm not 100% sure that having the settings() here is
    // the way i want to be doing this.
    Settings *settings;
};

Filter* Filter::defaultFilter()
{
    return new Filter();
}

Word Filter::end()
{
    return endWord;
}

Filter::Filter()
    : d(new Private)
{
    d->settings = 0;
}

Filter::~Filter()
{
    delete d;
}

void Filter::setSettings( Settings *conf )
{
    d->settings = conf;
}

Settings *Filter::settings() const
{
    return d->settings;
}

void Filter::restart()
{
    m_finder.toStart();
}

void Filter::setBuffer( const QString& buffer )
{
    m_buffer = buffer;
    m_finder = QTextBoundaryFinder(QTextBoundaryFinder::Word, m_buffer);
}

QString Filter::buffer() const
{
    return m_buffer;
}

bool Filter::atEnd() const
{
    return m_finder.position() >= m_buffer.length() || m_finder.position() < 0;
}

// we don't want to spell check empty words, or single-char words of the form
// '<', '=', etc.
static bool
isValidWord(const QString &str)
{
    if(str.isEmpty() || (str.length() == 1 && !str[0].isLetter())) {
      return false;
    }
    const int length = str.length();
    for(int i = 0; i < length; ++i) {
      if(!str[i].isNumber()) {
        return true;
      }
    }
    // 'str' only contains numbers
    return false;
}

static bool
finderNextWord(QTextBoundaryFinder &finder, QString &word, int &bufferStart)
{
    QTextBoundaryFinder::BoundaryReasons boundary = finder.boundaryReasons();
    int start = finder.position(), end = finder.position();
    bool inWord = (boundary & QTextBoundaryFinder::StartOfItem) != 0;
    while (finder.toNextBoundary() > 0) {
        boundary = finder.boundaryReasons();
        if ((boundary & QTextBoundaryFinder::EndOfItem) && inWord) {
            end = finder.position();
            QString str = finder.string().mid(start, end - start);
            if (isValidWord(str)) {
                word = str;
                bufferStart = start;
#if 0
                qDebug().nospace() << "Word at " << start << " word="
                         <<  str << ", len=" << str.length();
#endif
                return true;
            }
            inWord = false;
        }
        if ((boundary & QTextBoundaryFinder::StartOfItem)) {
            start = finder.position();
            inWord = true;
        }
    }
    return false;
}

static bool finderWordAt(QTextBoundaryFinder &finder,
                         int at,
                         QString &word, int &bufferStart)
{
    int oldPosition = finder.position();

    finder.setPosition(at);
    if (!finder.isAtBoundary() || (finder.boundaryReasons() & QTextBoundaryFinder::EndOfItem)) {
        if (finder.toPreviousBoundary() <= 0) {
            /* QTextBoundaryIterator doesn't consider start of the string
             * a boundary so we need to rewind to the beginning to catch
             * the first word */
            if (at > 0 && finder.string().length() > 0) {
                finder.toStart();
            } else
                return false;
        }
    }
    bool ret = finderNextWord(finder, word, bufferStart);
    finder.setPosition(oldPosition);
    return ret;
}

Word Filter::nextWord() const
{
    QString foundWord;
    int start;
    bool allUppercase = false;
    bool runTogether = false;

    if (!finderNextWord(m_finder, foundWord, start))
        return Filter::end();

    allUppercase = ( foundWord == foundWord.toUpper() );

    //TODO implement runtogether correctly.
    //We must ask to sonnet plugins to do it and not directly here.

    if ( shouldBeSkipped( allUppercase, runTogether, foundWord ) )
        return nextWord();
    return Word( foundWord, start );
}

Word Filter::wordAtPosition( unsigned int pos ) const
{
    QString foundWord;
    int start;
    if (!finderWordAt(m_finder, pos, foundWord, start))
        return Filter::end();
    return Word( foundWord, start );
}


void Filter::setCurrentPosition( int i )
{
    QString word;
    int pos;

    //to make sure we're at an reasonable word boundary
    if (!finderWordAt(m_finder, i, word, pos)) {
        return;
    }
    m_finder.setPosition(pos);
}

int Filter::currentPosition() const
{
    return m_finder.position();
}

void Filter::replace( const Word& w, const QString& newWord)
{
    int oldLen = w.word.length();

    //start spell checkin from the just correct word
    m_buffer = m_buffer.replace( w.start, oldLen, newWord );
    m_finder = QTextBoundaryFinder(QTextBoundaryFinder::Word,
                                     m_buffer);
    m_finder.setPosition(w.start);
}

QString Filter::context() const
{
    int len = 60;
    //we don't want the expression underneath casted to an unsigned int
    //which would cause it to always evaluate to false
    int signedPosition = m_finder.position();
    bool begin = (signedPosition - len/2)<=0;


    QString buffer = m_buffer;
    Word word = wordAtPosition( m_finder.position() );
    buffer = buffer.replace( word.start, word.word.length(),
                             QString::fromLatin1( "<b>%1</b>" ).arg( word.word ) );

    QString context;
    if ( begin )
        context = QString::fromLatin1("%1...")
                  .arg( buffer.mid(  0, len ) );
    else
        context = QString::fromLatin1("...%1...")
                  .arg( buffer.mid(  m_finder.position() - 20, len ) );

    context.replace( QLatin1Char('\n'), QLatin1Char(' ') );

    return context;
}

bool Filter::trySkipLinks() const
{
    QChar currentChar;
    int currentPosition = m_finder.position();

    if (currentPosition < 0 || currentPosition >= m_buffer.length())
        return false;
    currentChar = m_buffer.at( currentPosition );

    int length = m_buffer.length();
    //URL - if so skip
    if ( currentChar == QLatin1Char(':')
         && (currentPosition+1 < length)
         && (m_buffer.at( ++currentPosition ) == QLatin1Char('/') || ( currentPosition + 1 ) >= length ) ) {
        //in both cases url is considered finished at the first whitespace occurrence
        //TODO hey, "http://en.wikipedia.org/wiki/Main Page" --Nick Shaforostoff
        while ( !m_buffer.at( currentPosition++ ).isSpace() && currentPosition < length )
            ;
        m_finder.setPosition(currentPosition);
        return true;
    }

    //Email - if so skip
    if ( currentChar == QLatin1Char('@')) {
        while ( ++currentPosition < length && !m_buffer.at( currentPosition ).isSpace() )
            ;
        m_finder.setPosition(currentPosition);
        return true;
    }

    return false;
}

bool Filter::ignore( const QString& word ) const
{
    return d->settings && d->settings->ignore( word );
}

bool Filter::shouldBeSkipped( bool wordWasUppercase, bool wordWasRunTogether,
                             const QString& foundWord ) const
{
    bool checkUpper = ( d->settings ) ?
                      d->settings->checkUppercase () : true;

    bool skipRunTogether = ( d->settings ) ?
                           d->settings->skipRunTogether() : true;

    if ( trySkipLinks() )
        return true;

    if ( wordWasUppercase && !checkUpper )
        return true;

    if ( wordWasRunTogether && skipRunTogether )
        return true;

    return ignore( foundWord );
}

}
