#pragma once

#include "../Shared.h"

#include <QtCore/QEvent>

class CORE_EXPORT RefreshLibraryEvent : public QEvent
{
    public:
        explicit RefreshLibraryEvent(int delay);

        int getDelay() const;

    private:
        int delay;
};
