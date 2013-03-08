#pragma once

#include "Shared.h"

#include <QtCore/QString>

class CASPAR_EXPORT CasparThumbnail
{
    public:
        explicit CasparThumbnail(const QString& name, const QString& timestamp);

        const QString& getName() const;
        const QString& getTimestamp() const;

    private:
        QString name;
        QString timestamp;
};
