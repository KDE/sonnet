// krazy:excludeall=spelling
/**
 * test.cpp
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
#include "speller.h"

#include <QCoreApplication>
#include <qdebug.h>
#include <QtCore/QDate>

using namespace Sonnet;

int main( int argc, char** argv )
{
    QCoreApplication app(argc,argv);

    Speller dict("en_US");

    qDebug()<< "Clients are "   << dict.availableBackends();
    qDebug()<< "Languages are " << dict.availableLanguages();

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

    return 0;
}
