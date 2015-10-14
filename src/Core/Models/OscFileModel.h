#pragma once

#include "../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT OscFileModel
{
    public:
        explicit OscFileModel();

        int getFrame() const;
        void setFrame(int currentFrame);

        int getTotalFrames() const;
        void setTotalFrames(int totalFrames);

        double getFramesPerSecond() const;
        void setFramesPerSecond(double fps);

        const QString& getPath() const;
        void setPath(const QString& path);

    private:
        int currentFrame;
        int totalFrames;
        double fps;
        QString path;
};


