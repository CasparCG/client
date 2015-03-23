#pragma once

#include "Shared.h"
#include "ui_AboutDialog.h"

#include <QtWidgets/QDialog>
#include <QtWidgets/QWidget>

class WIDGETS_EXPORT AboutDialog : public QDialog, Ui::AboutDialog
{
    Q_OBJECT

    public:
        explicit AboutDialog(QWidget* parent = 0);
};
