#pragma once

#include "Shared.h"
#include "ui_OscTimeWidget.h"

#include <QtGui/QWidget>

class WIDGETS_EXPORT OscTimeWidget : public QWidget, Ui::OscTimeWidget
{
    Q_OBJECT

    public:
        explicit OscTimeWidget(QWidget* parent = 0);

        void reset();
        void setTime(int currentFrame);
        void setInOutTime(int seek, int length);
        void setProgress(int currentFrame);
        void setFramesPerSecond(int fps);
        void setPaused(bool paused);
        void setLoop(bool loop);

    private:
        int fps;
        bool paused;
        qint64 timestamp;

        QString convertToTimecode(double time);

        Q_SLOT void checkState();
};
