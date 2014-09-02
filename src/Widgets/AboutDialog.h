#pragma once

#include "Shared.h"
#include "ui_AboutDialog.h"

#if QT_VERSION >= 0x050000
#include <QtWidgets/QDialog>
#include <QtWidgets/QWidget>
#else
#include <QtGui/QDialog>
#include <QtGui/QWidget>
#endif

class WIDGETS_EXPORT AboutDialog : public QDialog, Ui::AboutDialog
{
    Q_OBJECT

    public:
        explicit AboutDialog(QWidget* parent = 0);
};
