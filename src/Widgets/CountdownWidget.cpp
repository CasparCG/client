#include "CountdownWidget.h"

#include "Global.h"

#include "EventManager.h"

#include <QtCore/QDebug>
#include <QtCore/QTimer>

CountdownWidget::CountdownWidget(QWidget* parent)
    : QWidget(parent),
      countdown(0)
{
    setupUi(this);

    this->lcdNumber->display("00:00:00:00");

    this->updateTimer.setInterval(50);

    QObject::connect(&this->updateTimer, SIGNAL(timeout()), this, SLOT(updateTime()));
    QObject::connect(&EventManager::getInstance(), SIGNAL(countdownChanged(const CountdownChangedEvent&)), this, SLOT(countdownChanged(const CountdownChangedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(executePlayoutCommand(const ExecutePlayoutCommandEvent&)), this, SLOT(executePlayoutCommand(const ExecutePlayoutCommandEvent&)));
}

void CountdownWidget::executePlayoutCommand(const ExecutePlayoutCommandEvent& event)
{
    if (event.getKey() == Qt::Key_F1 ||     // Stop.
        event.getKey() == Qt::Key_F10 ||    // Clear.
        event.getKey() == Qt::Key_F11 ||    // Clear videolayer.
        event.getKey() == Qt::Key_F12)      // Clear channel.
    {
        resetCounter();
    }
}

void CountdownWidget::countdownChanged(const CountdownChangedEvent& event)
{
    resetCounter();

    if (event.getCountdownTime().toInt() == 0)
        return;

    this->countdown = event.getCountdownTime().toInt();

    this->progressBarCountdown->setMaximum(this->countdown);
    this->progressBarCountdown->setValue(this->countdown);

    this->time = QTime::fromString("00:00:00:00").addMSecs(this->countdown);

    this->labelCountdown->setText(getTimecode());

    this->updateTimer.start(this->updateTimer.interval());
}

void CountdownWidget::updateTime()
{
    this->countdown = this->countdown - this->updateTimer.interval();
    if (this->countdown <= 0)
    {
        resetCounter();

        return;
    }

    this->time = this->time.addMSecs(-this->updateTimer.interval());

    QString text = getTimecode();

    this->lcdNumber->display(text);
    this->progressBarCountdown->setValue(this->countdown);

    qDebug() << text;
}

QString CountdownWidget::getTimecode()
{
    QString text = this->time.toString("hh:mm:ss").append(":");

    int msec = this->time.msec() / 10;
    if (msec < 10)
    text.append("0");

    return text.append(QString("%1").arg(msec));
}

void CountdownWidget::resetCounter()
{
    this->updateTimer.stop();

    this->lcdNumber->display("00:00:00:00");
    this->progressBarCountdown->setValue(0);
    this->labelCountdown->setText("00:00:00:00");
}
