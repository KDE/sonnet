/**
 * test_filter.cpp
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

#include "test_filter.h"
#include "filter_p.h"

#include <qtest.h>
#include <QTextBoundaryFinder>
#include <QDebug>

// QT5 TODO QTEST_GUILESS_MAIN(SonnetFilterTest)
QTEST_MAIN(SonnetFilterTest)

using namespace Sonnet;

struct Hit {
    Hit( const QString& w, int s ) : word( w ), start( s ) {}
    QString word;
    int start;
};

void SonnetFilterTest::testFilter()
{
    QString buffer( "This is     a sample thing. Please test me .     He's don't Le'Clerk." );
    QList<Hit> hits;
    hits.append( Hit( "This", 0 ) );
    hits.append( Hit( "is", 5 ) );
    hits.append( Hit( "a", 12 ) );
    hits.append( Hit( "sample", 14 ) );
    hits.append( Hit( "thing", 21 ) );
    hits.append( Hit( "Please", 28 ) );
    hits.append( Hit( "test", 35 ) );
    hits.append( Hit( "me", 40 ) );
    hits.append( Hit( "He's", 49 ) );
    hits.append( Hit( "don't", 54 ) );
    hits.append( Hit( "Le'Clerk", 60 ) );

    Filter filter;
    filter.setBuffer( buffer );

    Word w;
    int hitNumber = 0;
    while ( ! (w=filter.nextWord()).end ) {
        //qDebug()<< "Found word \""<< w.word << "\" which starts at position " << w.start;
        QCOMPARE( w.word, hits[hitNumber].word );
        QCOMPARE( w.start, hits[hitNumber].start );
        ++hitNumber;
    }
    QCOMPARE( hitNumber, hits.count() );

    // ? filter.setBuffer( buffer );
}


void SonnetFilterTest::testWordAt()
{
    QString buffer( "This is     a sample thing. Please test me .     He's don't Le'Clerk." );
    QList<Hit> hits;
    hits.append( Hit( "This", 0 ) );
    hits.append( Hit( "is", 5 ) );
    hits.append( Hit( "a", 12 ) );
    hits.append( Hit( "Please", 28 ) );
    hits.append( Hit( "me", 40 ) );

    Filter filter;
    filter.setBuffer( buffer );
    Word word;

    word = filter.wordAtPosition(3);
    QCOMPARE( word.word, hits[0].word );
    QCOMPARE( word.start, hits[0].start );

    word = filter.wordAtPosition(5);
    QCOMPARE( word.word, hits[1].word );
    QCOMPARE( word.start, hits[1].start );

    word = filter.wordAtPosition(12);
    QCOMPARE( word.word, hits[2].word );
    QCOMPARE( word.start, hits[2].start );

    word = filter.wordAtPosition(29);
    QCOMPARE( word.word, hits[3].word );
    QCOMPARE( word.start, hits[3].start );

    word = filter.wordAtPosition(42);
    QCOMPARE( word.word, hits[4].word );
    QCOMPARE( word.start, hits[4].start );

}

static QVector<ushort>
convertToUnicode(const QString &str)
{
    QVector<ushort> unicode;
    for (int i = 0; i < str.length(); ++i)
        unicode += str[i].unicode();
    return unicode;
}

void SonnetFilterTest::testIndic()
{
    QString buffer;
    QList<Hit> hits;
    hits.append( Hit( QString::fromUtf8("मराठी"), 0 ) );
    hits.append( Hit( QString::fromUtf8("भाषा"), 6 ) );
    hits.append( Hit( QString::fromUtf8("महाराष्ट्र"), 11 ) );
    hits.append( Hit( QString::fromUtf8("व"), 22 ) );
    hits.append( Hit( QString::fromUtf8("गोवा"), 24 ) );
    hits.append( Hit( QString::fromUtf8("राज्याची"), 29 ) );
    hits.append( Hit( QString::fromUtf8("राजभाषा"), 38 ) );
    hits.append( Hit( QString::fromUtf8("असून"), 46 ) );
    hits.append( Hit( QString::fromUtf8("सुमारे"), 51 ) );
    //hits.append( Hit( QString::fromUtf8("९"), 58 ) ); // This is the number 9, so we don't spell-check it
    hits.append( Hit( QString::fromUtf8("कोटी"), 60 ) );
    hits.append( Hit( QString::fromUtf8("लोकांची"), 65 ) );
    hits.append( Hit( QString::fromUtf8("मातृभाषा"), 73 ) );
    hits.append( Hit( QString::fromUtf8("आहे"), 82 ) );
    hits.append( Hit( QString::fromUtf8("मराठी"), 87 ) );
    hits.append( Hit( QString::fromUtf8("भाषा"), 93 ) );
    hits.append( Hit( QString::fromUtf8("कमीत"), 98 ) );
    hits.append( Hit( QString::fromUtf8("कमी"), 103 ) );
    //hits.append( Hit( QString::fromUtf8("१०००"), 107 ) ); // just a number
    hits.append( Hit( QString::fromUtf8("वर्षापासून"), 112 ) );
    hits.append( Hit( QString::fromUtf8("अस्तित्वात"), 123 ) );
    hits.append( Hit( QString::fromUtf8("आहे"), 134 ) );

    buffer = QString::fromUtf8("मराठी भाषा महाराष्ट्र व गोवा राज्याची राजभाषा असून सुमारे ९ कोटी लोकांची मातृभाषा आहे. मराठी भाषा कमीत कमी १००० वर्षापासून अस्तित्वात आहे.");

    Filter filter;
    filter.setBuffer( buffer );
    Word w;
    int hitNumber = 0;
    while ( ! (w=filter.nextWord()).end ) {
        QVector<ushort> unicode = convertToUnicode(w.word);
        //qDebug()<< "Found word \""<< unicode << "\" which starts at position " << w.start <<", len = "<<w.word.length();
        QCOMPARE( w.word, hits[hitNumber].word );
        QCOMPARE( w.start, hits[hitNumber].start );
        ++hitNumber;
    }
    QCOMPARE( hitNumber, hits.count() );
}

