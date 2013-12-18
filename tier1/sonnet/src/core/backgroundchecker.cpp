/**
 * backgroundchecker.cpp
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
#include "backgroundchecker.h"

#include "loader_p.h"
#include "backgroundengine_p.h"
#include "filter_p.h"
#include "settings_p.h"

using namespace Sonnet;

class BackgroundChecker::Private
{
public:
    BackgroundEngine *engine;
    QString currentText;
};


BackgroundChecker::BackgroundChecker(QObject *parent)
    : QObject(parent),
      d(new Private)
{
    d->engine = new BackgroundEngine(this);
    connect(d->engine, SIGNAL(misspelling(QString,int)),
            SIGNAL(misspelling(QString,int)));
    connect(d->engine, SIGNAL(done()),
            SLOT(slotEngineDone()));
}

BackgroundChecker::BackgroundChecker(const Speller &speller, QObject *parent)
    : QObject(parent),
      d(new Private)
{
    d->engine = new BackgroundEngine(this);
    d->engine->setSpeller(speller);
    connect(d->engine, SIGNAL(misspelling(QString,int)),
            SIGNAL(misspelling(QString,int)));
    connect(d->engine, SIGNAL(done()),
            SLOT(slotEngineDone()));
}

BackgroundChecker::~BackgroundChecker()
{
    delete d;
}

void BackgroundChecker::setText(const QString &text)
{
    d->currentText = text;
    d->engine->setText(text);
    d->engine->start();
}

void BackgroundChecker::start()
{
    d->currentText = fetchMoreText();
    // ## what if d->currentText.isEmpty()?
    //qDebug()<<"Sonnet BackgroundChecker: starting with : \"" << d->currentText << "\"";
    d->engine->setText(d->currentText);
    d->engine->start();
}

void BackgroundChecker::stop()
{
    d->engine->stop();
}

QString BackgroundChecker::fetchMoreText()
{
    return QString();
}

void BackgroundChecker::finishedCurrentFeed()
{
}

void BackgroundChecker::setSpeller(const Speller &speller)
{
    d->engine->setSpeller(speller);
}

Speller BackgroundChecker::speller() const
{
    return d->engine->speller();
}

bool BackgroundChecker::checkWord(const QString &word)
{
    return d->engine->checkWord( word );
}

bool BackgroundChecker::addWordToPersonal(const QString &word)
{
    return d->engine->addWord(word);
}

QStringList BackgroundChecker::suggest(const QString &word) const
{
    return d->engine->suggest(word);
}

void BackgroundChecker::changeLanguage(const QString &lang)
{
    d->engine->changeLanguage(lang);
}

void BackgroundChecker::continueChecking()
{
    d->engine->continueChecking();
}

void BackgroundChecker::slotEngineDone()
{
    finishedCurrentFeed();
    d->currentText = fetchMoreText();

    if ( d->currentText.isNull() ) {
        emit done();
    } else {
        d->engine->setText( d->currentText );
        d->engine->start();
    }
}

QString BackgroundChecker::text() const
{
    return d->engine->filter()->buffer();
}


QString BackgroundChecker::currentContext() const
{
    return d->engine->filter()->context();
}

void Sonnet::BackgroundChecker::replace(int start, const QString &oldText,
                                        const QString &newText)
{
    Word w(oldText, start);
    d->engine->filter()->replace(w, newText);
}

