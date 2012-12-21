#pragma once

#include "Shared.h"
#include "ui_ClockWidget.h"

#include <QtGui/QWidget>

class WIDGETS_EXPORT ClockWidget : public QWidget, Ui::ClockWidget
{
    Q_OBJECT

    public:
        explicit ClockWidget(QWidget* parent = 0);

    private:
        Q_SLOT void updateTime();
};
