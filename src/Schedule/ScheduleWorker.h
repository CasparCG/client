#pragma once

#include "Shared.h"

#include "ScheduleThread.h"
#include "ScheduleWorkerBase.h"

#include <QtCore/QObject>

class SCHEDULE_EXPORT ScheduleWorker : public ScheduleWorkerBase
{
    public:
        explicit ScheduleWorker(QObject* parent = 0);
        ~ScheduleWorker();

        void start();

    private:
        ScheduleThread* thread = nullptr;

};

