#include "OscFileModel.h"

OscFileModel::OscFileModel()
    : currentFrame(0), totalFrames(0), fps(0), path("")
{    
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

double OscFileModel::getFramesPerSecond() const
{
    return this->fps;
}

void OscFileModel::setFramesPerSecond(double fps)
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
