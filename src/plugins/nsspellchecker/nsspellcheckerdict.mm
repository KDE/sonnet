/**
 * nsspellcheckerdict.mm
 *
 * Copyright (C)  2015  Nick Shaforostoff <shaforostoff@gmail.com>
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
#include "nsspellcheckerdict.h"
#include "nsspellcheckerdebug.h"

#import <AppKit/AppKit.h>

using namespace Sonnet;

NSSpellCheckerDict::NSSpellCheckerDict(const QString &lang)
    : SpellerPlugin(lang)
    , m_langCode(lang.toNSString())
{
}

NSSpellCheckerDict::~NSSpellCheckerDict()
{
}

bool NSSpellCheckerDict::isCorrect(const QString &word) const
{
    NSRange range = [[NSSpellChecker sharedSpellChecker] checkSpellingOfString:word.toNSString()
        startingAt:0 language:reinterpret_cast<NSString*>(m_langCode)
        wrap:NO inSpellDocumentWithTag:0 wordCount:nullptr];
    return range.length==0;
}

QStringList NSSpellCheckerDict::suggest(const QString &word) const
{
    NSString* correction = [[NSSpellChecker sharedSpellChecker] correctionForWordRange:NSMakeRange(0, word.length())
        inString:word.toNSString() language:reinterpret_cast<NSString*>(m_langCode) inSpellDocumentWithTag:0];
    return QStringList(QString::fromNSString(correction));
}

bool NSSpellCheckerDict::storeReplacement(const QString &bad,
                                    const QString &good)
{
    return false;
}

bool NSSpellCheckerDict::addToPersonal(const QString &word)
{
    return false;
}

bool NSSpellCheckerDict::addToSession(const QString &word)
{
    return false;
}
