#include "OscTimeWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "Timecode.h"

#include <QtCore/QDateTime>
#include <QtCore/QTimer>
#include <QtCore/QDebug>

OscTimeWidget::OscTimeWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUi(this);

    this->setVisible(false);
    this->progressBarOscTime->setVisible(false);
    if (DatabaseManager::getInstance().getConfigurationByName("DisableInAndOutPoints").getValue() == "true")
    {
        this->labelOscInTime->setVisible(false);
        this->labelOscOutTime->setVisible(false);
    }

    this->useDropFrameNotation = (DatabaseManager::getInstance().getConfigurationByName("UseDropFrameNotation").getValue() == "true") ? true : false;
    this->labelOscTime->setText(QString("00:00:00").append((this->useDropFrameNotation == true) ? ".00" : ":00"));
    this->labelOscInTime->setText(QString("00:00:00").append((this->useDropFrameNotation == true) ? ".00" : ":00"));
    this->labelOscOutTime->setText(QString("00:00:00").append((this->useDropFrameNotation == true) ? ".00" : ":00"));

#if defined(Q_OS_MAC)
    this->labelOscTime->setGeometry(this->labelOscTime->geometry().x() + 2, this->labelOscTime->geometry().y() + 1,
                                    this->labelOscTime->geometry().width(), this->labelOscTime->geometry().height());
    this->labelOscInTime->setGeometry(this->labelOscInTime->geometry().x() - 2, this->labelOscInTime->geometry().y(),
                                    this->labelOscInTime->geometry().width(), this->labelOscInTime->geometry().height());
    this->labelOscOutTime->setGeometry(this->labelOscOutTime->geometry().x() + 2, this->labelOscOutTime->geometry().y(),
                                    this->labelOscOutTime->geometry().width(), this->labelOscOutTime->geometry().height());
    this->progressBarOscTime->setGeometry(this->progressBarOscTime->geometry().x(), this->progressBarOscTime->geometry().y(),
                                    this->progressBarOscTime->geometry().width() - 7, this->progressBarOscTime->geometry().height());
#endif
}

void OscTimeWidget::setRecording(bool enabled)
{
    if (enabled)
        this->labelOscTime->setStyleSheet("color: red;");
    else
        this->labelOscTime->setStyleSheet("");
}

void OscTimeWidget::setRecordOnly(bool enabled)
{
    this->recordOnly = enabled;

    this->labelOscInTime->setVisible(!enabled);
    this->labelOscOutTime->setVisible(!enabled);
    this->progressBarOscTime->setVisible(!enabled);
}

void OscTimeWidget::reset()
{
    if (this->paused)
        return;

    this->fps = 0;
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

void OscTimeWidget::setTime(double currentTime)
{
    if (this->fps == 0)
        return;

    this->setVisible(true);

    this->labelOscTime->setText(Timecode::fromTime(currentTime, this->fps, this->useDropFrameNotation));

    if (!this->recordOnly)
    {
        if (this->timestamp == 0) // First time.
            QTimer::singleShot(500, this, SLOT(checkState()));

        this->timestamp = QDateTime::currentMSecsSinceEpoch();
    }
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

void OscTimeWidget::setInOutTime(double inTime, double outTime)
{
    if (this->fps == 0)
        return;

    this->setVisible(true);

    this->labelOscInTime->setText(Timecode::fromTime(inTime, this->fps, this->useDropFrameNotation));
    this->labelOscOutTime->setText(Timecode::fromTime(outTime, this->fps, this->useDropFrameNotation));

    this->progressBarOscTime->setRange(inTime * this->fps, outTime * this->fps);
}

void OscTimeWidget::setProgress(double currentTime)
{
    if (this->fps == 0)
        return;

    this->setVisible(true);

    this->progressBarOscTime->setVisible(true);
    this->progressBarOscTime->setValue(currentTime * this->fps);
    this->progressBarOscTime->update();
}

void OscTimeWidget::setFramesPerSecond(double fps)
{
    this->fps = fps;
}

void OscTimeWidget::setPaused(bool paused)
{  
    if (this->fps == 0)
        return; // We are not playing.

    this->paused = paused;

    if (this->paused && !this->progressBarOscTime->isVisible())
        this->progressBarOscTime->setVisible(true);
}

void OscTimeWidget::setLoop(bool loop)
{
    Q_UNUSED(loop);
}

void OscTimeWidget::checkState()
{
    qint64 currentTimestamp = QDateTime::currentMSecsSinceEpoch();

    if (!this->paused && (currentTimestamp - this->timestamp) >= 500)
    {
        reset();

        return;
    }

    QTimer::singleShot(500, this, SLOT(checkState()));
}

void OscTimeWidget::setCompactView(bool compactView)
{
    if (compactView)
    {
        this->labelOscTime->setVisible(!compactView);
        this->labelOscInTime->setVisible(!compactView);
        this->labelOscOutTime->setVisible(!compactView);

        this->progressBarOscTime->setFixedHeight(Osc::COMPACT_PROGRESSBAR_HEIGHT);
#if defined(Q_OS_WIN)
        this->progressBarOscTime->move(Osc::COMPACT_PROGRESSBAR_X, Osc::COMPACT_PROGRESSBAR_Y);
#elif defined(Q_OS_MAC)
        this->progressBarOscTime->move(Osc::COMPACT_PROGRESSBAR_X + 5, Osc::COMPACT_PROGRESSBAR_Y);
#endif
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
#if defined(Q_OS_WIN)
        this->progressBarOscTime->move(Osc::DEFAULT_PROGRESSBAR_X, Osc::DEFAULT_PROGRESSBAR_Y);
#elif defined(Q_OS_MAC)
        this->progressBarOscTime->move(Osc::DEFAULT_PROGRESSBAR_X + 5, Osc::DEFAULT_PROGRESSBAR_Y);
#endif
    }
}
