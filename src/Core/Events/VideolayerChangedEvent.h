#pragma once

#include "../Shared.h"

#include <QtCore/QEvent>
#include <QtCore/QString>

class CORE_EXPORT VideolayerChangedEvent : public QEvent
{
    public:
        explicit VideolayerChangedEvent(int videolayer);

        int getVideolayer() const;

    private:
        int videolayer;
};
