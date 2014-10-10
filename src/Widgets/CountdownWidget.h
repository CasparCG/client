#pragma once

#include "Shared.h"
#include "ui_CountdownWidget.h"

#include "Events/CountdownChangedEvent.h"
#include "Events/Rundown/ExecutePlayoutCommandEvent.h"

#include <QtCore/QTime>
#include <QtCore/QTimer>

#include <QtGui/QWidget>

class WIDGETS_EXPORT CountdownWidget : public QWidget, Ui::CountdownWidget
{
    Q_OBJECT

    public:
        explicit CountdownWidget(QWidget* parent = 0);

    private:
        int countdown;
        QTime time;
        QTimer updateTimer;

        void resetCounter();
        QString getTimecode();

        Q_SLOT void updateTime();
        Q_SLOT void countdownChanged(const CountdownChangedEvent&);
        Q_SLOT void executePlayoutCommand(const ExecutePlayoutCommandEvent&);
};
