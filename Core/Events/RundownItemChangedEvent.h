#pragma once

#include "../Shared.h"

#include <QtCore/QEvent>
#include <QtCore/QString>

class CORE_EXPORT RundownItemChangedEvent : public QEvent
{
    public:
        explicit RundownItemChangedEvent(const QString& label, const QString& name, const QString& deviceName);

        const QString& getLabel() const;
        const QString& getName() const;
        const QString& getDeviceName() const;

    private:
        QString label;
        QString name;
        QString deviceName;
};
