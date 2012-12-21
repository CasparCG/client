#include "ClockWidget.h"

#include <QtCore/QTime>
#include <QtCore/QTimer>

ClockWidget::ClockWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUi(this);

    this->lcdNumber->hide();

    QTimer* timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    //timer->start(40);
}

void ClockWidget::updateTime()
{
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm:ss").append(":");

    int frame = time.msec() / 40;
    if (frame < 10)
        text.append("0");

    text.append(QString("%1").arg(frame));

    this->lcdNumber->display(text);
    if (this->lcdNumber->isHidden())
        this->lcdNumber->show();
}
