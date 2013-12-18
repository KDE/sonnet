/**
 * configdialog.cpp
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
#include "configdialog.h"
#include "configwidget.h"

#include <QDialogButtonBox>
#include <QVBoxLayout>

using namespace Sonnet;

class ConfigDialog::Private
{
public:
    Private( ConfigDialog *parent )
       : q( parent ) {}
    ConfigWidget *ui;
    ConfigDialog *q;
    void slotConfigChanged();
};

void ConfigDialog::Private::slotConfigChanged()
{
  emit q->languageChanged( ui->language() );
}

ConfigDialog::ConfigDialog(QWidget *parent)
    : QDialog(parent),
      d(new Private(this))
{
    setObjectName( "SonnetConfigDialog" );
    setModal( true );
    setWindowTitle( tr( "Spell Checking Configuration" ) );

    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    d->ui = new ConfigWidget(this);
    layout->addWidget(d->ui);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
    buttonBox->setStandardButtons(QDialogButtonBox::Ok
                                | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

    connect(buttonBox, SIGNAL(accepted()),
            this, SLOT(slotOk()));
    connect(buttonBox, SIGNAL(rejected()),
            this, SLOT(reject()));
    /*
    connect(buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()),
            this, SLOT(slotApply()));
        */
    connect(d->ui, SIGNAL(configChanged()),
            this, SLOT(slotConfigChanged()));
    
    connect(d->ui, SIGNAL(configChanged()),
            this, SIGNAL(configChanged()));
}

ConfigDialog::~ConfigDialog()
{
    delete d;
}

void ConfigDialog::slotOk()
{
    d->ui->save();
    accept();
}

void ConfigDialog::slotApply()
{
    d->ui->save();
}

void ConfigDialog::setLanguage( const QString &language )
{
    d->ui->setLanguage( language );
}

QString ConfigDialog::language() const
{
    return d->ui->language();
}

#include "moc_configdialog.cpp"
