#include "OscDeviceManager.h"
#include "DatabaseManager.h"

#include <QtCore/QSharedPointer>

Q_GLOBAL_STATIC(OscDeviceManager, oscDeviceManager)

OscDeviceManager::OscDeviceManager()
{
}

OscDeviceManager& OscDeviceManager::getInstance()
{
    return *oscDeviceManager();
}

void OscDeviceManager::initialize()
{
    this->oscSender = QSharedPointer<OscSender>();

    QString oscPort = DatabaseManager::getInstance().getConfigurationByName("OscPort").getValue();
    this->oscListener = QSharedPointer<OscListener>(new OscListener());
    if (DatabaseManager::getInstance().getConfigurationByName("EnableOscInputMonitoring").getValue() == "true")
        this->oscListener->start((oscPort.isEmpty() == true) ? Osc::DEFAULT_PORT : oscPort.toInt());
}

void OscDeviceManager::uninitialize()
{
}

const QSharedPointer<OscSender> OscDeviceManager::getOscSender() const
{
    return this->oscSender;
}

const QSharedPointer<OscListener> OscDeviceManager::getOscListener() const
{
    return this->oscListener;
}
