#pragma once

#include "../Shared.h"
#include "Global.h"

#include <QtCore/QString>

class CORE_EXPORT GpiPortModel
{
    public:
        explicit GpiPortModel(
                int port, bool risingEdge, Playout::PlayoutType::Type action);

        int getPort() const;
        bool isRisingEdge() const;
        Playout::PlayoutType::Type getAction() const;

    private:
        int port;
        bool risingEdge;
        Playout::PlayoutType::Type action;
};
