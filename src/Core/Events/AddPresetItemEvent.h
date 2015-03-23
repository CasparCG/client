#pragma once

#include "../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT AddPresetItemEvent
{
    public:
        explicit AddPresetItemEvent(const QString& preset);

        const QString& getPreset() const;

    private:
        QString preset;
};
