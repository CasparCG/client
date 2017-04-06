#pragma once

#include "Global.h"

#include <QtCore/QObject>
#include <QtCore/QTimer>

class ItemScheduler : public QObject
{
    Q_OBJECT

    public:
        explicit ItemScheduler(QObject *parent = 0);
        virtual ~ItemScheduler();

        void schedulePlayAndStop(int delay, int duration, const QString& delayType, int framesPerSecond = 0);
        void scheduleUpdate(int delay, const QString& delayType, int framesPerSecond = 0);

        void cancel();

    signals:
        void executePlay();
        void executeStop();
        void executeUpdate();

    private:
        QTimer playTimer;
        QTimer stopTimer;
        QTimer updateTimer;
};
