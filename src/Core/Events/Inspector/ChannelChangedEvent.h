#pragma once

#include "../../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT ChannelChangedEvent
{
    public:
        explicit ChannelChangedEvent(int channel);

        int getChannel() const;

    private:
        int channel;
};
