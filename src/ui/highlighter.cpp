/*
 * highlighter.cpp
 *
 * SPDX-FileCopyrightText: 2004 Zack Rusin <zack@kde.org>
 * SPDX-FileCopyrightText: 2006 Laurent Montel <montel@kde.org>
 * SPDX-FileCopyrightText: 2013 Martin Sandsmark <martin.sandsmark@org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include "highlighter.h"

#include "languagefilter_p.h"
#include "loader_p.h"
#include "settingsimpl_p.h"
#include "speller.h"
#include "tokenizer_p.h"

#include "ui_debug.h"

#include <QColor>
#include <QEvent>
#include <QHash>
#include <QKeyEvent>
#include <QMetaMethod>
#include <QPlainTextEdit>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QTextEdit>
#include <QTimer>

namespace Sonnet
{
// Cache of previously-determined languages (when using AutoDetectLanguage)
// There is one such cache per block (paragraph)
class LanguageCache : public QTextBlockUserData
{
public:
    // Key: QPair<start, length>
    // Value: language name
    QMap<QPair<int, int>, QString> languages;

    // Remove all cached language information after @p pos
    void invalidate(int pos)
    {
        QMutableMapIterator<QPair<int, int>, QString> it(languages);
        it.toBack();
        while (it.hasPrevious()) {
            it.previous();
            if (it.key().first + it.key().second >= pos) {
                it.remove();
            } else {
                break;
            }
        }
    }

    QString languageAtPos(int pos) const
    {
        // The data structure isn't really great for such lookups...
        QMapIterator<QPair<int, int>, QString> it(languages);
        while (it.hasNext()) {
            it.next();
            if (it.key().first <= pos && it.key().first + it.key().second >= pos) {
                return it.value();
            }
        }
        return QString();
    }
};

class HighlighterPrivate
{
public:
    HighlighterPrivate(Highlighter *qq, const QColor &col)
        : textEdit(nullptr)
        , plainTextEdit(nullptr)
        , spellColor(col)
        , q(qq)
    {
        tokenizer = new WordTokenizer();
        active = true;
        automatic = false;
        autoDetectLanguageDisabled = false;
        wordCount = 0;
        errorCount = 0;
        intraWordEditing = false;
        completeRehighlightRequired = false;
        spellColor = spellColor.isValid() ? spellColor : Qt::red;
        languageFilter = new LanguageFilter(new SentenceTokenizer());

        loader = Loader::openLoader();
        loader->settings()->restore();

        spellchecker = new Sonnet::Speller();
        spellCheckerFound = spellchecker->isValid();
        rehighlightRequest = new QTimer(q);
        q->connect(rehighlightRequest, &QTimer::timeout, q, &Highlighter::slotRehighlight);

        if (!spellCheckerFound) {
            return;
        }

        disablePercentage = loader->settings()->disablePercentageWordError();
        disableWordCount = loader->settings()->disableWordErrorCount();

        completeRehighlightRequired = true;
        rehighlightRequest->setInterval(0);
        rehighlightRequest->setSingleShot(true);
        rehighlightRequest->start();
    }

    ~HighlighterPrivate();
    WordTokenizer *tokenizer = nullptr;
    LanguageFilter *languageFilter = nullptr;
    Loader *loader = nullptr;
    Speller *spellchecker = nullptr;
    QTextEdit *textEdit = nullptr;
    QPlainTextEdit *plainTextEdit = nullptr;
    bool active;
    bool automatic;
    bool autoDetectLanguageDisabled;
    bool completeRehighlightRequired;
    bool intraWordEditing;
    bool spellCheckerFound; // cached d->dict->isValid() value
    QMetaObject::Connection contentsChangeConnection;
    int disablePercentage = 0;
    int disableWordCount = 0;
    int wordCount, errorCount;
    QTimer *rehighlightRequest = nullptr;
    QColor spellColor;
    Highlighter *const q;
};

HighlighterPrivate::~HighlighterPrivate()
{
    delete spellchecker;
    delete languageFilter;
    delete tokenizer;
}

Highlighter::Highlighter(QTextEdit *edit, const QColor &_col)
    : QSyntaxHighlighter(edit)
    , d(new HighlighterPrivate(this, _col))
{
    d->textEdit = edit;
    d->textEdit->installEventFilter(this);
    d->textEdit->viewport()->installEventFilter(this);
}

Highlighter::Highlighter(QPlainTextEdit *edit, const QColor &col)
    : QSyntaxHighlighter(edit)
    , d(new HighlighterPrivate(this, col))
{
    d->plainTextEdit = edit;
    setDocument(d->plainTextEdit->document());
    d->plainTextEdit->installEventFilter(this);
    d->plainTextEdit->viewport()->installEventFilter(this);
}

Highlighter::~Highlighter()
{
    if (d->contentsChangeConnection) {
        // prevent crash from QSyntaxHighlighter::~QSyntaxHighlighter -> (...) -> QTextDocument::contentsChange() signal emission:
        // ASSERT failure in Sonnet::Highlighter: "Called object is not of the correct type (class destructor may have already run)"
        QObject::disconnect(d->contentsChangeConnection);
    }
}

bool Highlighter::spellCheckerFound() const
{
    return d->spellCheckerFound;
}

void Highlighter::slotRehighlight()
{
    if (d->completeRehighlightRequired) {
        d->wordCount = 0;
        d->errorCount = 0;
        rehighlight();
    } else {
        // rehighlight the current para only (undo/redo safe)
        QTextCursor cursor;
        if (d->textEdit) {
            cursor = d->textEdit->textCursor();
        } else {
            cursor = d->plainTextEdit->textCursor();
        }
        if (cursor.hasSelection()) {
            cursor.clearSelection();
        }
        cursor.insertText(QString());
    }
    // if (d->checksDone == d->checksRequested)
    // d->completeRehighlightRequired = false;
    QTimer::singleShot(0, this, SLOT(slotAutoDetection()));
}

bool Highlighter::automatic() const
{
    return d->automatic;
}

bool Highlighter::autoDetectLanguageDisabled() const
{
    return d->autoDetectLanguageDisabled;
}

bool Highlighter::intraWordEditing() const
{
    return d->intraWordEditing;
}

void Highlighter::setIntraWordEditing(bool editing)
{
    d->intraWordEditing = editing;
}

void Highlighter::setAutomatic(bool automatic)
{
    if (automatic == d->automatic) {
        return;
    }

    d->automatic = automatic;
    if (d->automatic) {
        slotAutoDetection();
    }
}

void Highlighter::setAutoDetectLanguageDisabled(bool autoDetectDisabled)
{
    d->autoDetectLanguageDisabled = autoDetectDisabled;
}

void Highlighter::slotAutoDetection()
{
    bool savedActive = d->active;

    // don't disable just because 1 of 4 is misspelled.
    if (d->automatic && d->wordCount >= 10) {
        // tme = Too many errors
        /* clang-format off */
        bool tme = (d->errorCount >= d->disableWordCount)
                   && (d->errorCount * 100 >= d->disablePercentage * d->wordCount);
        /* clang-format on */

        if (d->active && tme) {
            d->active = false;
        } else if (!d->active && !tme) {
            d->active = true;
        }
    }

    if (d->active != savedActive) {
        if (d->active) {
            Q_EMIT activeChanged(tr("As-you-type spell checking enabled."));
        } else {
            qCDebug(SONNET_LOG_UI) << "Sonnet: Disabling spell checking, too many errors";
            Q_EMIT activeChanged(
                tr("Too many misspelled words. "
                   "As-you-type spell checking disabled."));
        }

        d->completeRehighlightRequired = true;
        d->rehighlightRequest->setInterval(100);
        d->rehighlightRequest->setSingleShot(true);
    }
}

