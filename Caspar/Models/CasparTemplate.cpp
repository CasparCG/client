#include "CasparTemplate.h"

CasparTemplate::CasparTemplate(const QString& name)
    : name(name)
{
}

const QString& CasparTemplate::getName() const
{
    return this->name;
}
