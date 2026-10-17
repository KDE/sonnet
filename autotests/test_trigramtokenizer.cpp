/*
    SPDX-FileCopyrightText: 2026 Volker Krause <vkrause@kde.org>
    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "../src/core/trigramtokenizer_p.h"

#include <QDebug>
#include <QObject>
#include <QTest>

using namespace Qt::Literals;
using namespace Sonnet;

class TrigramTokenizerTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testEmpty_data()
    {
        QTest::addColumn<QString>("input");

        QTest::newRow("empty") << QString();
        QTest::newRow("space") << u" "_s;
        QTest::newRow("null") << u"\0"_s;
        QTest::newRow("bell") << u"\b"_s;
        QTest::newRow("space-folding") << u"   "_s;
        QTest::newRow("number") << u"123"_s;
        QTest::newRow("punct") << u"!?"_s;
        QTest::newRow("emoji") << u"🍫"_s;
    }
    void testEmpty()
    {
        QFETCH(QString, input);
        Sonnet::TrigramTokenizer t(input);
        QVERIFY(!t.hasNext());
    }

    void testTokenize_data()
    {
        QTest::addColumn<QString>("input");
        QTest::addColumn<QList<Sonnet::Trigram>>("trigrams");

        QTest::newRow("single-letter") << u"a"_s << QList<Trigram>({Trigram(u" a ")});
        QTest::newRow("case-folding") << u"A"_s << QList<Trigram>({Trigram(u" a ")});
        QTest::newRow("two-letters") << u"ab"_s << QList<Trigram>({Trigram(u" ab"), Trigram(u"ab ")});
        QTest::newRow("space-normalization") << u"a\nb"_s << QList<Trigram>({Trigram(u" a "), Trigram(u"a b"), Trigram(u" b ")});
        QTest::newRow("space-compression") << u"a\n \nb"_s << QList<Trigram>({Trigram(u" a "), Trigram(u"a b"), Trigram(u" b ")});
        QTest::newRow("non-print-filter") << u"a\b"_s << QList<Trigram>({Trigram(u" a ")});
        QTest::newRow("three-letters") << u"aBc"_s << QList<Trigram>({Trigram(u" ab"), Trigram(u"abc"), Trigram(u"bc ")});
        QTest::newRow("unicode") << u"ÄÖÜ"_s << QList<Trigram>({Trigram(u" äö"), Trigram(u"äöü"), Trigram(u"öü ")});
        QTest::newRow("leading-space-compression") << u"     abcd"_s << QList<Trigram>({Trigram(u" ab"), Trigram(u"abc"), Trigram(u"bcd"), Trigram(u"cd ")});
        QTest::newRow("trailing-space-compression") << u"abcd\n\n\n"_s << QList<Trigram>({Trigram(u" ab"), Trigram(u"abc"), Trigram(u"bcd"), Trigram(u"cd ")});
        QTest::newRow("word-number") << u"abcd 123"_s << QList<Trigram>({Trigram(u" ab"), Trigram(u"abc"), Trigram(u"bcd"), Trigram(u"cd ")});
        QTest::newRow("number-word") << u"123 abcd"_s << QList<Trigram>({Trigram(u" ab"), Trigram(u"abc"), Trigram(u"bcd"), Trigram(u"cd ")});
        QTest::newRow("question") << u"¿abcd?"_s << QList<Trigram>({Trigram(u" ab"), Trigram(u"abc"), Trigram(u"bcd"), Trigram(u"cd ")});
        QTest::newRow("emoji") << u"ab🍫cd"_s << QList<Trigram>({Trigram(u" ab"), Trigram(u"ab "), Trigram(u"b c"), Trigram(u" cd"), Trigram(u"cd ")});
    }

    void testTokenize()
    {
        QFETCH(QString, input);
        QFETCH(QList<Sonnet::Trigram>, trigrams);

        Sonnet::TrigramTokenizer tokenizer(input);
        QVERIFY(tokenizer.hasNext());
        QList<Sonnet::Trigram> ts;
        while (tokenizer.hasNext()) {
            ts.push_back(tokenizer.next());
        }
        QCOMPARE(ts, trigrams);
    }
};

QTEST_APPLESS_MAIN(TrigramTokenizerTest)

#include "test_trigramtokenizer.moc"
