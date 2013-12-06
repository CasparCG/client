#pragma once

#include "Shared.h"

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

        const QSharedPointer<OscListener> getOscListener() const;

    private:
        QSharedPointer<OscListener> oscListener;
};

