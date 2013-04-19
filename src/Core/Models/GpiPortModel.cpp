#include "GpiPortModel.h"

GpiPortModel::GpiPortModel(
        int port, bool risingEdge, Playout::PlayoutType::Type action)
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

Playout::PlayoutType::Type GpiPortModel::getAction() const
{
    return action;
}
