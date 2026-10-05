// SPDX-FileCopyrightText: 2026 Volker Krause
// SPDX-License-Identifier: LGPL-2.0-or-later

#include <Sonnet/GuessLanguage>

#include <QApplication>
#include <QPlainTextEdit>
#include <QTextStream>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QPlainTextEdit editor;
    Sonnet::GuessLanguage guesser;

    QObject::connect(&editor, &QPlainTextEdit::textChanged, &app, [&]() {
        QTextStream(stdout) << guesser.identify(editor.toPlainText()) << '\n';
    });

    editor.show();
    return app.exec();
}
