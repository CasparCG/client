#pragma once

#include "../Shared.h"

#include "Playout.h"

class CORE_EXPORT AbstractPlayoutCommand
{
    public:
        virtual ~AbstractPlayoutCommand();

        virtual bool executeCommand(Playout::PlayoutType type) = 0;
};
