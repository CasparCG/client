#include "OscFileModel.h"

OscFileModel::OscFileModel()
    : currentTime(0), totalTime(0), currentClip(0), totalClip(0), fps(0), name("")
{    
}

double OscFileModel::getClip() const
{
    return this->currentClip;
}

void OscFileModel::setClip(double currentClip)
{
    this->currentClip = currentClip;
}

double OscFileModel::getTotalClip() const
{
    return this->totalClip;
}

void OscFileModel::setTotalClip(double totalClip)
{
    this->totalClip = totalClip;
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

double OscFileModel::getFramesPerSecond() const
{
    return this->fps;
}

void OscFileModel::setFramesPerSecond(double fps)
{
    this->fps = fps;
}

const QString& OscFileModel::getName() const
{
    return this->name;
}

void OscFileModel::setName(const QString& name)
{
    this->name = name;
}
