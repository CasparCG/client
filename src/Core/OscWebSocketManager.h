#pragma once

#include "Shared.h"

#include "OscWebSocketListener.h"

#include <QtCore/QObject>
#include <QtCore/QSharedPointer>

class CORE_EXPORT OscWebSocketManager : public QObject
{
    Q_OBJECT

    public:
        explicit OscWebSocketManager();

        static OscWebSocketManager& getInstance();

        void initialize();
        void uninitialize();

        const QSharedPointer<OscWebSocketListener> getOscWebSocketListener() const;

    private:
        QSharedPointer<OscWebSocketListener> oscWebSocketListener;
};

