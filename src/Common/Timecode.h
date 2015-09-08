#pragma once

#include "Shared.h"

#include <QtCore/QString>
#include <QtCore/QTime>

class COMMON_EXPORT Timecode
{
    public:
        static QString fromTime(const QTime& time, bool useDropFrameNotation);
        static QString fromTime(double time, double fps, bool useDropFrameNotation);

    private:
        Timecode() {}
};
