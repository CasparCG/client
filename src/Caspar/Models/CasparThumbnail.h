#pragma once

#include "../Shared.h"

#include <QtCore/QString>

class CASPAR_EXPORT CasparThumbnail
{
    public:
        explicit CasparThumbnail(const QString& name, const QString& timestamp, const QString& size);

        const QString& getName() const;
        const QString& getTimestamp() const;
        const QString& getSize() const;

    private:
        QString name;
        QString timestamp;
        QString size;
};
