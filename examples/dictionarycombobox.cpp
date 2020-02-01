/*
    SPDX-FileCopyrightText: 2008 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "dictionarycombobox.h"

#include <qapplication.h>
#include <QDebug>
#include <QHBoxLayout>
#include <QPushButton>

using namespace Sonnet;

class DictionaryComboBoxTest : public QWidget
{
    Q_OBJECT
public:
    DictionaryComboBoxTest()
    {
        QHBoxLayout *topLayout = new QHBoxLayout(this);
        dcb = new DictionaryComboBox(this);
        topLayout->addWidget(dcb, 1);
        connect(dcb, SIGNAL(dictionaryChanged(QString)), SLOT(dictChanged(QString)));
        connect(dcb, SIGNAL(dictionaryNameChanged(QString)), SLOT(dictNameChanged(QString)));
        QPushButton *btn = new QPushButton(QStringLiteral("Dump"), this);
        topLayout->addWidget(btn);
        connect(btn, SIGNAL(clicked()), SLOT(dump()));
    }

public Q_SLOTS:
    void dump()
    {
        qDebug() << "Current dictionary: " << dcb->currentDictionary();
        qDebug() << "Current dictionary name: " << dcb->currentDictionaryName();
    }

    void dictChanged(const QString &name)
    {
        qDebug() << "Current dictionary changed: " << name;
    }

    void dictNameChanged(const QString &name)
    {
        qDebug() << "Current dictionary name changed: " << name;
    }

private:
    DictionaryComboBox *dcb;
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    DictionaryComboBoxTest *test = new DictionaryComboBoxTest();
    test->show();

    return app.exec();
}

#include "dictionarycombobox.moc"
