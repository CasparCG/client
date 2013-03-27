#include "CasparThumbnail.h"

CasparThumbnail::CasparThumbnail(const QString& name, const QString& timestamp, const QString& size)
    : name(name), timestamp(timestamp), size(size)
{
}

const QString& CasparThumbnail::getName() const
{
    return this->name;
}

const QString& CasparThumbnail::getTimestamp() const
{
    return this->timestamp;
}

const QString& CasparThumbnail::getSize() const
{
    return this->size;
}
