#pragma once

#include "Shared.h"
#include "ui_LiveDialog.h"

#include <QtGui/QDialog>
#include <QtGui/QWidget>

class WIDGETS_EXPORT LiveDialog : public QDialog, Ui::LiveDialog
{
    Q_OBJECT

    public:
        explicit LiveDialog(QWidget* parent = 0);

        QWidget* getRenderTarget();
};
