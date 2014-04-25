#pragma once

#include "Shared.h"

#include <stdint.h>

#include <QtCore/QObject>

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QAbstractSocket>

class TRICASTER_EXPORT NtfcDevice : public QObject
{
    Q_OBJECT

    public:
        explicit NtfcDevice(const QString& address, int port, QObject* parent = 0);
        virtual ~NtfcDevice();

        void disconnectDevice();

        bool isConnected() const;
        int getPort() const;
        const QString& getAddress() const;

        Q_SLOT void connectDevice();

    protected:
        enum NtfcDeviceCommand
        {
            NONE,
            CONNECTIONSTATE
        };

        QTcpSocket* socket;
        NtfcDeviceCommand command;

        virtual void sendNotification() = 0;

        void resetDevice();
        void writeMessage(const QString& message);

    private:
        struct TcpMessageHeader
        {
            uint32_t versionNo;         // The current tcp ip transmission version. This must be 1.
            uint32_t messageType;		// The message type, this must currently be 0 for send.
            uint32_t destinationSize;	// The FC3 destination name in bytes.
            uint32_t messageSize;		// The actual transmission size in bytes.
        };

        struct MessageHeader
        {
            uint32_t type;				// For XML messages, this must be 2.
            uint32_t size;				// The actual raw data size in bytes.
            uint32_t reserved[10];		// Internal data members.
        };

        int port;
        bool connected;
        QString address;

        //Q_SLOT void readMessage();
        Q_SLOT void setConnected();
        Q_SLOT void setDisconnected();
};
