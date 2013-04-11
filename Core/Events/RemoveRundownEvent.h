#pragma once

#include "../Shared.h"

#include <QtCore/QEvent>

class CORE_EXPORT RemoveRundownEvent : public QEvent
{
    public:
        explicit RemoveRundownEvent(int index);

        int getIndex() const;

    private:
        int index;
};
