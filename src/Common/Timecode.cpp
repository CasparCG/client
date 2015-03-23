#include "Timecode.h"

#include <QtCore/QString>

QString Timecode::fromTime(const QTime& time)
{
    QString result = time.toString("hh:mm:ss").append(":");

    int msec = time.msec() / 10;
    if (msec < 10)
        result.append("0");

    return result.append(QString("%1").arg(msec));
}

QString Timecode::fromTime(double time, int fps)
{
    int hour;
    int minutes;
    int seconds;
    int frames;

    QString smpteFormat;

    hour = (int)(time / 3600);
    minutes = (int)((time - hour * 3600) / 60);
    seconds = (int)(time - hour * 3600 - minutes * 60);
    frames = (int)((time - hour * 3600 - minutes * 60 - seconds) * fps);

    return smpteFormat.sprintf("%02d:%02d:%02d:%02d", hour, minutes, seconds, frames);
}
