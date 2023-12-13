#pragma once

#include "Shared.h"
#include "ui_DurationWidget.h"

#include "Events/DurationChangedEvent.h"
#include "Events/Rundown/ExecutePlayoutCommandEvent.h"

#include <QtCore/QTime>
#include <QtCore/QTimer>
#include <QtCore/QElapsedTimer>
#include <QtCore/QDateTime>

#include <QtWidgets/QWidget>

class WIDGETS_EXPORT DurationWidget : public QWidget, Ui::DurationWidget
{
    Q_OBJECT

    public:
        explicit DurationWidget(QWidget* parent = 0);

    private:
        int duration = 0;
        bool useDropFrameNotation = false;

        QTime time;
        QElapsedTimer timeSinceStart;
        QTimer updateTimer;

        void resetDuration();
        QString getTimecode();

        Q_SLOT void updateTime();
        Q_SLOT void durationChanged(const DurationChangedEvent&);
        Q_SLOT void executePlayoutCommand(const ExecutePlayoutCommandEvent&);
};
