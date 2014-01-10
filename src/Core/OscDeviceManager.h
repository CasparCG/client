#pragma once

#include "Shared.h"

#include "OscSender.h"
#include "OscListener.h"

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
        const QSharedPointer<OscListener> getOscListener() const;

    private:
        QSharedPointer<OscSender> oscSender;
        QSharedPointer<OscListener> oscListener;
};

