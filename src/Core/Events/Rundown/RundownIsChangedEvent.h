#pragma once

#include "../../Shared.h"

#include <QtCore/QEvent>

class CORE_EXPORT RundownIsChangedEvent : public QEvent
{
    public:
        explicit RundownIsChangedEvent(bool changed);

        bool getChanged() const;

    private:
        bool changed;
};
