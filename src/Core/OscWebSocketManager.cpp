#include "OscWebSocketManager.h"
#include "DatabaseManager.h"

#include <QtCore/QSharedPointer>

Q_GLOBAL_STATIC(OscWebSocketManager, oscWebSocketManager)

OscWebSocketManager::OscWebSocketManager()
{
}

OscWebSocketManager& OscWebSocketManager::getInstance()
{
    return *oscWebSocketManager();
}

void OscWebSocketManager::initialize()
{
    QString oscWebSocketPort = DatabaseManager::getInstance().getConfigurationByName("OscWebSocketPort").getValue();
    this->oscWebSocketListener = QSharedPointer<OscWebSocketListener>(new OscWebSocketListener("0.0.0.0", (oscWebSocketPort.isEmpty() == true) ? Osc::DEFAULT_WEBSOCKET_PORT : oscWebSocketPort.toInt()));
    if (DatabaseManager::getInstance().getConfigurationByName("EnableOscInput").getValue() == "true")
        this->oscWebSocketListener->start();
}

void OscWebSocketManager::uninitialize()
{
}

const QSharedPointer<OscWebSocketListener> OscWebSocketManager::getOscWebSocketListener() const
{
    return this->oscWebSocketListener;
}
