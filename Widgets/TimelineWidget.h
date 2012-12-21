#pragma once

#include "Shared.h"
#include "ui_TimelineWidget.h"

#include <QtGui/QWidget>

class WIDGETS_EXPORT TimelineWidget : public QWidget, Ui::TimelineWidget
{
    Q_OBJECT

    public:
        explicit TimelineWidget(QWidget* parent = 0);
};
