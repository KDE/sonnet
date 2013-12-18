/*
 * Copyright (c) 2003 Ingo Kloecker <kloecker@kde.org>
 * Copyright (c) 2008 Tom Albers <tomalbers@kde.nl>
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

#include "dictionarycombobox.h"

#include <speller.h>
#include <qdebug.h>

namespace Sonnet
{

//@cond PRIVATE 
class DictionaryComboBox::Private 
{
    public:
        Private( DictionaryComboBox* combo ) : q( combo ) {};
        DictionaryComboBox* q;
        void slotDictionaryChanged( int idx );
};

void DictionaryComboBox::Private::slotDictionaryChanged( int idx )
{
    emit q->dictionaryChanged( q->itemData( idx ).toString() );
    emit q->dictionaryNameChanged( q->itemText( idx ) );
}
//@endcon

DictionaryComboBox::DictionaryComboBox( QWidget * parent )
        : QComboBox( parent ), d( new DictionaryComboBox::Private( this ) )
{
    reloadCombo();
    connect( this, SIGNAL(activated(int)),
             SLOT(slotDictionaryChanged(int)) );
}

DictionaryComboBox::~DictionaryComboBox()
{
    delete d;
}

QString DictionaryComboBox::currentDictionaryName() const
{
    return currentText();
}

QString DictionaryComboBox::currentDictionary() const
{
    return itemData( currentIndex() ).toString();
}

void DictionaryComboBox::setCurrentByDictionaryName( const QString & name )
{
    if ( name.isEmpty() || name == currentText() )
        return;

    int idx = findText( name );
    if ( idx == -1 ) {
        //qDebug() << "name not found" << name;
        return;
    }

    setCurrentIndex( idx );
    d->slotDictionaryChanged( idx );
}

void DictionaryComboBox::setCurrentByDictionary( const QString & dictionary )
{
    if ( dictionary.isEmpty() || dictionary == itemData( currentIndex() ).toString() )
        return;

    int idx = findData( dictionary );
    if ( idx == -1 ) {
        qDebug() << "dictionary not found" << dictionary;
        return;
    }

    setCurrentIndex( idx );
    d->slotDictionaryChanged( idx );
}

void DictionaryComboBox::reloadCombo()
{
    clear();
    Sonnet::Speller* speller = new Sonnet::Speller();
    QMap<QString, QString> dictionaries = speller->availableDictionaries();
    QMapIterator<QString, QString> i( dictionaries );
    while ( i.hasNext() ) {
        i.next();
        //qDebug() << "Populate combo:" << i.key() << ":" << i.value();
        addItem( i.key(), i.value() );
    }
    delete speller;
}

}

#include "moc_dictionarycombobox.cpp"
