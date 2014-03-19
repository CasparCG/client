#pragma once

#include "../../Shared.h"

class CORE_EXPORT DeleteRundownEvent
{
    public:
        explicit DeleteRundownEvent(int index);

        int getIndex() const;

    private:
        int index;
};
