/**
 * parsetrigrams.cpp
 *
 * Parse a set of trigram files into a QHash, and serialize to stdout.
 *
 * SPDX-FileCopyrightText: 2006 Jacob Rideout <kde@jacobrideout.net>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <QDataStream>
#include <QDir>
#include <QFile>
#include <QHash>
#include <QRegularExpression>
#include <QString>
#include <QTextStream>

int main(int argc, char **argv)
{
    if (argc < 2) {
        return 1;
    }

    QFile sout;
    sout.open(stdout, QIODevice::WriteOnly);
    QDataStream out(&sout);

    QString path = QLatin1String(argv[1]);
    QDir td(path);

    QHash<QString, QHash<QString, int>> models;

    const QRegularExpression rx(QStringLiteral("(?:.{3})\\s+(.*)"));
    for (const QString &fname : td.entryList(QDir::Files)) {
        QFile fin(td.filePath(fname));
        fin.open(QFile::ReadOnly | QFile::Text);
        QTextStream stream(&fin);
        stream.setCodec("UTF-8");
        while (!stream.atEnd()) {
            QString line = stream.readLine();
            const QRegularExpressionMatch match = rx.match(line);
            if (match.hasMatch()) {
                models[fname][line.left(3)] = match.capturedRef(1).toInt();
            }
        }
    }

    out << models;
}
