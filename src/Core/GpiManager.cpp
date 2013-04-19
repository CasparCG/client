#include "GpiManager.h"

#include "DatabaseManager.h"

#include <QtCore/QDebug>

#include <QtGui/QApplication>

Q_GLOBAL_STATIC(GpiManager, gpiManager)

GpiManager::GpiManager()
{
}

GpiManager& GpiManager::getInstance()
{
    return *gpiManager();
}

void GpiManager::initialize()
{
    QString serialPort = DatabaseManager::getInstance().getConfigurationByName("GpiSerialPort").getValue();
    int baudRate = DatabaseManager::getInstance().getConfigurationByName("GpiBaudRate").getValue().toInt();

    this->device = GpiDevice::Ptr(new GpiDevice(serialPort, baudRate));

    setupPorts();
}

void GpiManager::uninitialize()
{
}

void GpiManager::reinitialize()
{
    QString serialPort = DatabaseManager::getInstance().getConfigurationByName("GpiSerialPort").getValue();
    int baudRate = DatabaseManager::getInstance().getConfigurationByName("GpiBaudRate").getValue().toInt();

    this->device->reset(serialPort, baudRate);

    setupPorts();
}

void GpiManager::setupPorts()
{
    DatabaseManager& database = DatabaseManager::getInstance();

    foreach (const GpiPortModel gpiPort, database.getGpiPorts())
        this->device->setupGpiPort(gpiPort.getPort(), gpiPort.isRisingEdge());

    foreach (const GpoPortModel gpoPort, database.getGpoPorts())
       this-> device->setupGpoPort(gpoPort.getPort(), gpoPort.getPulseLengthMillis(), gpoPort.isRisingEdge());
}

GpiDevice::Ptr GpiManager::getGpiDevice()
{
    return device;
}
