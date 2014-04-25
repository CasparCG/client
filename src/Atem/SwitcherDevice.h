#pragma once

#include "Shared.h"

#include "qatemconnection.h"

#include <QtCore/QObject>

class ATEM_EXPORT SwitcherDevice : public QObject
{
    Q_OBJECT

    public:
        explicit SwitcherDevice(const QString& address, QObject* parent = 0);
        virtual ~SwitcherDevice();

        void disconnectDevice();

        bool isConnected() const;
        const QString& getAddress() const;

        Q_SLOT void connectDevice();

    protected:
        enum SwitcherDeviceCommand
        {
            NONE,
            CONNECTIONSTATE
        };

        QAtemConnection* atemConnection;

        SwitcherDeviceCommand command;
        QList<QString> response;

        virtual void sendNotification() = 0;

        void resetDevice();
        void writeMessage(const QString& message);

    private:      
        bool connected;
        QString address;

        Q_SLOT void setConnected();
        Q_SLOT void setDisconnected();
};
