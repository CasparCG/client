#pragma once

#include "../../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT TargetChangedEvent
{
    public:
        explicit TargetChangedEvent(const QString& target);

        const QString& getTarget() const;

    private:
        QString target;
};
