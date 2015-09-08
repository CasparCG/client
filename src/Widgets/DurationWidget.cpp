#include "DurationWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Timecode.h"

#include <QtCore/QDebug>
#include <QtCore/QTimer>

DurationWidget::DurationWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUi(this);

    this->useDropFrameNotation = (DatabaseManager::getInstance().getConfigurationByName("UseDropFrameNotation").getValue() == "true") ? true : false;

    this->updateTimer.setInterval(50);
    this->lcdNumber->display(QString("00:00:00").append((this->useDropFrameNotation == true) ? ".00" : ":00"));
    this->labelZero->setText(QString("00:00:00").append((this->useDropFrameNotation == true) ? ".00" : ":00"));
    this->labelCountdownDuration->setText(QString("00:00:00").append((this->useDropFrameNotation == true) ? ".00" : ":00"));

    QObject::connect(&this->updateTimer, SIGNAL(timeout()), this, SLOT(updateTime()));
    QObject::connect(&EventManager::getInstance(), SIGNAL(durationChanged(const DurationChangedEvent&)), this, SLOT(durationChanged(const DurationChangedEvent&)));
    //QObject::connect(&EventManager::getInstance(), SIGNAL(executePlayoutCommand(const ExecutePlayoutCommandEvent&)), this, SLOT(executePlayoutCommand(const ExecutePlayoutCommandEvent&)));
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

    this->time = QTime::fromString(QString("00:00:00").append((this->useDropFrameNotation == true) ? ".00" : ":00")).addMSecs(this->duration);

    this->labelCountdownDuration->setText(Timecode::fromTime(this->time, this->useDropFrameNotation));

    qDebug("DurationWidget::durationChanged %s", qPrintable(this->labelCountdownDuration->text()));

    this->updateTimer.start();
    this->timeSinceStart.restart();
}

void DurationWidget::updateTime()
{
    if (this->timeSinceStart.elapsed() > this->duration)
    {
        resetDuration();

        return;
    }

    QTime currentTime = this->time.addMSecs(-this->timeSinceStart.elapsed());
    QString timecode = Timecode::fromTime(currentTime, this->useDropFrameNotation);

    this->lcdNumber->display(timecode);
    this->progressBarDuration->setValue(this->duration - this->timeSinceStart.elapsed());
    this->progressBarDuration->update();
}

void DurationWidget::resetDuration()
{
    this->updateTimer.stop();

    this->progressBarDuration->setValue(0);
    this->lcdNumber->display(QString("00:00:00").append((this->useDropFrameNotation == true) ? ".00" : ":00"));
    this->labelCountdownDuration->setText(QString("00:00:00").append((this->useDropFrameNotation == true) ? ".00" : ":00"));
}
