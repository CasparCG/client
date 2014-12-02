#pragma once

#include "../Shared.h"

#include "Global.h"
#include "Playout.h"

class CORE_EXPORT AbstractPlayoutCommand
{
    public:
        virtual ~AbstractPlayoutCommand();

        virtual bool executeCommand(Playout::PlayoutType::Type type) = 0;
};
