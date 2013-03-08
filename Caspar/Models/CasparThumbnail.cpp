#include "Models/CasparThumbnail.h"

CasparThumbnail::CasparThumbnail(const QString& name, const QString& timestamp)
    : name(name), timestamp(timestamp)
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
