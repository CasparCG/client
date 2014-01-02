#pragma once

#include "Shared.h"

#include <QtCore/QObject>

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QAbstractSocket>

class TRICASTER_EXPORT NTFCDevice : public QObject
{
    Q_OBJECT

    public:
        explicit NTFCDevice(const QString& address, int port, QObject* parent = 0);
        virtual ~NTFCDevice();

        void disconnectDevice();

        bool isConnected() const;
        int getPort() const;
        const QString& getAddress() const;

        Q_SLOT void connectDevice();

    protected:
        enum NTFCCommand
        {
            NONE,
            CONNECTIONSTATE
        };

        QTcpSocket* socket;
        NTFCCommand command;
        QList<QString> response;

        virtual void sendNotification() = 0;

        void resetDevice();
        void writeMessage(const QString& message);

    private:
        struct TcpMessageHeader
        {
            unsigned int versionNo          : 32;	// The current tcp ip transmission version. This must be 1.
            unsigned int messageType        : 32;	// The message type, this must currently be 0 for send.
            unsigned int destinationSize    : 32;	// The FC3 destination name in bytes.
            unsigned int messageSize        : 32;	// The actual transmission size in bytes.
        };

        struct MessageHeader
        {
            unsigned int type               : 32;	// For XML messages, this must be 2.
            unsigned int size               : 32;	// The actual raw data size in bytes.
            unsigned char reserved[40]      ;		// Internal data members.
        };

        int port;
        bool connected;
        QString address;

        //Q_SLOT void readMessage();
        Q_SLOT void setConnected();
        Q_SLOT void setDisconnected();
};
