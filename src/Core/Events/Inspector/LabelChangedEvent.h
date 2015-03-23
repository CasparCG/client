#pragma once

#include "../../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT LabelChangedEvent
{
    public:
        explicit LabelChangedEvent(const QString& label);

        const QString& getLabel() const;

    private:
        QString label;
};
