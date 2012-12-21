#pragma once

#include "../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT GpiDeviceModel
{
    public:
        explicit GpiDeviceModel(const QString& serialPort, int baudRate);

        const QString& getSerialPort() const;
        int getBaudRate() const;

    private:
        QString serialPort;
        int baudRate;
};
