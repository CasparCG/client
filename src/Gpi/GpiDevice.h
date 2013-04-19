#pragma once

#include "Shared.h"

#include <map>

#include <gpio/serial_port_device.h>

#include <QSharedPointer>

class GPI_EXPORT GpiDevice : public QObject
{
    Q_OBJECT

public:
    typedef QSharedPointer<GpiDevice> Ptr;

    explicit GpiDevice(const QString& serialPort, int baudRate);

    void reset(const QString& serialPort, int baudRate);

    bool isConnected();

    void trigger(int gpo);
    int getNumGpi();
    int getNumGpo();
    void setupGpoPort(int gpo, int pulseLengthMillis, bool risingEdge);
    void setupGpiPort(int gpi, bool risingEdge);

    Q_SIGNAL void connectionStateChanged(bool, GpiDevice*);
    Q_SIGNAL void gpiTriggered(int, GpiDevice*);
private:
    void onTrigger(int gpi);
    void onConnected(bool state);
private:
    bool connected;
    std::map<int, gpio::gpo_trigger::ptr> gpo_triggers;
    gpio::gpio_device::ptr device;
};
