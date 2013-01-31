#pragma once

#include "Shared.h"

#include <QtCore/QString>

class CASPAR_EXPORT CasparServerVersion
{
    public:
        explicit CasparServerVersion(const QString& version);

        const QString& getVersion() const;

    private:
        QString version;
};
