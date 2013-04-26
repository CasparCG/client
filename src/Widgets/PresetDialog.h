#pragma once

#include "Shared.h"
#include "ui_PresetDialog.h"

#include <QtGui/QDialog>
#include <QtGui/QWidget>

class WIDGETS_EXPORT PresetDialog : public QDialog, Ui::PresetDialog
{
    Q_OBJECT

    public:
        explicit PresetDialog(QWidget* parent = 0);

        const QString getName() const;

    protected:
        void accept();

    private:
            Q_SLOT void nameChanged(QString);
};
