#pragma once

#include "Shared.h"

#include <QtCore/QString>

class CASPAR_EXPORT CasparMedia
{
    public:
        explicit CasparMedia(const QString& name, const QString& type);

        const QString& getName() const;
        const QString& getType() const;

    private:
        QString name;
        QString type;
};
