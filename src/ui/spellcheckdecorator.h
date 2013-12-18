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
#ifndef SPELLCHECKDECORATOR_H
#define SPELLCHECKDECORATOR_H

#include <QtCore/QObject>

#include <sonnet/sonnetui_export.h>

class QTextEdit;

namespace Sonnet
{

class Highlighter;

/**
 * @short Connects a Sonnet::Highlighter to a QTextEdit extending the context menu of the text edit with spell check suggestions
 * @author Aurélien Gâteau <agateau@kde.org>
 * @since 5.0
 **/

class SONNETUI_EXPORT SpellCheckDecorator : public QObject
{
    Q_OBJECT
public:
    explicit SpellCheckDecorator(QTextEdit *textEdit);
    ~SpellCheckDecorator();


    /**
     * Set a custom highlighter on the decorator.
     *
     * SpellCheckDecorator does not take ownership of the new highlighter.
     */
    void setHighlighter(Highlighter *highlighter);

    /**
     * Returns the hightlighter used by the decorator
     */
    Highlighter *highlighter() const;

protected:
    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;

    /**
     * Returns true if the spell checking should be enabled for a given block of text
     * The default implementation always returns true.
     */
    virtual bool isSpellCheckingEnabledForBlock(const QString &textBlock) const;

private:
    class Private;
    Private *const d;
    Q_DISABLE_COPY(SpellCheckDecorator)
};

}

#endif /* SPELLCHECKDECORATOR_H */
