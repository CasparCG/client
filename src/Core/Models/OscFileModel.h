#pragma once

#include "../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT OscFileModel
{
    public:
        explicit OscFileModel();

        double getClip() const;
        void setClip(double currentClip);

        double getTotalClip() const;
        void setTotalClip(double totalClip);

        double getTime() const;
        void setTime(double currentTime);

        double getTotalTime() const;
        void setTotalTime(double totalTime);

        double getFramesPerSecond() const;
        void setFramesPerSecond(double fps);

        const QString& getName() const;
        void setName(const QString& name);

    private:
        double currentTime;
        double totalTime;
        double currentClip;
        double totalClip;
        double fps;
        QString name;
};


