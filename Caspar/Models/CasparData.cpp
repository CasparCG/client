#include "CasparData.h"

CasparData::CasparData(const QString& name)
    : name(name)
{
}

const QString& CasparData::getName() const
{
    return this->name;
}
