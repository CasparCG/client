#pragma once

#include "../Shared.h"
#include "ui_ActionWidget.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT ActionWidget : public QWidget, Ui::ActionWidget
{
    Q_OBJECT

    public:
        explicit ActionWidget(QWidget* parent = 0);
};
