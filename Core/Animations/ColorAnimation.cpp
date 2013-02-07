#include "ColorAnimation.h"
#include "QDebug"

ColorAnimation::ColorAnimation(QWidget* target, QObject* parent)
    : QObject(parent),
    value(255), target(target), animation(NULL)
{
    this->animation = new QPropertyAnimation(this, "color");
    this->animation->setDuration(350);
    this->animation->setKeyValueAt(0, 0);
    this->animation->setKeyValueAt(1, 255);
}

void ColorAnimation::start(int loopCount)
{
    this->animation->setLoopCount(loopCount);
    this->animation->start();
}

void ColorAnimation::stop()
{
    this->animation->stop();
}

int ColorAnimation::color() const
{
    return this->value;
}

void ColorAnimation::setColor(const int value)
{
    this->value = value;

    this->target->setStyleSheet(QString("background-color: rgba(%1, %2, 0, 255);").arg(255 - this->value).arg(this->value));
}