void Highlighter::setActive(bool active)
{
    if (active == d->active) {
        return;
    }
    d->active = active;
    rehighlight();

    if (d->active) {
        Q_EMIT activeChanged(tr("As-you-type spell checking enabled."));
    } else {
        Q_EMIT activeChanged(tr("As-you-type spell checking disabled."));
    }
}

bool Highlighter::isActive() const
{
    return d->active;
}

void Highlighter::contentsChange(int pos, int add, int rem)
{
    // Invalidate the cache where the text has changed
    const QTextBlock &lastBlock = document()->findBlock(pos + add - rem);
    QTextBlock block = document()->findBlock(pos);
    do {
        LanguageCache *cache = dynamic_cast<LanguageCache *>(block.userData());
        if (cache) {
            cache->invalidate(pos - block.position());
        }
        block = block.next();
    } while (block.isValid() && block < lastBlock);
}

static bool hasNotEmptyText(const QString &text)
{
    for (int i = 0; i < text.length(); ++i) {
        if (!text.at(i).isSpace()) {
            return true;
        }
    }
    return false;
}

void Highlighter::highlightBlock(const QString &text)
{
    if (!hasNotEmptyText(text) || !d->active || !d->spellCheckerFound) {
        return;
    }

    if (!d->contentsChangeConnection) {
        d->contentsChangeConnection = connect(document(), &QTextDocument::contentsChange, this, &Highlighter::contentsChange);
    }

        d->languageFilter->setBuffer(text);

        LanguageCache *cache = dynamic_cast<LanguageCache *>(currentBlockUserData());
        if (!cache) {
            cache = new LanguageCache;
            setCurrentBlockUserData(cache);
        }

        const bool autodetectLanguage = d->spellchecker->testAttribute(Speller::AutoDetectLanguage);
        while (d->languageFilter->hasNext()) {
            Token sentence = d->languageFilter->next();
            if (autodetectLanguage && !d->autoDetectLanguageDisabled) {
                QString lang;
                QPair<int, int> spos = QPair<int, int>(sentence.position(), sentence.length());
                // try cache first
                if (cache->languages.contains(spos)) {
                    lang = cache->languages.value(spos);
                } else {
                    lang = d->languageFilter->language();
                    if (!d->languageFilter->isSpellcheckable()) {
                        lang.clear();
                    }
                    cache->languages[spos] = lang;
                }
                if (lang.isEmpty()) {
                    continue;
                }
                d->spellchecker->setLanguage(lang);
            }

            d->tokenizer->setBuffer(sentence.toString());
            int offset = sentence.position();
            while (d->tokenizer->hasNext()) {
                Token word = d->tokenizer->next();
                if (!d->tokenizer->isSpellcheckable()) {
                    continue;
                }
                ++d->wordCount;
                if (d->spellchecker->isMisspelled(word.toString())) {
                    ++d->errorCount;
                    setMisspelled(word.position() + offset, word.length());
                } else {
                    unsetMisspelled(word.position() + offset, word.length());
                }
            }
        }
    // QTimer::singleShot( 0, this, SLOT(checkWords()) );
    setCurrentBlockState(0);
}

