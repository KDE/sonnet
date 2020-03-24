/**
 * Copyright (C)  2019 Waqar Ahmed <waqar.17a@gmail.com>
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

#include <QObject>
#include <qtest.h>
#include "guesslanguage.h"

class SonnetAutoDetectTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
   
    void autodetect_data();
    void autodetect();
};

void SonnetAutoDetectTest::autodetect_data()
{
    QTest::addColumn<QString>("sentence");
    QTest::addColumn<QString>("correct_lang");
    QTest::addColumn<QStringList>("suggested_langs");
    
    /* These tests will fail if you dont have the respective dictionary installed.
     * They will also fail if the dictionary file name is different from 'correct_lang'
     */
    QTest::newRow("English") << QStringLiteral("This is an English sentence.") << QStringLiteral("en_US-large") << QStringList{QLatin1String("en_US-large"), QLatin1String("de_DE"), QLatin1String("ur_PK")};
    
    QTest::newRow("German") << QStringLiteral("Dies ist ein deutscher Satz.") << QStringLiteral("de_DE_frami") << QStringList{QLatin1String("hi_IN"), QLatin1String("pl_PL"), QLatin1String("de_DE_frami")};
    
    QTest::newRow("Malayam") << QStringLiteral("ഇന്ത്യയുടെ തെക്കു ഭാഗത്തു സ്ഥിതി ചെയ്യുന്ന ഒരു സംസ്ഥാനമാണ് കേരളം.") << QStringLiteral("ml_IN") << QStringList{QLatin1String("ml_IN"), QLatin1String("ur_PK"), QLatin1String("en_US-large")};
}

void SonnetAutoDetectTest::autodetect()
{
    QFETCH(QString, sentence);
    QFETCH(QString, correct_lang);
    QFETCH(QStringList, suggested_langs);
 
    Sonnet::GuessLanguage gl;
    QString actual_lang = gl.identify(sentence, suggested_langs);
    
    QCOMPARE(actual_lang, correct_lang);
}

QTEST_GUILESS_MAIN(SonnetAutoDetectTest)

#include "test_autodetect.moc"
