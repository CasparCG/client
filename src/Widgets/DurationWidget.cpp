#include "DurationWidget.h"

#include "Global.h"

#include "EventManager.h"
#include "Timecode.h"

#include <QtCore/QDebug>
#include <QtCore/QTimer>

DurationWidget::DurationWidget(QWidget* parent)
    : QWidget(parent),
      duration(0)
{
    setupUi(this);

    this->lcdNumber->display("00:00:00:00");

    this->updateTimer.setInterval(50);

    QObject::connect(&this->updateTimer, SIGNAL(timeout()), this, SLOT(updateTime()));
    QObject::connect(&EventManager::getInstance(), SIGNAL(durationChanged(const DurationChangedEvent&)), this, SLOT(durationChanged(const DurationChangedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(executePlayoutCommand(const ExecutePlayoutCommandEvent&)), this, SLOT(executePlayoutCommand(const ExecutePlayoutCommandEvent&)));
}

void DurationWidget::executePlayoutCommand(const ExecutePlayoutCommandEvent& event)
{
    if (event.getKey() == Qt::Key_F1 ||     // Stop.
        event.getKey() == Qt::Key_F10 ||    // Clear.
        event.getKey() == Qt::Key_F11 ||    // Clear videolayer.
        event.getKey() == Qt::Key_F12)      // Clear channel.
    {
        resetDuration();
    }
}

void DurationWidget::durationChanged(const DurationChangedEvent& event)
{
    resetDuration();

    if (event.getDuration() == 0)
        return;

    this->duration = event.getDuration();

    this->progressBarDuration->setMaximum(this->duration);
    this->progressBarDuration->setValue(this->duration);

    this->time = QTime::fromString("00:00:00:00").addMSecs(this->duration);

    this->labelDuration->setText(Timecode::fromTime(this->time));

    this->updateTimer.start(this->updateTimer.interval());
}

void DurationWidget::updateTime()
{
    this->duration = this->duration - this->updateTimer.interval();
    if (this->duration <= 0)
    {
        resetDuration();

        return;
    }

    this->time = this->time.addMSecs(-this->updateTimer.interval());

    QString text = Timecode::fromTime(this->time);

    this->lcdNumber->display(text);
    this->progressBarDuration->setValue(this->duration);
}

void DurationWidget::resetDuration()
{
    this->updateTimer.stop();

    this->lcdNumber->display("00:00:00:00");
    this->progressBarDuration->setValue(0);
    this->labelDuration->setText("00:00:00:00");
}
