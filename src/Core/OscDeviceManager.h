#pragma once

#include "Shared.h"

#include "OscSender.h"
#include "OscMonitorListener.h"
#include "OscControlListener.h"

#include <QtCore/QObject>
#include <QtCore/QSharedPointer>

class CORE_EXPORT OscDeviceManager : public QObject
{
    Q_OBJECT

    public:
        explicit OscDeviceManager();

        static OscDeviceManager& getInstance();

        void initialize();
        void uninitialize();

        const QSharedPointer<OscSender> getOscSender() const;
        const QSharedPointer<OscMonitorListener> getOscMonitorListener() const;
        const QSharedPointer<OscControlListener> getOscControlListener() const;

    private:
        QSharedPointer<OscSender> oscSender;
        QSharedPointer<OscMonitorListener> oscMonitorListener;
        QSharedPointer<OscControlListener> oscControlListener;
};

