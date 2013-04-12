#pragma once

#include "../Shared.h"

#include <QtCore/QEvent>

class CORE_EXPORT DeleteRundownEvent : public QEvent
{
    public:
        explicit DeleteRundownEvent(int index);

        int getIndex() const;

    private:
        int index;
};
