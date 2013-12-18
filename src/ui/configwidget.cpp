/**
 * configwidget.cpp
 *
 * Copyright (C)  2004  Zack Rusin <zack@kde.org>
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
#include "configwidget.h"
#include "ui_configui.h"

#include "loader_p.h"
#include "settings_p.h"

#include <QCheckBox>
#include <QLayout>
#include <QListWidgetItem>

using namespace Sonnet;

class ConfigWidget::Private
{
public:
    Loader *loader;
    Ui_SonnetConfigUI ui;
    QWidget *wdg;
};

ConfigWidget::ConfigWidget(QWidget *parent)
    : QWidget(parent),
      d(new Private)
{
    d->loader = Loader::openLoader();

    QVBoxLayout *layout = new QVBoxLayout( this );
    layout->setMargin( 0 );
    layout->setObjectName( "SonnetConfigUILayout" );
    d->wdg = new QWidget( this );
    d->ui.setupUi( d->wdg );

    //QStringList clients = d->loader->clients();
    d->ui.m_langCombo->setCurrentByDictionary( d->loader->settings()->defaultLanguage() );
    //d->ui->m_clientCombo->insertStringList( clients );
    d->ui.m_skipUpperCB->setChecked( !d->loader->settings()->checkUppercase() );
    d->ui.m_skipRunTogetherCB->setChecked( d->loader->settings()->skipRunTogether() );
    d->ui.m_checkerEnabledByDefaultCB->setChecked( d->loader->settings()->checkerEnabledByDefault() );
    QStringList ignoreList = d->loader->settings()->currentIgnoreList();
    ignoreList.sort();
    d->ui.ignoreListWidget->addItems(ignoreList);
    d->ui.m_bgSpellCB->setChecked( d->loader->settings()->backgroundCheckerEnabled() );
    d->ui.m_bgSpellCB->hide();//hidden by default
    connect(d->ui.addButton, SIGNAL(clicked()), SLOT(slotIgnoreAdded()));
    connect(d->ui.removeButton, SIGNAL(clicked()), SLOT(slotIgnoreAdded()));

    layout->addWidget( d->wdg );
    connect(d->ui.m_langCombo, SIGNAL(dictionaryChanged(QString)), this, SIGNAL(configChanged()));
    connect(d->ui.m_bgSpellCB, SIGNAL(clicked(bool)),this,SIGNAL(configChanged()));
    connect(d->ui.m_skipUpperCB, SIGNAL(clicked(bool)), this, SIGNAL(configChanged()));
    connect(d->ui.m_skipRunTogetherCB, SIGNAL(clicked(bool)), this, SIGNAL(configChanged()));
    connect(d->ui.m_checkerEnabledByDefaultCB, SIGNAL(clicked(bool)), this, SIGNAL(configChanged()));
}

ConfigWidget::~ConfigWidget()
{
    delete d;
}

void ConfigWidget::save()
{
    setFromGui();
}

void ConfigWidget::setFromGui()
{
    if (d->ui.m_langCombo->count() ) {
        d->loader->settings()->setDefaultLanguage( d->ui.m_langCombo->currentDictionary() );
    }
    d->loader->settings()->setCheckUppercase(
        !d->ui.m_skipUpperCB->isChecked() );
    d->loader->settings()->setSkipRunTogether(
        d->ui.m_skipRunTogetherCB->isChecked() );
    d->loader->settings()->setBackgroundCheckerEnabled(
        d->ui.m_bgSpellCB->isChecked() );
    d->loader->settings()->setCheckerEnabledByDefault(
        d->ui.m_checkerEnabledByDefaultCB->isChecked() );
}

void ConfigWidget::slotIgnoreWordAdded()
{
    QStringList ignoreList = d->loader->settings()->currentIgnoreList();
    QString newWord = d->ui.newIgnoreEdit->text();
    if (newWord.isEmpty() || ignoreList.contains(newWord))
        return;
    ignoreList.append(newWord);
    d->loader->settings()->setCurrentIgnoreList(ignoreList);
    
    d->ui.ignoreListWidget->clear();
    d->ui.ignoreListWidget->addItems(ignoreList);
}

void ConfigWidget::slotIgnoreWordRemoved()
{
    QStringList ignoreList = d->loader->settings()->currentIgnoreList();
    const QList<QListWidgetItem*> selectedItems = d->ui.ignoreListWidget->selectedItems();
    Q_FOREACH(const QListWidgetItem *item, selectedItems) {
        ignoreList.removeAll(item->text());
    }
    d->loader->settings()->setCurrentIgnoreList(ignoreList);
    
    d->ui.ignoreListWidget->clear();
    d->ui.ignoreListWidget->addItems(ignoreList);
}

void ConfigWidget::setBackgroundCheckingButtonShown( bool b )
{
    d->ui.m_bgSpellCB->setVisible( b );
}

bool ConfigWidget::backgroundCheckingButtonShown() const
{
    return !d->ui.m_bgSpellCB->isHidden();
}

void ConfigWidget::slotDefault()
{
    d->ui.m_skipUpperCB->setChecked( false );
    d->ui.m_skipRunTogetherCB->setChecked( false );
    d->ui.m_checkerEnabledByDefaultCB->setChecked( false );
    d->ui.m_bgSpellCB->setChecked( true );
    d->ui.ignoreListWidget->clear();
}

void ConfigWidget::setLanguage( const QString &language )
{
    d->ui.m_langCombo->setCurrentByDictionary( language );
}

QString ConfigWidget::language() const
{
    if ( d->ui.m_langCombo->count() ) {
        return d->ui.m_langCombo->currentDictionary();
    } else {
        return QString();
    }
}

