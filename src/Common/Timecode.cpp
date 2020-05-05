#include "Timecode.h"

#include <QtCore/QString>

QString Timecode::fromTime(const QTime& time, bool useDropFrameNotation)
{
    QString result = time.toString("hh:mm:ss").append((useDropFrameNotation == true) ? "." : ":");

    int msec = time.msec() / 10;
    if (msec < 10)
        result.append("0");

    return result.append(QString("%1").arg(msec));
}

QString Timecode::fromTime(double time, double fps, bool useDropFrameNotation)
{
    int hour;
    int minutes;
    int seconds;
    int frames;

    hour = (int)(time / 3600);
    minutes = (int)((time - hour * 3600) / 60);
    seconds = (int)(time - hour * 3600 - minutes * 60);
    frames = (int)((time - hour * 3600 - minutes * 60 - seconds) * fps);

    return QString("%1%2%3%4%5").arg(hour, 2, 16)
                                .arg(minutes, 2, 16)
                                .arg(seconds, 2, 16)
                                .arg(useDropFrameNotation ? "." : ":")
                                .arg(frames, 2, 16);
}
