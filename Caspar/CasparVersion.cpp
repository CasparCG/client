#include "CasparVersion.h"

CasparVersion::CasparVersion(const QString& version)
    : version(version)
{
}

const QString& CasparVersion::getVersion() const
{
    return this->version;
}
