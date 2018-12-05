#pragma once

#include "Shared.h"
#include "ui_OscTimeWidget.h"

#include <QtWidgets/QWidget>

class WIDGETS_EXPORT OscTimeWidget : public QWidget, Ui::OscTimeWidget
{
    Q_OBJECT

    public:
        explicit OscTimeWidget(QWidget* parent = 0);

        void reset();
        void setTime(double currentTime);
        void setStartTime(const QString& startTime, bool reverseOscTime);
        void setInOutTime(double inTime, double outTime);
        void setProgress(double currentTime);
        void setFramesPerSecond(double fps);
        void setPaused(bool paused);
        void setLoop(bool loop);
        void setRecording(bool enabled);
        void setRecordOnly(bool enabled);

        void setCompactView(bool compactView);

    private:
        double fps = 0;
        bool paused = false;
        bool recordOnly = false;
        bool reverseOscTime = false;
        bool useDropFrameNotation = false;

        qint64 timestamp = 0;
        QString startTime = "";

        Q_SLOT void checkState();
};
