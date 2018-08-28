#pragma once

#include "ScheduleWorker.h"
#include "Events/ScheduleChangedEvent.h"
#include "Models/ScheduleModel.h"

#include "Shared.h"

#include <QtCore/QThread>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QSharedPointer>
#include <QtCore/QMutex>

class CORE_EXPORT ScheduleManager : public QObject
{
    Q_OBJECT

    public:
        explicit ScheduleManager();

        static ScheduleManager& getInstance();

        void initialize();
        void uninitialize();

        const QSharedPointer<ScheduleWorker> getScheduleWorker() const;

    private:
        QMutex eventsMutex;
        bool orderStop = false;

        QSharedPointer<ScheduleWorker> worker;
        QSharedPointer<QList<ScheduleModel>> modelList;
        QSharedPointer<QList<ScheduleModel>> playedQuery;

        // You wonder why such unnecessary function is implemented here, do you?
        // Well unfortunately, this was necessary, because the operator== within QTime is broken (Don't know why)
        // I was wondering why didn't the time match up when its obviously matching each other
        // (I even debug the time to see if it really did match up)
        // And, it did, but the condition fail, weird right? The documentation tell less than what I imagine could be
        // a missing explanation of what will gonne be compared,
        // and my guess is that 'msecs' are included, which are useless to compare -.-
        // That's why this function was made, to cover the (questionably?) broken operator.
        bool isTimeMatch(const QTime& first, const QTime& second);

        Q_SLOT void mainWorker();
        Q_SLOT void stopProcessing();
        Q_SLOT void scheduleUpdated(const ScheduleChangedEvent& event);

};

