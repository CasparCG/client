#include "AlphaAnimation.h"

AlphaAnimation::AlphaAnimation(QWidget* target, QObject* parent)
    : QObject(parent),
    value(255), target(target), animation(NULL)
{
    this->animation = new QPropertyAnimation(this, "alpha");
    this->animation->setDuration(1000);
    this->animation->setKeyValueAt(0, 0);
    this->animation->setKeyValueAt(0.5, 192);
    this->animation->setKeyValueAt(1, 255);
}

void AlphaAnimation::start(int loopCount)
{
    this->animation->setLoopCount(loopCount);
    this->animation->start();
}

int AlphaAnimation::alpha() const
{
    return this->value;
}

void AlphaAnimation::setAlpha(const int value)
{
    this->value = value;

    QString stylesheet(QString("color: rgba(255, 255, 255, %1);").arg(this->value));
    this->target->setStyleSheet(stylesheet);
}
