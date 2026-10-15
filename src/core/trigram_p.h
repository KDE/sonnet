/*
    SPDX-FileCopyrightText: 2026 Volker Krause <vkrause@kde.org>
    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SONNET_TRIGRAM_P_H
#define SONNET_TRIGRAM_P_H

#include <QChar>

namespace Sonnet
{

/** A trigram for language detection. */
class Trigram
{
public:
    explicit Trigram(QStringView s)
    {
        Q_ASSERT(s.size() == 3);
        trigram[0] = s[0].toCaseFolded();
        trigram[1] = s[1].toCaseFolded();
        trigram[2] = s[2].toCaseFolded();
    }

    /** Trigram contains to consecutive whitespaces. */
    [[nodiscard]] bool hasTwoConsecutiveSpaces() const
    {
        return trigram[1].isSpace() && (trigram[0].isSpace() || trigram[2].isSpace());
    }

    [[nodiscard]] std::strong_ordering operator<=>(const Trigram &other) const
    {
        return QStringView(trigram) <=> QStringView(other.trigram);
    }
    [[nodiscard]] bool operator==(const Trigram &other) const
    {
        return QStringView(trigram) == QStringView(other.trigram);
    }

    constexpr operator QStringView() const
    {
        return QStringView(trigram);
    }

private:
    QChar trigram[3];
};
}

#endif
