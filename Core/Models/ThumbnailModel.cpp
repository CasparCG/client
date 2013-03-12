#include "ThumbnailModel.h"

ThumbnailModel::ThumbnailModel(int id, const QString& data, const QString& timestamp, const QString& size, const QString& name, const QString& address)
    : id(id), data(data), timestamp(timestamp), size(size), name(name), address(address)
{
}

int ThumbnailModel::getId() const
{
    return this->id;
}

const QString& ThumbnailModel::getData() const
{
    return this->data;
}

const QString& ThumbnailModel::getTimestamp() const
{
    return this->timestamp;
}

const QString& ThumbnailModel::getSize() const
{
    return this->size;
}

const QString& ThumbnailModel::getName() const
{
    return this->name;
}

const QString& ThumbnailModel::getAddress() const
{
    return this->address;
}
