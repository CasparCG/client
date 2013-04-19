#pragma once

#include "../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT GpoPortModel
{
    public:
        explicit GpoPortModel(int port, bool risingEdge, int pulseLengthMillis);

        int getPort() const;
        bool isRisingEdge() const;
        int getPulseLengthMillis() const;

    private:
        int port;
        bool risingEdge;
        int pulseLengthMillis;
};
