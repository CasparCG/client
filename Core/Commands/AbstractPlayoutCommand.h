#pragma once

#include "../Shared.h"

#include "Global.h"

class CORE_EXPORT AbstractPlayoutCommand
{
    public:
        virtual bool executeCommand(enum Playout::PlayoutType::Type type) = 0;
};
