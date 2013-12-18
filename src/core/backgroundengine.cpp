/**
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
#include "backgroundengine_p.h"

#include "spellerplugin_p.h"
#include "filter_p.h"

#include <QDebug>

#include <QtCore/QTimer>

using namespace Sonnet;

BackgroundEngine::BackgroundEngine(QObject *parent)
    : QObject(parent)
{
    m_filter = Filter::defaultFilter();
}

BackgroundEngine::~BackgroundEngine()
{
    delete m_filter;
}

void BackgroundEngine::setSpeller(const Speller &speller)
{
    m_dict = speller;
}

void BackgroundEngine::setText(const QString &text)
{
    m_filter->setBuffer(text);
}

QString BackgroundEngine::text() const
{
    return m_filter->buffer();
}

void BackgroundEngine::changeLanguage(const QString &lang)
{
    m_dict.setLanguage(lang);
}

QString BackgroundEngine::language() const
{
    return m_dict.language();
}

void BackgroundEngine::setFilter(Filter *filter)
{
    QString oldText = m_filter->buffer();
    m_filter = filter;
    m_filter->setBuffer(oldText);
}

void BackgroundEngine::start()
{
    QTimer::singleShot(0, this, SLOT(checkNext()));
}

void BackgroundEngine::stop()
{
}

void BackgroundEngine::continueChecking()
{
    QTimer::singleShot(0, this, SLOT(checkNext()));
}

void BackgroundEngine::checkNext()
{
    Word w = m_filter->nextWord();
    if (w.end) {
        emit done();
        return;
    }

    if (Q_UNLIKELY( m_dict.isMisspelled(w.word) )) {
        //qDebug()<<"found misspelling "<< w.word;
        emit misspelling(w.word, w.start);
        //wait for the handler. the parent will decide itself when to continue
    } else
        continueChecking();
}

bool BackgroundEngine::checkWord(const QString &word)
{
    return m_dict.isCorrect(word);
}

bool BackgroundEngine::addWord(const QString &word)
{
    return m_dict.addToPersonal(word);
}

QStringList BackgroundEngine::suggest(const QString &word)
{
    return m_dict.suggest(word);
}

#include "moc_backgroundengine_p.cpp"