QString Highlighter::currentLanguage() const
{
    return d->spellchecker->language();
}

void Highlighter::setCurrentLanguage(const QString &lang)
{
    QString prevLang = d->spellchecker->language();
    d->spellchecker->setLanguage(lang);
    d->spellCheckerFound = d->spellchecker->isValid();
    if (!d->spellCheckerFound) {
        qCDebug(SONNET_LOG_UI) << "No dictionary for \"" << lang << "\" staying with the current language.";
        d->spellchecker->setLanguage(prevLang);
        return;
    }
    d->wordCount = 0;
    d->errorCount = 0;
    if (d->automatic || d->active) {
        d->rehighlightRequest->start(0);
    }
}

void Highlighter::setMisspelled(int start, int count)
{
    QTextCharFormat format;
    format.setFontUnderline(true);
    format.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
    format.setUnderlineColor(d->spellColor);
    setFormat(start, count, format);
}

void Highlighter::unsetMisspelled(int start, int count)
{
    setFormat(start, count, QTextCharFormat());
}

bool Highlighter::eventFilter(QObject *o, QEvent *e)
{
    if (!d->spellCheckerFound) {
        return false;
    }
    if ((o == d->textEdit || o == d->plainTextEdit) && (e->type() == QEvent::KeyPress)) {
        QKeyEvent *k = static_cast<QKeyEvent *>(e);
        // d->autoReady = true;
        if (d->rehighlightRequest->isActive()) { // try to stay out of the users way
            d->rehighlightRequest->start(500);
        }
        /* clang-format off */
        if (k->key() == Qt::Key_Enter
            || k->key() == Qt::Key_Return
            || k->key() == Qt::Key_Up
            || k->key() == Qt::Key_Down
            || k->key() == Qt::Key_Left
            || k->key() == Qt::Key_Right
            || k->key() == Qt::Key_PageUp
            || k->key() == Qt::Key_PageDown
            || k->key() == Qt::Key_Home
            || k->key() == Qt::Key_End
            || (k->modifiers() == Qt::ControlModifier
                && (k->key() == Qt::Key_A
                    || k->key() == Qt::Key_B
                    || k->key() == Qt::Key_E
                    || k->key() == Qt::Key_N
                    || k->key() == Qt::Key_P))) { /* clang-format on */
            if (intraWordEditing()) {
                setIntraWordEditing(false);
                d->completeRehighlightRequired = true;
                d->rehighlightRequest->setInterval(500);
                d->rehighlightRequest->setSingleShot(true);
                d->rehighlightRequest->start();
            }
        } else {
            setIntraWordEditing(true);
        }
        if (k->key() == Qt::Key_Space //
            || k->key() == Qt::Key_Enter //
            || k->key() == Qt::Key_Return) {
            QTimer::singleShot(0, this, SLOT(slotAutoDetection()));
        }
    } else if (((d->textEdit && (o == d->textEdit->viewport())) //
                || (d->plainTextEdit && (o == d->plainTextEdit->viewport()))) //
               && (e->type() == QEvent::MouseButtonPress)) {
        // d->autoReady = true;
        if (intraWordEditing()) {
            setIntraWordEditing(false);
            d->completeRehighlightRequired = true;
            d->rehighlightRequest->setInterval(0);
            d->rehighlightRequest->setSingleShot(true);
            d->rehighlightRequest->start();
        }
    }
    return false;
}

