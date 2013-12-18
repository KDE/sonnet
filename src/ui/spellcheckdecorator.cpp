/*
 * spellcheckdecorator.h
 *
 * Copyright (C)  2013  Aurélien Gâteau <agateau@kde.org>
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
#include <spellcheckdecorator.h>

// Local
#include <highlighter.h>

// Qt
#include <QContextMenuEvent>
#include <QMenu>
#include <QTextEdit>

namespace Sonnet
{

class SpellCheckDecorator::Private
{
public:
    Private(SpellCheckDecorator *installer, QTextEdit *textEdit)
    : q(installer)
    , m_textEdit(textEdit)
    , m_highlighter(0)
    {
        // Catch pressing the "menu" key
        m_textEdit->installEventFilter(q);
        // Catch right-click
        m_textEdit->viewport()->installEventFilter(q);
    }

    bool onContextMenuEvent(QContextMenuEvent *event);
    void execSuggestionMenu(const QPoint &pos, const QString &word, const QTextCursor &cursor);
    void createDefaultHighlighter();

    SpellCheckDecorator *q;
    QTextEdit *m_textEdit;
    Highlighter *m_highlighter;
};

bool SpellCheckDecorator::Private::onContextMenuEvent(QContextMenuEvent *event)
{
    if (!m_highlighter) {
        createDefaultHighlighter();
    }

    // Obtain the cursor at the mouse position and the current cursor
    QTextCursor cursorAtMouse = m_textEdit->cursorForPosition(event->pos());
    const int mousePos = cursorAtMouse.position();
    QTextCursor cursor = m_textEdit->textCursor();

    // Check if the user clicked a selected word
    const bool selectedWordClicked = cursor.hasSelection() &&
                               mousePos >= cursor.selectionStart() &&
                               mousePos <= cursor.selectionEnd();

    // Get the word under the (mouse-)cursor and see if it is misspelled.
    // Don't include apostrophes at the start/end of the word in the selection.
    QTextCursor wordSelectCursor(cursorAtMouse);
    wordSelectCursor.clearSelection();
    wordSelectCursor.select(QTextCursor::WordUnderCursor);
    QString selectedWord = wordSelectCursor.selectedText();

    bool isMouseCursorInsideWord = true;
    if ((mousePos < wordSelectCursor.selectionStart() ||
            mousePos >= wordSelectCursor.selectionEnd())
                                        && (selectedWord.length() > 1)) {
         isMouseCursorInsideWord = false;
    }

    // Clear the selection again, we re-select it below (without the apostrophes).
    wordSelectCursor.setPosition(wordSelectCursor.position()-selectedWord.size());
    if (selectedWord.startsWith('\'') || selectedWord.startsWith('\"')) {
        selectedWord = selectedWord.right(selectedWord.size() - 1);
        wordSelectCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);
    }
    if (selectedWord.endsWith('\'') || selectedWord.endsWith('\"'))
        selectedWord.chop(1);

    wordSelectCursor.movePosition(QTextCursor::NextCharacter,
                                  QTextCursor::KeepAnchor, selectedWord.size());

    const bool wordIsMisspelled = isMouseCursorInsideWord &&
                            m_highlighter->isActive() &&
                            !selectedWord.isEmpty() &&
                            m_highlighter &&
                            m_highlighter->isWordMisspelled(selectedWord);

    // If the user clicked a selected word, do nothing.
    // If the user clicked somewhere else, move the cursor there.
    // If the user clicked on a misspelled word, select that word.
    // Same behavior as in OpenOffice Writer.
    bool checkBlock = q->isSpellCheckingEnabledForBlock(cursorAtMouse.block().text());
    if (!selectedWordClicked) {
        if (wordIsMisspelled && checkBlock) {
            m_textEdit->setTextCursor(wordSelectCursor);
        } else {
            m_textEdit->setTextCursor(cursorAtMouse);
        }
        cursor = m_textEdit->textCursor();
    }

    // Use standard context menu for already selected words, correctly spelled
    // words and words inside quotes.
    if (!wordIsMisspelled || selectedWordClicked || !checkBlock) {
        return false;
    }
    execSuggestionMenu(event->globalPos(), selectedWord, cursor);
    return true;
}

void SpellCheckDecorator::Private::execSuggestionMenu(const QPoint &pos, const QString &selectedWord, const QTextCursor &_cursor)
{
    QTextCursor cursor = _cursor;
    QMenu menu; //don't use KMenu here we don't want auto management accelerator

    //Add the suggestions to the menu
    const QStringList reps = m_highlighter->suggestionsForWord(selectedWord);
    if (reps.isEmpty()) {
        QAction *suggestionsAction = menu.addAction(tr("No suggestions for %1").arg(selectedWord));
        suggestionsAction->setEnabled(false);
    } else {
        QStringList::const_iterator end(reps.constEnd());
        for (QStringList::const_iterator it = reps.constBegin(); it != end; ++it) {
            menu.addAction(*it);
        }
    }

    menu.addSeparator();

    QAction *ignoreAction = menu.addAction(tr("Ignore"));
    QAction *addToDictAction = menu.addAction(tr("Add to Dictionary"));
    //Execute the popup inline
    const QAction *selectedAction = menu.exec(pos);

    if (selectedAction) {
        Q_ASSERT(cursor.selectedText() == selectedWord);

        if (selectedAction == ignoreAction) {
            m_highlighter->ignoreWord(selectedWord);
            m_highlighter->rehighlight();
        }
        else if (selectedAction == addToDictAction) {
            m_highlighter->addWordToDictionary(selectedWord);
            m_highlighter->rehighlight();
        }

        // Other actions can only be one of the suggested words
        else {
            const QString replacement = selectedAction->text();
            Q_ASSERT(reps.contains(replacement));
            cursor.insertText(replacement);
            m_textEdit->setTextCursor(cursor);
        }
    }
}

void SpellCheckDecorator::Private::createDefaultHighlighter()
{
    m_highlighter = new Highlighter(m_textEdit);
}

SpellCheckDecorator::SpellCheckDecorator(QTextEdit *textEdit)
: QObject(textEdit)
, d(new Private(this, textEdit))
{
}

SpellCheckDecorator::~SpellCheckDecorator()
{
    delete d;
}

void SpellCheckDecorator::setHighlighter(Highlighter *highlighter)
{
    d->m_highlighter = highlighter;
}

Highlighter *SpellCheckDecorator::highlighter() const
{
    if (!d->m_highlighter) {
        d->createDefaultHighlighter();
    }
    return d->m_highlighter;
}

bool SpellCheckDecorator::eventFilter(QObject * /*obj*/, QEvent *event)
{
    if (event->type() == QEvent::ContextMenu) {
        return d->onContextMenuEvent(static_cast<QContextMenuEvent *>(event));
    }
    return false;
}

bool SpellCheckDecorator::isSpellCheckingEnabledForBlock(const QString &textBlock) const
{
    Q_UNUSED(textBlock);
    return true;
}

} // namespace

