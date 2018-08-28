#include "ScheduleManager.h"

#include "DatabaseManager.h"
#include "EventManager.h"

#include "Events/AddScheduleItemEvent.h"

#include <QtCore/QDateTime>
#include <QtCore/QMetaType>

Q_GLOBAL_STATIC(ScheduleManager, scheduleManager)

ScheduleManager::ScheduleManager()
{

}

ScheduleManager& ScheduleManager::getInstance()
{
    return *scheduleManager();
}

void ScheduleManager::initialize()
{
    this->worker = QSharedPointer<ScheduleWorker>(new ScheduleWorker());
    this->modelList = QSharedPointer< QList<ScheduleModel> >(new QList<ScheduleModel>(DatabaseManager::getInstance().getScheduledProgram()));
    this->playedQuery = QSharedPointer<QList<ScheduleModel>>(new QList<ScheduleModel>());

    qRegisterMetaType<AddScheduleItemEvent>();
    qRegisterMetaType<RestoreBeforeScheduleEvent>();

    QObject::connect(&EventManager::getInstance(), SIGNAL(scheduleChanged(const ScheduleChangedEvent&)),
                     this, SLOT(scheduleUpdated(const ScheduleChangedEvent&)));
    QObject::connect(this->worker.data(), SIGNAL(scheduleMainWorker()), this, SLOT(mainWorker()), Qt::DirectConnection);
    QObject::connect(this->worker.data(), SIGNAL(stopProcessing()), this, SLOT(stopProcessing()), Qt::DirectConnection);

    // Start threading schedule
    this->worker.data()->start();
}

void ScheduleManager::uninitialize()
{
}

const QSharedPointer<ScheduleWorker> ScheduleManager::getScheduleWorker() const
{
    return this->worker;
}

// Why you ask? Read my comment of this function inside the header files...
bool ScheduleManager::isTimeMatch(const QTime &first, const QTime &second)
{
    // Compare the hours first
    if (first.hour() != second.hour())
        return false;

    // Then compare the minutes
    if (first.minute() != second.minute())
        return false;

    // Lastly the seconds
    if (first.second() != second.second())
        return false;

    // And the msecs- I'm sorry for swearing but, f*ck that sh*t of a property
    return true;
}

void ScheduleManager::mainWorker()
{
    this->orderStop = false;

    while (!this->orderStop) {
        {
            QThread::msleep(10);

            // Catch current time for safety, even if there will be slight delay
            QDateTime currentTime = QDateTime::currentDateTime();
            {
                QMutexLocker locker(&eventsMutex);
                foreach (ScheduleModel model, *this->modelList.data())
                {
                    // Wire with Widget responsible for activity commencing
                    if (this->isTimeMatch(currentTime.time(), model.getTimelineBegan().time())){
                        if (model.getActivated() && !this->playedQuery.data()->contains(model)){
                            EventManager::getInstance().fireAddScheduleItemEvent(AddScheduleItemEvent(model));
                            this->playedQuery.data()->push_back(model);
                            qDebug("A preset named %s has been added (in Scheduler Thread)", qPrintable(model.getName()));
                            break;
                        }
                    }
                }
            }

            // Process the end timeline of schedule
            if (!this->playedQuery.data()->isEmpty()) {
                foreach (ScheduleModel model, *this->playedQuery.data())
                {
                    if (this->isTimeMatch(currentTime.time(), model.getTimelineEnd().time())) {
                        this->playedQuery.data()->removeAt(this->playedQuery.data()->indexOf(model));
                    }
                }

                // After-removal check
                if (this->playedQuery.data()->isEmpty())
                    EventManager::getInstance().fireRestoreBeforeScheduleEvent(RestoreBeforeScheduleEvent());
            }
        }
    }
}

void ScheduleManager::stopProcessing()
{
    orderStop = true;
}

void ScheduleManager::scheduleUpdated(const ScheduleChangedEvent &event)
{
    Q_UNUSED(event);
    QList<ScheduleModel> other = DatabaseManager::getInstance().getScheduledProgram();
    {
        QMutexLocker locker(&eventsMutex);
        modelList.data()->clear();
        modelList.data()->swap(other);
    }

    qDebug("Schedule list in schedule query updated.");

}

