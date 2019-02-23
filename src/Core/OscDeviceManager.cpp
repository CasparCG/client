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

    QString oscMonitorPort = DatabaseManager::getInstance().getConfigurationByName("OscMonitorPort").getValue();
    this->oscMonitorListener = QSharedPointer<OscMonitorListener>(new OscMonitorListener());
    if (DatabaseManager::getInstance().getConfigurationByName("EnableOscInputMonitor").getValue() == "true")
        this->oscMonitorListener->start((oscMonitorPort.isEmpty() == true) ? Osc::DEFAULT_MONITOR_PORT : oscMonitorPort.toInt());

    QString oscControlPort = DatabaseManager::getInstance().getConfigurationByName("OscControlPort").getValue();
    this->oscControlListener = QSharedPointer<OscControlListener>(new OscControlListener());
    if (DatabaseManager::getInstance().getConfigurationByName("EnableOscInputControl").getValue() == "true")
        this->oscControlListener->start((oscControlPort.isEmpty() == true) ? Osc::DEFAULT_CONTROL_PORT : oscControlPort.toInt());
}

void OscDeviceManager::uninitialize()
{
}

const QSharedPointer<OscSender> OscDeviceManager::getOscSender() const
{
    return this->oscSender;
}

const QSharedPointer<OscMonitorListener> OscDeviceManager::getOscMonitorListener() const
{
    return this->oscMonitorListener;
}
 
const QSharedPointer<OscControlListener> OscDeviceManager::getOscControlListener() const
{
    return this->oscControlListener;
}
