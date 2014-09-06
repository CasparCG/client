#pragma once

#include "Shared.h"
#include "ui_HelpDialog.h"

#if QT_VERSION >= 0x050000
#include <QtWidgets/QDialog>
#include <QtWidgets/QWidget>
#else
#include <QtGui/QDialog>
#include <QtGui/QWidget>
#endif

class WIDGETS_EXPORT HelpDialog : public QDialog, Ui::HelpDialog
{
    Q_OBJECT

    public:
        explicit HelpDialog(QWidget* parent = 0);
};
