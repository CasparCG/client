#pragma once

#include "Shared.h"

#include <QtCore/QObject>

class QAtemConnection;

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
        enum class SwitcherDeviceCommand
        {
            NONE,
            CONNECTIONSTATE
        };

        QList<QString> response;

        QAtemConnection* atemConnection = nullptr;

        SwitcherDeviceCommand command = SwitcherDeviceCommand::NONE;

        void resetDevice();
        void writeMessage(const QString& message);

        virtual void sendNotification() = 0;

    private:
        QString address;

        bool connected = false;

        Q_SLOT void setConnected();
        Q_SLOT void setDisconnected();
};
