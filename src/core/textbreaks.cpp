/*  This file is part of the KDE libraries
    SPDX-FileCopyrightText: 2006 Jacob R Rideout <kde@jacobrideout.net>
    SPDX-FileCopyrightText: 2006 Martin Sandsmark <martin.sandsmark@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include <QHash>
#include <QString>
#include <QTextBoundaryFinder>

#include "textbreaks_p.h"

namespace Sonnet
{
TextBreaks::TextBreaks(const QString &text)
{
    setText(text);
}

TextBreaks::~TextBreaks() = default;

QString TextBreaks::text() const
{
    return m_text;
}

void TextBreaks::setText(const QString &text)
{
    m_text = text;
}

TextBreaks::Positions TextBreaks::wordBreaks(const QString &text)
{
    Positions breaks;

    if (text.isEmpty()) {
        return breaks;
    }

    QTextBoundaryFinder boundaryFinder(QTextBoundaryFinder::Word, text);

    while (boundaryFinder.position() < text.length()) {
        if (!(boundaryFinder.boundaryReasons().testFlag(QTextBoundaryFinder::StartOfItem))) {
            if (boundaryFinder.toNextBoundary() == -1) {
                break;
            }
            continue;
        }

        Position pos;
        pos.start = boundaryFinder.position();
        int end = boundaryFinder.toNextBoundary();
        if (end == -1) {
            break;
        }
        pos.length = end - pos.start;
        if (pos.length < 1) {
            continue;
        }
        breaks.append(pos);

        if (boundaryFinder.toNextBoundary() == -1) {
            break;
        }
    }
    return breaks;
}

TextBreaks::Positions TextBreaks::sentenceBreaks(const QString &text)
{
    Positions breaks;

    if (text.isEmpty()) {
        return breaks;
    }

    QTextBoundaryFinder boundaryFinder(QTextBoundaryFinder::Sentence, text);

    while (boundaryFinder.position() < text.length()) {
        Position pos;
        pos.start = boundaryFinder.position();
        int end = boundaryFinder.toNextBoundary();
        if (end == -1) {
            break;
        }
        pos.length = end - pos.start;
        if (pos.length < 1) {
            continue;
        }
        breaks.append(pos);
    }
    return breaks;
}

TextBreaks::Positions TextBreaks::wordBreaks() const
{
    return wordBreaks(m_text);
}

TextBreaks::Positions TextBreaks::sentenceBreaks() const
{
    return sentenceBreaks(m_text);
}
}
