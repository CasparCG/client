#include "CasparServerVersion.h"

CasparServerVersion::CasparServerVersion(const QString& version)
    : version(version)
{
}

const QString& CasparServerVersion::getVersion() const
{
    return this->version;
}
