#include "ActiveAnimation.h"

#include <QtCore/QPropertyAnimation>

#include <QtWidgets/QWidget>

ActiveAnimation::ActiveAnimation(QWidget* target, QObject* parent)
    : QObject(parent), target(target)
{
    this->animation = new QPropertyAnimation(this, "color");
    this->animation->setDuration(350);
    this->animation->setKeyValueAt(0, 255);
    this->animation->setKeyValueAt(1, 0);
}

void ActiveAnimation::start(int loopCount)
{
    this->animation->setLoopCount(loopCount);
    this->animation->start();
}

void ActiveAnimation::stop()
{
    this->animation->stop();
}

int ActiveAnimation::color() const
{
    return this->value;
}

void ActiveAnimation::setColor(const int value)
{
    this->value = value;

    this->target->setStyleSheet(QString("background-color: rgba(%1, %2, 0, 255);").arg(255 - this->value).arg(this->value));
}
