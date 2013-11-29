#include "OscTimeWidget.h"

#include "Global.h"

#include "DatabaseManager.h"

#include <QtCore/QDateTime>
#include <QtCore/QTimer>
#include <QtCore/QDebug>

OscTimeWidget::OscTimeWidget(QWidget* parent)
    : QWidget(parent),
      fps(0), paused(false), timestamp(0), startTime(""), reverseOscTime(false)
{
    setupUi(this);

    this->setVisible(false);
    this->progressBarOscTime->setVisible(false);

    if (DatabaseManager::getInstance().getConfigurationByName("DisableInAndOutPoints").getValue() == "true")
    {
        this->labelOscInTime->setVisible(false);
        this->labelOscOutTime->setVisible(false);
        this->labelOscTime->setFixedWidth(this->labelOscTime->width() + 20);
        this->labelOscTime->setGeometry(this->labelOscTime->geometry().x() - 10, this->labelOscTime->geometry().y(),
                                        this->labelOscTime->geometry().width(), this->labelOscTime->geometry().height());
    }
}

void OscTimeWidget::reset()
{
    this->timestamp = 0;
    this->progressBarOscTime->reset();

    if (this->startTime.isEmpty())
    {
        this->setVisible(false);
        this->labelOscTime->setText(Osc::DEFAULT_TIME);
    }
    else
    {
        this->progressBarOscTime->setVisible(false);
        this->labelOscTime->setText(this->startTime);
    }
}

void OscTimeWidget::setTime(int currentFrame)
{
    if (this->fps == 0)
        return;

    this->setVisible(true);

    double currentTime = currentFrame * (1.0 / this->fps);

    this->labelOscTime->setText(convertToTimecode(currentTime));

    if (this->timestamp == 0) // First time.
        QTimer::singleShot(200, this, SLOT(checkState()));

    this->timestamp = QDateTime::currentMSecsSinceEpoch();
}

void OscTimeWidget::setStartTime(const QString& startTime, bool reverseOscTime)
{
    if (startTime.isEmpty())
        return;

    this->startTime = startTime;
    this->reverseOscTime = reverseOscTime;
    this->labelOscTime->setText(this->startTime);

    if (this->reverseOscTime)
        this->progressBarOscTime->setValue(this->progressBarOscTime->maximum());

    this->setVisible(true);
}

void OscTimeWidget::setInOutTime(int seek, int length)
{
    if (this->fps == 0)
        return;

    this->setVisible(true);

    double inTime = seek * (1.0 / this->fps);
    double outTime = (seek + length) * (1.0 / this->fps);

    this->labelOscInTime->setText(convertToTimecode(inTime));
    this->labelOscOutTime->setText(convertToTimecode(outTime));

    this->progressBarOscTime->setRange(seek, seek + length);
}

void OscTimeWidget::setProgress(int currentFrame)
{
    if (this->fps == 0)
        return;

    this->setVisible(true);

    this->progressBarOscTime->setVisible(true);
    this->progressBarOscTime->setValue(currentFrame);
    this->progressBarOscTime->update();
}

void OscTimeWidget::setFramesPerSecond(int fps)
{
    this->fps = fps;
}

void OscTimeWidget::setPaused(bool paused)
{
    // Indicate paused state. Border change?
}

void OscTimeWidget::setLoop(bool loop)
{
    // Indicate loop state.
}

QString OscTimeWidget::convertToTimecode(double time)
{
    int hour;
    int minutes;
    int seconds;
    int frames;

    QString smpteFormat;

    hour = time / 3600;
    minutes = (time - hour * 3600) / 60;
    seconds = time - hour * 3600 - minutes * 60;
    frames = (time - hour * 3600 - minutes * 60 - seconds) * this->fps;

    return smpteFormat.sprintf("%02d:%02d:%02d:%02d", hour, minutes, seconds, frames);
}

void OscTimeWidget::checkState()
{
    qint64 currentTimestamp = QDateTime::currentMSecsSinceEpoch();

    if (!this->paused && (currentTimestamp - this->timestamp) >= 200)
    {
        reset();

        return;
    }

    QTimer::singleShot(200, this, SLOT(checkState()));
}

void OscTimeWidget::setCompactView(bool compactView)
{
    if (compactView)
    {
        this->labelOscTime->setVisible(!compactView);
        this->labelOscInTime->setVisible(!compactView);
        this->labelOscOutTime->setVisible(!compactView);

        this->progressBarOscTime->setFixedHeight(Osc::COMPACT_PROGRESSBAR_HEIGHT);
        this->progressBarOscTime->move(Osc::COMPACT_PROGRESSBAR_X, Osc::COMPACT_PROGRESSBAR_Y);
    }
    else
    {
        this->labelOscTime->setVisible(!compactView);
        if (DatabaseManager::getInstance().getConfigurationByName("DisableInAndOutPoints").getValue() == "true")
        {
            this->labelOscInTime->setVisible(compactView);
            this->labelOscOutTime->setVisible(compactView);
        }
        else
        {
            this->labelOscInTime->setVisible(!compactView);
            this->labelOscOutTime->setVisible(!compactView);
        }

        this->progressBarOscTime->setFixedHeight(Osc::DEFAULT_PROGRESSBAR_HEIGHT);
        this->progressBarOscTime->move(Osc::DEFAULT_PROGRESSBAR_X, Osc::DEFAULT_PROGRESSBAR_Y);
    }
}
