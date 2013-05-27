#pragma once

#include "Shared.h"
#include "ui_TimeWidget.h"

#include <QtGui/QImage>
#include <QtGui/QWidget>

class WIDGETS_EXPORT TimeWidget : public QWidget, Ui::TimeWidget
{
    Q_OBJECT

    public:
        explicit TimeWidget(QWidget* parent = 0);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        Q_SLOT void updateTime();
};
