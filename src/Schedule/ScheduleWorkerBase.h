#pragma once

#include "Shared.h"

#include <QtCore/QObject>

class SCHEDULE_EXPORT ScheduleWorkerBase : public QObject
{
    Q_OBJECT

    public:
        explicit ScheduleWorkerBase(QObject* parent = 0);

        Q_SIGNAL void scheduleMainWorker();
        Q_SIGNAL void stopProcessing();
};
