#include "GpiDevice.h"

#include <boost/bind.hpp>

GpiDevice::GpiDevice(const QString& serialPort, int baudRate)
    : device(gpio::serial_port_device::create(
            serialPort.toStdString(), baudRate, boost::bind(&GpiDevice::onConnected, this, _1)))
{
}

void GpiDevice::reset(const QString& serialPort, int baudRate)
{
    emit connectionStateChanged(false, this);
    connected = false;
    gpo_triggers.clear();
    device.reset();
    device = gpio::serial_port_device::create(
            serialPort.toStdString(), baudRate, boost::bind(&GpiDevice::onConnected, this, _1));
}

void GpiDevice::setupGpiPort(int gpi, bool risingEdge)
{
    device->setup_gpi_pulse(gpi, risingEdge ? gpio::LOW : gpio::HIGH, boost::bind(&GpiDevice::onTrigger, this, gpi));
}

void GpiDevice::setupGpoPort(int gpo, int pulseLengthMillis, bool risingEdge)
{
    gpo_triggers.insert(std::make_pair(
            gpo,
            device->setup_gpo_pulse(gpo, risingEdge ? gpio::LOW : gpio::HIGH, pulseLengthMillis)));
}

void GpiDevice::onTrigger(int gpi)
{
    emit gpiTriggered(gpi, this);
}

void GpiDevice::onConnected(bool state)
{
    connected = state;
    emit connectionStateChanged(state, this);
}

bool GpiDevice::isConnected()
{
    return connected;
}

void GpiDevice::trigger(int gpo)
{
    std::map<int, gpio::gpo_trigger::ptr>::iterator iter = gpo_triggers.find(gpo);

    if (iter != gpo_triggers.end())
        iter->second->fire();
    else
        throw std::runtime_error("GPO not setup for port");
}

int GpiDevice::getNumGpi()
{
    return device->get_num_gpi_ports();
}

int GpiDevice::getNumGpo()
{
    return device->get_num_gpo_ports();
}
