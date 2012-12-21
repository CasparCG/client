#pragma once

#include "Shared.h"

#include "GpiDevice.h"

#include <QtCore/QObject>
#include <QtCore/QList>

class CORE_EXPORT GpiManager : public QObject
{
    Q_OBJECT

    public:
        explicit GpiManager();

        static GpiManager& getInstance();

        void initialize();
        void reinitialize();
        void uninitialize();

        GpiDevice::Ptr getGpiDevice();

    private:
        void setupPorts();

        GpiDevice::Ptr device;
};
