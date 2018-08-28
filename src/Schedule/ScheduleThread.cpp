#include "ScheduleThread.h"

ScheduleThread::ScheduleThread(ScheduleWorkerBase* worker, QObject* parent)
    : worker(worker), QThread(parent)
{

}

void ScheduleThread::run()
{
    emit this->worker->scheduleMainWorker();
}

void ScheduleThread::stop()
{
    emit this->worker->stopProcessing();
}
