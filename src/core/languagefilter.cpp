/*  This file is part of the KDE libraries
 
    Copyright (c) 2009 Jakub Stachowski <qbast@go2.pl>
 
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.
 
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.
 
    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include <QtCore/QString>
#include <QtCore/QDebug>

#include "languagefilter_p.h"
#include "guesslanguage.h"
#include "speller.h"
#include "globals.h"

namespace Sonnet
{

#define MIN_RELIABILITY 0.1
#define MAX_ITEMS 5

class LanguageFilterPrivate
{
public:
    LanguageFilterPrivate(AbstractTokenizer* s) : source(s) { gl.setLimits(MAX_ITEMS, MIN_RELIABILITY); }
    ~LanguageFilterPrivate() { delete source; }
    
    QString doIdentify(const QString& text, const QStringList& fallbacks) const;
    QString mainLanguage() const;
    
    AbstractTokenizer* source;
    QStringRef lastToken;
    
    mutable QString lastLanguage;
    mutable QString cachedMainLanguage;
    QString prevLanguage;
    
    GuessLanguage gl;
    Speller sp;
};

QString LanguageFilterPrivate::doIdentify(const QString& text, const QStringList& fallbacks) const
{
    QStringList candidates=gl.identify(text);
    
    qDebug() << "Guess for " << text << " got "  << candidates;
    // guesser was sure enough
    if (candidates.size() == 1) return candidates.front();

    // guesser was unable to even narrow it down to 3 candidates. In this case try fallbacks too 
    if (candidates.size()==MAX_ITEMS || candidates.isEmpty()) candidates += fallbacks;
    candidates.removeDuplicates();

    QStringList availableDictionaries=Speller().availableLanguages();
    QStringList toDictDetect;

    // make sure that dictionary-based detection only gets languages with installed dictionaries
    Q_FOREACH(const QString& s, candidates) if (availableDictionaries.contains(s)) toDictDetect += s;

    QString ret;

    // only one of candidates has dictionary - use it
    if (toDictDetect.size()==1) ret=toDictDetect.front();
    else if (!toDictDetect.isEmpty()) ret=detectLanguage(text, toDictDetect);

    // dictionary-based detection did not work, return best guess from previous step
    if (ret.isEmpty() && !candidates.isEmpty()) ret=candidates.front();

    qDebug() << "Dict got " << ret << " using " << candidates;
    return ret;
}

QString LanguageFilterPrivate::mainLanguage() const
{
    if (cachedMainLanguage.isNull())  cachedMainLanguage=doIdentify(source->buffer(), QStringList(Speller().defaultLanguage()));
    return cachedMainLanguage;
}


/* -----------------------------------------------------------------*/

LanguageFilter::LanguageFilter(AbstractTokenizer* source) : d(new LanguageFilterPrivate(source))
{
    d->prevLanguage=Speller().defaultLanguage();
}

bool LanguageFilter::hasNext() const
{
    return d->source->hasNext();
}

void LanguageFilter::setBuffer( const QString& buffer )
{
    d->cachedMainLanguage=QString();
    d->source->setBuffer(buffer);
}

QStringRef LanguageFilter::next() 
{
    d->lastToken=d->source->next();
    d->prevLanguage=d->lastLanguage;
    d->lastLanguage=QString();
    return d->lastToken;
}

QString LanguageFilter::language() const
{
    if (d->lastLanguage.isNull()) 
        d->lastLanguage=d->doIdentify(d->lastToken.toString(), QStringList() << d->mainLanguage() << d->prevLanguage);
    return d->lastLanguage;
}

bool LanguageFilter::isSpellcheckable() const {
    const QString& lastlang=language();
    if (lastlang.isEmpty()) return false;
    
    //FIXME: do something a little more smart here
    Q_FOREACH(const QString& lang, d->sp.availableLanguages()) 
        if (lang.startsWith(lastlang)) return true;
    return false;
}

QString LanguageFilter::buffer() const
{
    return d->source->buffer();
}
void LanguageFilter::replace(int position, int len, const QString& newWord)
{
    d->source->replace(position,len,newWord);
    //FIXME: fix lastToken
    // uncache language for current token - it may have changed
    d->lastLanguage=QString();
}



}
