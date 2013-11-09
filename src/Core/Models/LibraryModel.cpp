#include "LibraryModel.h"

LibraryModel::LibraryModel(int id, const QString& label, const QString& name, const QString& deviceName, const QString& type, int thumbnailId, const QString& timecode)
    : id(id), label(label), name(name), deviceName(deviceName), type(type), thumbnailId(thumbnailId), timecode(timecode)
{
}

int LibraryModel::getId() const
{
    return this->id;
}

const QString& LibraryModel::getLabel() const
{
    return this->label;
}

const QString& LibraryModel::getName() const
{
    return this->name;
}

const QString& LibraryModel::getDeviceName() const
{
    return this->deviceName;
}

const QString& LibraryModel::getType() const
{
    return this->type;
}

const QString& LibraryModel::getTimecode() const
{
    return this->timecode;
}

void LibraryModel::setLabel(const QString& label)
{
    this->label = label;
}

void LibraryModel::setName(const QString& name)
{
    this->name = name;
}

void LibraryModel::setDeviceName(const QString& deviceName)
{
    this->deviceName = deviceName;
}

void LibraryModel::setTimecode(const QString& timecode)
{
    this->timecode = timecode;
}

int LibraryModel::getThumbnailId() const
{
    return this->thumbnailId;
}
