#pragma once

#include "Shared.h"

#include <QtCore/QString>
#include <QtCore/QTime>

class COMMON_EXPORT Timecode
{
    public:
        static QString fromTime(const QTime& time);
        static QString fromTime(double time, int fps);

    private:
        Timecode() {}
};
