/*
    SPDX-FileCopyrightText: 2026 Volker Krause <vkrause@kde.org>
    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SONNET_TRIGRAM_P_H
#define SONNET_TRIGRAM_P_H

#include <QChar>
#include <QStringView>

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

    constexpr explicit Trigram(QChar t1, QChar t2, QChar t3)
    {
        trigram[0] = t1;
        trigram[1] = t2;
        trigram[2] = t3;
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
