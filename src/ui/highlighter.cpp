/**
 * highlighter.cpp
 *
 * Copyright (C)  2004  Zack Rusin <zack@kde.org>
 * Copyright (C)  2006  Laurent Montel <montel@kde.org>
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

#include "highlighter.h"

#include "speller.h"
#include "loader_p.h"
#include "filter_p.h"
#include "settings_p.h"

#include <QDebug>
#include <QTextEdit>
#include <QTextCharFormat>
#include <QTimer>
#include <QColor>
#include <QHash>
#include <QTextCursor>
#include <QEvent>
#include <QKeyEvent>
#include <QApplication>
#include <QMetaMethod>

namespace Sonnet {

class Highlighter::Private
{
public:
    ~Private();
    Filter     *filter;
    Loader     *loader;
    Speller    *dict;
    QHash<QString, Speller*> dictCache;
    QTextEdit *edit;
    bool active;
    bool automatic;
    bool completeRehighlightRequired;
    bool intraWordEditing;
    bool spellCheckerFound; //cached d->dict->isValid() value
    int disablePercentage;
    int disableWordCount;
    int wordCount, errorCount;
    QTimer *rehighlightRequest;
    QColor spellColor;
    int suggestionListeners; // #of connections for the newSuggestions signal
};

Highlighter::Private::~Private()
{
  qDeleteAll(dictCache);
  delete filter;
}

Highlighter::Highlighter(QTextEdit *textEdit,
                         const QString& configFile,
                         const QColor& _col)
    : QSyntaxHighlighter(textEdit),
      d(new Private)
{
    d->filter = Filter::defaultFilter();
    d->edit = textEdit;
    d->active = true;
    d->automatic = true;
    d->wordCount = 0;
    d->errorCount = 0;
    d->intraWordEditing = false;
    d->completeRehighlightRequired = false;
    d->spellColor = _col.isValid() ? _col : Qt::red;
    d->suggestionListeners = 0;

    textEdit->installEventFilter( this );
    textEdit->viewport()->installEventFilter( this );

    d->loader = Loader::openLoader();

    //Do not load an empty settings file as it will cause the spellchecker to fail
    //if the KLocale::global()->language() (default value) spellchecker is not installed,
    //and we have a global sonnetrc file with a spellcheck lang installed which could be used.
    if (!configFile.isEmpty()) {
        d->filter->setSettings(d->loader->settings());
    }

    d->dict = new Sonnet::Speller();
    d->spellCheckerFound = d->dict->isValid();
    d->rehighlightRequest = new QTimer(this);
    connect( d->rehighlightRequest, SIGNAL(timeout()),
             this, SLOT(slotRehighlight()));

    if(!d->spellCheckerFound)
        return;

    d->dictCache.insert(d->dict->language(), d->dict);

    d->disablePercentage = d->loader->settings()->disablePercentageWordError();
    d->disableWordCount = d->loader->settings()->disableWordErrorCount();

    //Add kde personal word
    const QStringList l = Highlighter::personalWords();
    for ( QStringList::ConstIterator it = l.begin(); it != l.end(); ++it ) {
        d->dict->addToSession( *it );
    }
    d->completeRehighlightRequired = true;
    d->rehighlightRequest->setInterval(0);
    d->rehighlightRequest->setSingleShot(true);
    d->rehighlightRequest->start();
}

Highlighter::~Highlighter()
{
    delete d;
}

bool Highlighter::spellCheckerFound() const
{
    return d->spellCheckerFound;
}

// Since figuring out spell correction suggestions is extremely costly,
// we keep track of whether the user actually wants some, and only offer them
// in that case
// With Qt5, we could use QObject::isSignalConnected(), but this is faster.
void Highlighter::connectNotify(const QMetaMethod& signal)
{
    if (signal.methodSignature() == "newSuggestions(QString,QStringList)")
        ++d->suggestionListeners;
    QSyntaxHighlighter::connectNotify(signal);
}

void Highlighter::disconnectNotify(const QMetaMethod& signal)
{
    if (signal.methodSignature() == "newSuggestions(QString,QStringList)")
        --d->suggestionListeners;
    QSyntaxHighlighter::disconnectNotify(signal);
}

void Highlighter::slotRehighlight()
{
    if (d->completeRehighlightRequired) {
        d->wordCount  = 0;
        d->errorCount = 0;
        rehighlight();

    } else {
	//rehighlight the current para only (undo/redo safe)
        QTextCursor cursor = d->edit->textCursor();
        cursor.insertText( "" );
    }
    //if (d->checksDone == d->checksRequested)
    //d->completeRehighlightRequired = false;
    QTimer::singleShot( 0, this, SLOT(slotAutoDetection()));
}


QStringList Highlighter::personalWords()
{
    QStringList l;
    l.append( "KMail" );
    l.append( "KOrganizer" );
    l.append( "KAddressBook" );
    l.append( "KHTML" );
    l.append( "KIO" );
    l.append( "KJS" );
    l.append( "Konqueror" );
    l.append( "Sonnet" );
    l.append( "Kontact" );
    l.append( "Qt" );
    return l;
}

bool Highlighter::automatic() const
{
    return d->automatic;
}

bool Highlighter::intraWordEditing() const
{
    return d->intraWordEditing;
}

void Highlighter::setIntraWordEditing( bool editing )
{
    d->intraWordEditing = editing;
}


void Highlighter::setAutomatic( bool automatic )
{
    if ( automatic  == d->automatic )
        return;

    d->automatic = automatic;
    if ( d->automatic )
        slotAutoDetection();
}

void Highlighter::slotAutoDetection()
{
    bool savedActive = d->active;

    //don't disable just because 1 of 4 is misspelled.
    if (d->automatic && d->wordCount >= 10) {
        // tme = Too many errors
        bool tme = (d->errorCount >= d->disableWordCount) && (
            d->errorCount * 100 >= d->disablePercentage * d->wordCount);
        if (d->active && tme) {
            d->active = false;
        } else if (!d->active && !tme) {
            d->active = true;
        }
    }

    if (d->active != savedActive) {
        if (d->active) {
            emit activeChanged(tr("As-you-type spell checking enabled."));
        } else {
            emit activeChanged(tr( "Too many misspelled words. "
                               "As-you-type spell checking disabled."));
        }

        d->completeRehighlightRequired = true;
        d->rehighlightRequest->setInterval(100);
        d->rehighlightRequest->setSingleShot(true);
    }

}

void Highlighter::setActive( bool active )
{
    if ( active == d->active )
        return;
    d->active = active;
    rehighlight();


    if ( d->active )
        emit activeChanged( tr("As-you-type spell checking enabled.") );
    else
        emit activeChanged( tr("As-you-type spell checking disabled.") );
}

bool Highlighter::isActive() const
{
    return d->active;
}

void Highlighter::highlightBlock(const QString &text)
{
    if (text.isEmpty() || !d->active || !d->spellCheckerFound)
        return;

    d->filter->setBuffer( text );
    Word w = d->filter->nextWord();
    while ( !w.end ) {
        ++d->wordCount;
        if (d->dict->isMisspelled(w.word)) {
            ++d->errorCount;
            setMisspelled(w.start, w.word.length());
            if (d->suggestionListeners)
                emit newSuggestions(w.word, d->dict->suggest(w.word));
        } else
            unsetMisspelled(w.start, w.word.length());
        w = d->filter->nextWord();
    }
    //QTimer::singleShot( 0, this, SLOT(checkWords()) );
    setCurrentBlockState(0);
}

QString Highlighter::currentLanguage() const
{
    return d->dict->language();
}

void Highlighter::setCurrentLanguage(const QString &lang)
{
    if (!d->dictCache.contains(lang)) {
        d->dict = new Speller(*d->dict);
        d->dict->setLanguage(lang);
        if (d->dict->isValid()) {
            d->dictCache.insert(lang, d->dict);
        } else {
            d->spellCheckerFound = false;
            qWarning() << "No dictionary for \"" << lang << "\" staying with the current language.";
            return;
        }
    }
    d->dict = d->dictCache[lang];
    d->spellCheckerFound = d->dict->isValid();
    d->wordCount = 0;
    d->errorCount = 0;
    if (d->automatic) {
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

void Highlighter::unsetMisspelled( int start, int count )
{
    setFormat(start, count, QTextCharFormat());
}

bool Highlighter::eventFilter( QObject *o, QEvent *e)
{
#if 0
    if (o == textEdit() && (e->type() == QEvent::FocusIn)) {
        if ( d->globalConfig ) {
            QString skey = spellKey();
            if ( d->spell && d->spellKey != skey ) {
                d->spellKey = skey;
                KDictSpellingHighlighter::dictionaryChanged();
            }
        }
    }
#endif
    if (!d->spellCheckerFound)
        return false;
    if (o == d->edit  && (e->type() == QEvent::KeyPress)) {
	QKeyEvent *k = static_cast<QKeyEvent *>(e);
	//d->autoReady = true;
	if (d->rehighlightRequest->isActive()) // try to stay out of the users way
	    d->rehighlightRequest->start( 500 );
	if ( k->key() == Qt::Key_Enter ||
	     k->key() == Qt::Key_Return ||
	     k->key() == Qt::Key_Up ||
	     k->key() == Qt::Key_Down ||
	     k->key() == Qt::Key_Left ||
	     k->key() == Qt::Key_Right ||
	     k->key() == Qt::Key_PageUp ||
	     k->key() == Qt::Key_PageDown ||
	     k->key() == Qt::Key_Home ||
	     k->key() == Qt::Key_End ||
	     (( k->modifiers()== Qt::ControlModifier ) &&
	      ((k->key() == Qt::Key_A) ||
	       (k->key() == Qt::Key_B) ||
	       (k->key() == Qt::Key_E) ||
	       (k->key() == Qt::Key_N) ||
	       (k->key() == Qt::Key_P))) ) {
	    if ( intraWordEditing() ) {
		setIntraWordEditing( false );
		d->completeRehighlightRequired = true;
		d->rehighlightRequest->setInterval(500);
                d->rehighlightRequest->setSingleShot(true);
                d->rehighlightRequest->start();
	    }
#if 0
	    if (d->checksDone != d->checksRequested) {
		// Handle possible change of paragraph while
		// words are pending spell checking
		d->completeRehighlightRequired = true;
		d->rehighlightRequest->start( 500, true );
	    }
#endif
	} else {
	    setIntraWordEditing( true );
	}
	if ( k->key() == Qt::Key_Space ||
	     k->key() == Qt::Key_Enter ||
	     k->key() == Qt::Key_Return ) {
	    QTimer::singleShot( 0, this, SLOT(slotAutoDetection()));
	}
    }

    else if ( o == d->edit->viewport() &&
              ( e->type() == QEvent::MouseButtonPress )) {
	//d->autoReady = true;
	if ( intraWordEditing() ) {
	    setIntraWordEditing( false );
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
    d->dict->addToPersonal(word);
}

void Highlighter::ignoreWord(const QString &word)
{
    d->dict->addToSession(word);
}

QStringList Highlighter::suggestionsForWord(const QString &word, int max)
{
    QStringList suggestions = d->dict->suggest(word);
    if ( max != -1 ) {
        while ( suggestions.count() > max )
            suggestions.removeLast();
    }
    return suggestions;
}

bool Highlighter::isWordMisspelled(const QString &word)
{
    return d->dict->isMisspelled(word);
}

void Highlighter::setMisspelledColor(const QColor &color)
{
    d->spellColor = color;
}

bool Highlighter::checkerEnabledByDefault() const
{
    return d->loader->settings()->checkerEnabledByDefault();
}


}
