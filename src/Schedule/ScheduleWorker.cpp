#include "ScheduleWorker.h"

ScheduleWorker::ScheduleWorker(QObject* parent)
    : ScheduleWorkerBase(parent)
{
    this->thread = new ScheduleThread(this, this);
}

ScheduleWorker::~ScheduleWorker()
{
    if (this->thread != nullptr)
    {
        this->thread->stop();
        this->thread->wait();
    }
}

void ScheduleWorker::start()
{
    this->thread->start();
}
