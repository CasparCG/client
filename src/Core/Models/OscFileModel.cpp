#include "OscFileModel.h"

OscFileModel::OscFileModel()
    : currentTime(0), totalTime(0), currentFrame(0), totalFrames(0), fps(0), path("")
{    
}

double OscFileModel::getTime() const
{
    return this->currentTime;
}

void OscFileModel::setTime(double currentTime)
{
    this->currentTime = currentTime;
}

double OscFileModel::getTotalTime() const
{
    return this->totalTime;
}

void OscFileModel::setTotalTime(double totalTime)
{
    this->totalTime = totalTime;
}

int OscFileModel::getFrame() const
{
    return this->currentFrame;
}

void OscFileModel::setFrame(int currentFrame)
{
    this->currentFrame = currentFrame;
}

int OscFileModel::getTotalFrames() const
{
    return this->totalFrames;
}

void OscFileModel::setTotalFrames(int totalFrames)
{
    this->totalFrames = totalFrames;
}

int OscFileModel::getFramesPerSecond() const
{
    return this->fps;
}

void OscFileModel::setFramesPerSecond(int fps)
{
    this->fps = fps;
}

const QString& OscFileModel::getPath() const
{
    return this->path;
}

void OscFileModel::setPath(const QString& path)
{
    this->path = path;
}
