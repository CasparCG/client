#include "BorderAnimation.h"

#include "Global.h"

BorderAnimation::BorderAnimation(QWidget* target, QObject* parent)
    : QObject(parent),
    value(255), target(target), animation(NULL)
{    
    this->animation = new QPropertyAnimation(this, "alpha");
    this->animation->setDuration(1000);
    this->animation->setKeyValueAt(0, 255);
    this->animation->setKeyValueAt(0.5, 100);
    this->animation->setKeyValueAt(1, 255);
}

void BorderAnimation::start(int loopCount)
{
    this->animation->setLoopCount(loopCount);
    this->animation->start();
}

void BorderAnimation::stop()
{
    this->animation->stop();
    this->target->setStyleSheet(QString("border-color: rgba(%1);").arg(Stylesheet::DEFAULT_BORDER_COLOR));
}

int BorderAnimation::alpha() const
{
    return this->value;
}

void BorderAnimation::setAlpha(const int value)
{
    this->value = value;

    this->target->setStyleSheet(QString("border-color: rgba(255, 0, 0, %1);").arg(this->value));
}
