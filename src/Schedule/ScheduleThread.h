#pragma once

#include "Shared.h"

#include "ScheduleWorkerBase.h"

#include <QtCore/QThread>
#include <QtCore/QObject>

class SCHEDULE_EXPORT ScheduleThread : public QThread
{
    Q_OBJECT

    public:
        explicit ScheduleThread(ScheduleWorkerBase* worker, QObject* parent = 0);

        void stop();

    protected:
        void run();

    private:
        ScheduleWorkerBase* worker;

};

