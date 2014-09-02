#pragma once

#include "Shared.h"
#include "ui_OscTimeWidget.h"

#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

class WIDGETS_EXPORT OscTimeWidget : public QWidget, Ui::OscTimeWidget
{
    Q_OBJECT

    public:
        explicit OscTimeWidget(QWidget* parent = 0);

        void reset();
        void setTime(int currentFrame);
        void setStartTime(const QString& startTime, bool reverseOscTime);
        void setInOutTime(int seek, int length);
        void setProgress(int currentFrame);
        void setFramesPerSecond(int fps);
        void setPaused(bool paused);
        void setLoop(bool loop);

        void setCompactView(bool compactView);

    private:
        int fps;
        bool paused;
        qint64 timestamp;
        QString startTime;
        bool reverseOscTime;

        QString convertToTimecode(double time);

        Q_SLOT void checkState();
};
