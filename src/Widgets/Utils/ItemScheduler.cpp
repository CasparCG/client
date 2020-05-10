#include "ItemScheduler.h"

#include "Global.h"

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QDebug>

ItemScheduler::ItemScheduler(QObject *parent) : QObject(parent)
{
    this->playTimer.setSingleShot(true);
    this->playTimer.setTimerType(Utils::DEFAULT_TIMER_TYPE);

    this->stopTimer.setSingleShot(true);
    this->stopTimer.setTimerType(Utils::DEFAULT_TIMER_TYPE);

    this->updateTimer.setSingleShot(true);
    this->updateTimer.setTimerType(Utils::DEFAULT_TIMER_TYPE);

    QObject::connect(&this->playTimer, SIGNAL(timeout()), SIGNAL(executePlay()));
    QObject::connect(&this->stopTimer, SIGNAL(timeout()), SIGNAL(executeStop()));
    QObject::connect(&this->updateTimer, SIGNAL(timeout()), SIGNAL(executeUpdate()));
}

int ItemScheduler::getMilliseconds(int frames, double framesPerSecond)
{
    return static_cast<int>(frames * (1000.0 / framesPerSecond));
}

void ItemScheduler::schedulePlayAndStop(int delay, int duration, const QString& delayType, int framesPerSecond)
{
    // Stop all timers.
    this->cancel();

    int delayInMilliseconds = 0;
    int durationInMilliseconds = 0;
    if (delayType == Output::DEFAULT_DELAY_IN_FRAMES)
    {
        if (framesPerSecond > 0)
        {
            delayInMilliseconds = getMilliseconds(delay, framesPerSecond);
            durationInMilliseconds = getMilliseconds(duration, framesPerSecond);
        }
        else
            qCritical("When delay type is frames, fps must be specified");
    }
    else if (delayType == Output::DEFAULT_DELAY_IN_MILLISECONDS)
    {
        delayInMilliseconds = delay;
        durationInMilliseconds = duration;
    }
    else
        qCritical("Unsupported delay type %s", qPrintable(delayType));

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

    int delayInMilliseconds = 0;
    if (delayType == Output::DEFAULT_DELAY_IN_FRAMES)
    {
        if (framesPerSecond > 0)
            delayInMilliseconds = getMilliseconds(delay, framesPerSecond);
        else
            qCritical("When delay type is frames, fps must be specified");
    }
    else if (delayType == Output::DEFAULT_DELAY_IN_MILLISECONDS)
        delayInMilliseconds = delay;
    else
        qCritical("Unsupported delay type %s", qPrintable(delayType));

    this->updateTimer.setInterval(delayInMilliseconds);
    this->updateTimer.start();
}

void ItemScheduler::cancel()
{
    this->playTimer.stop();
    this->stopTimer.stop();
    this->updateTimer.stop();
}
