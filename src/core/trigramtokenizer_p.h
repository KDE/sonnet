/*
    SPDX-FileCopyrightText: 2026 Volker Krause <vkrause@kde.org>
    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SONNET_TRIGRAMTOKENIZER_P_H
#define SONNET_TRIGRAMTOKENIZER_P_H

#include "trigram_p.h"

#include <QDebug>
#include <QStringView>

#include <cstdint>

namespace Sonnet
{

/** Splits the given input string into trigrams, applying space normalization/compaction
 *  and case folding, as well as space-padding boundaries.
 */
class TrigramTokenizer
{
public:
    explicit TrigramTokenizer(QStringView s)
        : m_string(s.trimmed())
    {
        findNext(1);
        findNext(2);
    }

    [[nodiscard]] bool hasNext() const
    {
        return m_indexes[(m_current + 1) % 3] != -1;
    }

    [[nodiscard]] Trigram next()
    {
        Trigram t(normalizedChar(m_indexes[m_current]), normalizedChar(m_indexes[(m_current + 1) % 3]), normalizedChar(m_indexes[(m_current + 2) % 3]));
        m_current = (m_current + 1) % 3;
        findNext((m_current + 2) % 3);
        return t;
    }

private:
    void findNext(uint8_t idx)
    {
        const qsizetype prevPos = m_indexes[(idx + 2) % 3];
        const auto prevSpace = prevPos == -1 ? true : m_string[prevPos].isSpace();
        for (auto nextPos = prevPos + 1; nextPos < m_string.size(); ++nextPos) {
            const auto next = m_string[nextPos];
            if (prevSpace && next.isSpace()) {
                continue;
            }
            if (!next.isPrint() && !next.isSpace()) {
                continue;
            }
            m_indexes[idx] = nextPos;
            return;
        }
        m_indexes[idx] = -1;
    }

    [[nodiscard]] QChar normalizedChar(qsizetype idx) const
    {
        if (idx < 0) {
            return QLatin1Char(' ');
        }
        const auto c = m_string[idx];
        return c.isSpace() ? QLatin1Char(' ') : c.toCaseFolded();
    }

    QStringView m_string;
    // ringbuffer of indexes to the next three characters of the trigram
    // this avoids reading through the string more than once for merging spaces
    qsizetype m_indexes[3] = {-1, -1, -1};
    // current start index in the above ringbuffer (mod 3)
    uint8_t m_current = 0;
};
}

#endif
