#include "CasparMedia.h"

CasparMedia::CasparMedia(const QString& name, const QString& type, const QString& timecode)
    : name(name), type(type), timecode(timecode)
{
}

const QString& CasparMedia::getName() const
{
    return this->name;
}

const QString& CasparMedia::getType() const
{
    return this->type;
}

const QString& CasparMedia::getTimecode() const
{
    return this->timecode;
}
