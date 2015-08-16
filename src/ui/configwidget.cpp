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

class ConfigWidgetPrivate
{
public:
    Loader *loader;
    Ui_SonnetConfigUI ui;
    QWidget *wdg;
};

ConfigWidget::ConfigWidget(QWidget *parent)
    : QWidget(parent),
      d(new ConfigWidgetPrivate)
{
    d->loader = Loader::openLoader();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setObjectName(QStringLiteral("SonnetConfigUILayout"));
    d->wdg = new QWidget(this);
    d->ui.setupUi(d->wdg);

    d->ui.m_langCombo->setCurrentByDictionary(d->loader->settings()->defaultLanguage());
    d->ui.m_skipUpperCB->setChecked(!d->loader->settings()->checkUppercase());
    d->ui.m_skipRunTogetherCB->setChecked(d->loader->settings()->skipRunTogether());
    d->ui.m_checkerEnabledByDefaultCB->setChecked(d->loader->settings()->checkerEnabledByDefault());
    d->ui.m_autodetectCB->setChecked(d->loader->settings()->autodetectLanguage());
    QStringList ignoreList = d->loader->settings()->currentIgnoreList();
    ignoreList.sort();
    d->ui.ignoreListWidget->addItems(ignoreList);
    d->ui.m_bgSpellCB->setChecked(d->loader->settings()->backgroundCheckerEnabled());
    d->ui.m_bgSpellCB->hide();//hidden by default
    connect(d->ui.addButton, SIGNAL(clicked()), SLOT(slotIgnoreWordAdded()));
    connect(d->ui.removeButton, SIGNAL(clicked()), SLOT(slotIgnoreWordRemoved()));

    layout->addWidget(d->wdg);
    connect(d->ui.m_langCombo, SIGNAL(dictionaryChanged(QString)), this, SIGNAL(configChanged()));
    connect(d->ui.m_bgSpellCB, SIGNAL(clicked(bool)), this, SIGNAL(configChanged()));
    connect(d->ui.m_skipUpperCB, SIGNAL(clicked(bool)), this, SIGNAL(configChanged()));
    connect(d->ui.m_skipRunTogetherCB, SIGNAL(clicked(bool)), this, SIGNAL(configChanged()));
    connect(d->ui.m_checkerEnabledByDefaultCB, SIGNAL(clicked(bool)), this, SIGNAL(configChanged()));
    connect(d->ui.m_autodetectCB, SIGNAL(clicked(bool)), this, SIGNAL(configChanged()));
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
    bool changed = false;
    if (d->ui.m_langCombo->count()) {
        if (d->loader->settings()->setDefaultLanguage(
            d->ui.m_langCombo->currentDictionary()))
        {
            changed = true;
        }
    }
    if (d->loader->settings()->setCheckUppercase(
        !d->ui.m_skipUpperCB->isChecked()))
    {
        changed = true;
    }
    if (d->loader->settings()->setSkipRunTogether(
        d->ui.m_skipRunTogetherCB->isChecked()))
    {
        changed = true;
    }
    if (d->loader->settings()->setBackgroundCheckerEnabled(
        d->ui.m_bgSpellCB->isChecked()))
    {
        changed = true;
    }
    if (d->loader->settings()->setCheckerEnabledByDefault(
        d->ui.m_checkerEnabledByDefaultCB->isChecked()))
    {
        changed = true;
    }
    if (d->loader->settings()->setAutodetectLanguage(
        d->ui.m_autodetectCB->isChecked()))
    {
        changed = true;
    }

    if (changed) {
        d->loader->settings()->save();
    }
}

void ConfigWidget::slotIgnoreWordAdded()
{
    QStringList ignoreList = d->loader->settings()->currentIgnoreList();
    QString newWord = d->ui.newIgnoreEdit->text();
    d->ui.newIgnoreEdit->clear();
    if (newWord.isEmpty() || ignoreList.contains(newWord)) {
        return;
    }
    ignoreList.append(newWord);
    d->loader->settings()->setCurrentIgnoreList(ignoreList);

    d->ui.ignoreListWidget->clear();
    d->ui.ignoreListWidget->addItems(ignoreList);
}

void ConfigWidget::slotIgnoreWordRemoved()
{
    QStringList ignoreList = d->loader->settings()->currentIgnoreList();
    const QList<QListWidgetItem *> selectedItems = d->ui.ignoreListWidget->selectedItems();
    Q_FOREACH (const QListWidgetItem *item, selectedItems) {
        ignoreList.removeAll(item->text());
    }
    d->loader->settings()->setCurrentIgnoreList(ignoreList);

    d->ui.ignoreListWidget->clear();
    d->ui.ignoreListWidget->addItems(ignoreList);
}

void ConfigWidget::setBackgroundCheckingButtonShown(bool b)
{
    d->ui.m_bgSpellCB->setVisible(b);
}

bool ConfigWidget::backgroundCheckingButtonShown() const
{
    return !d->ui.m_bgSpellCB->isHidden();
}

void ConfigWidget::slotDefault()
{
    d->ui.m_autodetectCB->setChecked(true);
    d->ui.m_skipUpperCB->setChecked(false);
    d->ui.m_skipRunTogetherCB->setChecked(false);
    d->ui.m_checkerEnabledByDefaultCB->setChecked(false);
    d->ui.m_bgSpellCB->setChecked(true);
    d->ui.ignoreListWidget->clear();
    d->ui.m_langCombo->setCurrentByDictionary(d->loader->settings()->defaultLanguage());
}

void ConfigWidget::setLanguage(const QString &language)
{
    d->ui.m_langCombo->setCurrentByDictionary(language);
}

QString ConfigWidget::language() const
{
    if (d->ui.m_langCombo->count()) {
        return d->ui.m_langCombo->currentDictionary();
    } else {
        return QString();
    }
}

