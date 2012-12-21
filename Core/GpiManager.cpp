#include "GpiManager.h"

#include "DatabaseManager.h"
#include "Models/GpiDeviceModel.h"

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
    GpiDeviceModel model = DatabaseManager::getInstance().getGpiDevice();

    device = GpiDevice::Ptr(new GpiDevice(model.getSerialPort(), model.getBaudRate()));

    setupPorts();
}

void GpiManager::setupPorts()
{
    DatabaseManager& database = DatabaseManager::getInstance();

    foreach (const GpiPortModel gpiPort, database.getGpiPorts())
        device->setupGpiPort(gpiPort.getPort(), gpiPort.isRisingEdge());

    foreach (const GpoPortModel gpoPort, database.getGpoPorts())
        device->setupGpoPort(gpoPort.getPort(), gpoPort.getPulseLengthMillis(), gpoPort.isRisingEdge());
}

void GpiManager::reinitialize()
{
    GpiDeviceModel model = DatabaseManager::getInstance().getGpiDevice();

    device->reset(model.getSerialPort(), model.getBaudRate());

    setupPorts();
}

void GpiManager::uninitialize()
{

}

GpiDevice::Ptr GpiManager::getGpiDevice()
{
    return device;
}
