#include "CasparMedia.h"

CasparMedia::CasparMedia(const QString& name, const QString& type)
    : name(name), type(type)
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
