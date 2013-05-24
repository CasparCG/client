#pragma once

#include "Shared.h"
#include "ui_OscWidget.h"

#include <QtGui/QWidget>

class WIDGETS_EXPORT OscWidget : public QWidget, Ui::OscWidget
{
    Q_OBJECT

    public:
        explicit OscWidget(QWidget* parent = 0);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);
};
