#pragma once

#include "../Shared.h"

#include <QtCore/QEvent>
#include <QtCore/QString>

class CORE_EXPORT AddPresetItemEvent : public QEvent
{
    public:
        explicit AddPresetItemEvent(const QString& preset);

        const QString& getPreset() const;

    private:
        QString preset;
};
