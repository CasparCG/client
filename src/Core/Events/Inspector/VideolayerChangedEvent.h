#pragma once

#include "../../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT VideolayerChangedEvent
{
    public:
        explicit VideolayerChangedEvent(int videolayer);

        int getVideolayer() const;

    private:
        int videolayer;
};
