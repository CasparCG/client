#pragma once

#include "../Shared.h"

#include <QtCore/QEvent>
#include <QtCore/QString>

class CORE_EXPORT TargetChangedEvent : public QEvent
{
    public:
        explicit TargetChangedEvent(const QString& target);

        const QString& getTarget() const;

    private:
        QString target;
};
