// krazy:excludeall=spelling
/**
 *
 * Copyright 2007  Zack Rusin <zack@kde.org>
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

#include "test_core.h"
#include "speller.h"
#include "globals.h"

#include <qtest.h>
#include <QDebug>

// QT5 TODO QTEST_GUILESS_MAIN(SonnetCoreTest)
QTEST_MAIN(SonnetCoreTest)

using namespace Sonnet;

void SonnetCoreTest::testCore()
{
    Speller dict("en_US");

    qDebug()<< "Clients are "   << dict.availableBackends();
    qDebug()<< "Languages are " << dict.availableLanguages();
    qDebug()<< "Language names are " << dict.availableLanguageNames();
    qDebug()<< "Language dicts " << dict.availableDictionaries();

    QStringList words;

    words << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted"
          << "hello" << "helo" << "enviroment" << "guvernment" << "farted";

    QTime mtime;
    mtime.start();
    for (QStringList::Iterator itr = words.begin();
         itr != words.end(); ++itr) {
        if (!dict.isCorrect(*itr)) {
            //qDebug()<<"Word " << *itr <<" is misspelled";
            QStringList sug = dict.suggest(*itr);
            //qDebug()<<"Suggestions : "<<sug;
        }
    }
    //mtime.stop();
    qDebug()<<"Elapsed time is "<<mtime.elapsed();

    qDebug()<<"Detecting language ...";
    QString sentence = QString::fromLatin1("QClipboard features some convenience functions to access common data types: setText() allows the exchange of Unicode text and setPixmap() and setImage() allows the exchange of QPixmaps and QImages between applications.");
    qDebug()<<"\tlang is "<<Sonnet::detectLanguage(sentence);
}

void SonnetCoreTest::testCore2()
{
    Speller dict("de_DE");
    if (!dict.availableDictionaries().contains("German")) {
        QSKIP("This test requires a german spelling dictionary");
        return;
    }
    qDebug()<< "Clients are "   << dict.availableBackends();
    qDebug()<< "Languages are " << dict.availableLanguages();
    qDebug()<< "Language names are " << dict.availableLanguageNames();
    qDebug()<< "Language dicts " << dict.availableDictionaries();

    QStringList words;

    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";
    words << "Hallo" << "halo" << "Umgebunp" << "Regirung" << "bet";

    QTime mtime;
    mtime.start();
    for (QStringList::Iterator itr = words.begin();
         itr != words.end(); ++itr) {
        if (!dict.isCorrect(*itr)) {
            //qDebug()<<"Word " << *itr <<" is misspelled";
            QStringList sug = dict.suggest(*itr);
            //qDebug()<<"Suggestions : "<<sug;
        }
    }
    //mtime.stop();
    qDebug()<<"Elapsed time is "<<mtime.elapsed();

    qDebug()<<"Detecting language ...";
    QString sentence = QString::fromLatin1("Die K Desktop Environment (KDE; auf Deutsch K-Arbeitsumgebung; früher: Kool Desktop Environment) ist eine frei verfügbare Arbeitsumgebung, das heißt eine grafische Benutzeroberfläche mit vielen Zusatzprogrammen für den täglichen Gebrauch.");
    qDebug()<<"\tlang is "<<Sonnet::detectLanguage(sentence);
}

