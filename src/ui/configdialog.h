/*
 * configdialog.h
 *
 * SPDX-FileCopyrightText: 2004 Zack Rusin <zack@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */
#ifndef SONNET_CONFIGDIALOG_H
#define SONNET_CONFIGDIALOG_H

#include <QDialog>
class ConfigDialogPrivate;
#include "sonnetui_export.h"

namespace Sonnet
{
/// The sonnet ConfigDialog
class SONNETUI_EXPORT ConfigDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConfigDialog(QWidget *parent);
    ~ConfigDialog() override;

    /**
     * Sets the language/dictionary that will be selected by default
     * in this config dialog.
     * This overrides the setting in the config file.
     *
     * @param language the language which will be selected by default.
     * @since 4.1
     */
    void setLanguage(const QString &language);
    /**
     * return selected language
     * @since 4.8.1
     */
    QString language() const;

protected Q_SLOTS:
    virtual void slotOk();
    virtual void slotApply();

Q_SIGNALS:

    /**
     * This is emitted all the time when we change config and not just language
     *
     * @param language the language which the user has selected
     * @since 4.1
     */

    void languageChanged(const QString &language);

    /**
     * This is emitted when configChanged
     * @since 4.8.1
     */
    void configChanged();

private:
    ConfigDialogPrivate *const d;
    Q_DISABLE_COPY(ConfigDialog)
    Q_PRIVATE_SLOT(d, void slotConfigChanged())
};
}

#endif
