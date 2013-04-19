#pragma once

#include "../Shared.h"

#include <QtCore/QEvent>
#include <QtCore/QString>

class CORE_EXPORT ChannelChangedEvent : public QEvent
{
    public:
        explicit ChannelChangedEvent(int channel);

        int getChannel() const;

    private:
        int channel;
};
