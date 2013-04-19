#include "GpoPortModel.h"

GpoPortModel::GpoPortModel(int port, bool risingEdge, int pulseLengthMillis)
    : port(port), risingEdge(risingEdge), pulseLengthMillis(pulseLengthMillis)
{
}

int GpoPortModel::getPort() const
{
    return port;
}

bool GpoPortModel::isRisingEdge() const
{
    return risingEdge;
}

int GpoPortModel::getPulseLengthMillis() const
{
    return pulseLengthMillis;
}
