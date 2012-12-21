#include "GpiDeviceModel.h"

GpiDeviceModel::GpiDeviceModel(const QString& serialPort, int baudRate)
    : serialPort(serialPort), baudRate(baudRate)
{
}

const QString& GpiDeviceModel::getSerialPort() const
{
    return serialPort;
}

int GpiDeviceModel::getBaudRate() const
{
    return baudRate;
}
