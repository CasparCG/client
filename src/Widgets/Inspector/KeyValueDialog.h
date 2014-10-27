#pragma once

#include "../Shared.h"
#include "ui_KeyValueDialog.h"

#include <QtCore/QEvent>

#include <QtGui/QDialog>
#include <QtGui/QWidget>

class WIDGETS_EXPORT KeyValueDialog : public QDialog, Ui::KeyValueDialog
{
    Q_OBJECT

    public:
        explicit KeyValueDialog(QWidget* parent = 0);

        const QString getKey() const;
        const QString getValue() const;

        void setKey(const QString& key);
        void setValue(const QString& value);
        void setTitle(const QString& title);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);
        void accept();
};
