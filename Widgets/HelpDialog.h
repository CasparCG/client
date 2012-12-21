#pragma once

#include "Shared.h"
#include "ui_HelpDialog.h"

#include <QtGui/QDialog>
#include <QtGui/QWidget>

class WIDGETS_EXPORT HelpDialog : public QDialog, Ui::HelpDialog
{
    Q_OBJECT

    public:
        explicit HelpDialog(QWidget* parent = 0);
};
