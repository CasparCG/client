#include "FormatModel.h"

FormatModel::FormatModel(int id, const QString& name, int width, int height, const QString& fps)
    : id(id), name(name), width(width), height(height), fps(fps)
{    
}

int FormatModel::getId() const
{
    return this->id;
}

const QString& FormatModel::getName() const
{
    return this->name;
}

int FormatModel::getWidth() const
{
    return this->width;
}

int FormatModel::getHeight() const
{
    return this->height;
}

const QString& FormatModel::getFramesPerSecond() const
{
    return this->fps;
}
