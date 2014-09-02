#pragma once

#include "../Shared.h"
#include "ui_ActionWidget.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

class WIDGETS_EXPORT ActionWidget : public QWidget, Ui::ActionWidget
{
    Q_OBJECT

    public:
        explicit ActionWidget(QWidget* parent = 0);
};
