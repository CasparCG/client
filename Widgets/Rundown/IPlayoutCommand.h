#pragma once

#include "../Shared.h"

#include "Global.h"

class WIDGETS_EXPORT IPlayoutCommand
{
    public:
        virtual bool executeCommand(enum Playout::PlayoutType::Type type) = 0;
};