void Highlighter::addWordToDictionary(const QString &word)
{
    d->spellchecker->addToPersonal(word);
}

void Highlighter::ignoreWord(const QString &word)
{
    d->spellchecker->addToSession(word);
}

QStringList Highlighter::suggestionsForWord(const QString &word, int max)
{
    QStringList suggestions = d->spellchecker->suggest(word);
    if (max >= 0 && suggestions.count() > max) {
        suggestions = suggestions.mid(0, max);
    }
    return suggestions;
}

QStringList Highlighter::suggestionsForWord(const QString &word, const QTextCursor &cursor, int max)
{
    LanguageCache *cache = dynamic_cast<LanguageCache *>(cursor.block().userData());
    if (cache) {
        const QString cachedLanguage = cache->languageAtPos(cursor.positionInBlock());
        if (!cachedLanguage.isEmpty()) {
            d->spellchecker->setLanguage(cachedLanguage);
        }
    }
    QStringList suggestions = d->spellchecker->suggest(word);
    if (max >= 0 && suggestions.count() > max) {
        suggestions = suggestions.mid(0, max);
    }
    return suggestions;
}

bool Highlighter::isWordMisspelled(const QString &word)
{
    return d->spellchecker->isMisspelled(word);
}

void Highlighter::setMisspelledColor(const QColor &color)
{
    d->spellColor = color;
}

bool Highlighter::checkerEnabledByDefault() const
{
    return d->loader->settings()->checkerEnabledByDefault();
}

void Highlighter::setDocument(QTextDocument *document)
{
    d->contentsChangeConnection = {};
    QSyntaxHighlighter::setDocument(document);
}
}

#include "moc_highlighter.cpp"
