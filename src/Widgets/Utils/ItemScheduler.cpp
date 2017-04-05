#include "ItemScheduler.h"

#include "Global.h"

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QDebug>

int getMilliseconds(int frames, double framesPerSecond)
{
    return static_cast<int>(frames * (1000.0 / framesPerSecond));
}

ItemScheduler::ItemScheduler(QObject *parent) : QObject(parent)
{
    this->playTimer.setSingleShot(true);
    this->stopTimer.setSingleShot(true);
    this->updateTimer.setSingleShot(true);

    this->playTimer.setTimerType(Utils::DEFAULT_TIMER_TYPE);
    this->stopTimer.setTimerType(Utils::DEFAULT_TIMER_TYPE);
    this->updateTimer.setTimerType(Utils::DEFAULT_TIMER_TYPE);

    QObject::connect(&this->playTimer, SIGNAL(timeout()), SIGNAL(executePlay()));
    QObject::connect(&this->stopTimer, SIGNAL(timeout()), SIGNAL(executeStop()));
    QObject::connect(&this->updateTimer, SIGNAL(timeout()), SIGNAL(executeUpdate()));
}

ItemScheduler::~ItemScheduler()
{
}

void ItemScheduler::schedulePlayAndStop(int delay, int duration, const QString &delayType, int framesPerSecond)
{
    // Stop all timers
    this->cancel();

    int delayInMilliseconds;
    int durationInMilliseconds;

    if (delayType == Output::DEFAULT_DELAY_IN_FRAMES)
    {
        delayInMilliseconds = getMilliseconds(delay, framesPerSecond);
        durationInMilliseconds = getMilliseconds(duration, framesPerSecond);
    }
    else if (delayType == Output::DEFAULT_DELAY_IN_MILLISECONDS)
    {
        delayInMilliseconds = delay;
        durationInMilliseconds = duration;
    }

    this->playTimer.setInterval(delayInMilliseconds);
    this->playTimer.start();

    if (durationInMilliseconds > 0)
    {
        this->stopTimer.setInterval(delayInMilliseconds + durationInMilliseconds);
        this->stopTimer.start();
    }
}

void ItemScheduler::scheduleUpdate(int delay, const QString& delayType, int framesPerSecond)
{
    this->updateTimer.stop();

    int delayInMilliseconds;

    if (delayType == Output::DEFAULT_DELAY_IN_FRAMES)
    {
        delayInMilliseconds = getMilliseconds(delay, framesPerSecond);
    }
    else if (delayType == Output::DEFAULT_DELAY_IN_MILLISECONDS)
    {
        delayInMilliseconds = delay;
    }

    this->updateTimer.setInterval(delayInMilliseconds);
    this->updateTimer.start();
}

void ItemScheduler::cancel()
{
    this->playTimer.stop();
    this->stopTimer.stop();
    this->updateTimer.stop();
}
