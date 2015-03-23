#include "GpiPortModel.h"

GpiPortModel::GpiPortModel(int port, bool risingEdge, Playout::PlayoutType action)
    : port(port), risingEdge(risingEdge), action(action)
{
}

int GpiPortModel::getPort() const
{
    return port;
}

bool GpiPortModel::isRisingEdge() const
{
    return risingEdge;
}

Playout::PlayoutType GpiPortModel::getAction() const
{
    return action;
}
