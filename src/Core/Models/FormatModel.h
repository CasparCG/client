#pragma once

#include "../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT FormatModel
{
    public:
        explicit FormatModel(int id, const QString& name, int width, int height, const QString& fps);

        int getId() const;
        const QString& getName() const;
        int getWidth() const;
        int getHeight() const;
        const QString& getFramesPerSecond() const;

    private:
        int id;
        QString name;
        int width;
        int height;
        QString fps;
};


