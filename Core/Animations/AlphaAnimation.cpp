#include "AlphaAnimation.h"
#include "QDebug"

AlphaAnimation::AlphaAnimation(QWidget* target, QObject* parent)
    : QObject(parent),
    value(255), target(target), animation(NULL)
{
    this->animation = new QPropertyAnimation(this, "alpha");
    this->animation->setDuration(500);
    this->animation->setKeyValueAt(0, 0);
    //this->animation->setKeyValueAt(0.5, 192); // 192
    //this->animation->setKeyValueAt(0.6, 255); // 192
    this->animation->setKeyValueAt(1, 255);
}

void AlphaAnimation::start(int loopCount)
{
    //this->animation->setLoopCount(loopCount);
    this->animation->start();
}

int AlphaAnimation::alpha() const
{
    return this->value;
}

void AlphaAnimation::setAlpha(const int value)
{
    this->value = value;

    qDebug() << QString("background-color: rgb(%1, %2, 0);").arg(255 - this->value).arg(this->value);
    QString stylesheet(QString("background-color: rgb(%1, %2, 0);").arg(255 - this->value).arg(this->value));
    this->target->setStyleSheet(stylesheet);
}
