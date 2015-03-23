#pragma once

#include "Shared.h"

#include <QtCore/QObject>
#include <QtCore/QTextDecoder>

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QAbstractSocket>

class REPOSITORY_EXPORT RrupDevice : public QObject
{
    Q_OBJECT

    public:
        explicit RrupDevice(const QString& address, int port, QObject* parent = 0);
        virtual ~RrupDevice();

        void disconnectDevice();

        bool isConnected() const;
        int getPort() const;
        const QString& getAddress() const;

        Q_SLOT void connectDevice();

    protected:
        enum RrupDeviceCommand
        {
            NONE,
            CONNECTIONSTATE,
            ADD,
            REMOVE
        };

        QTcpSocket* socket;
        RrupDeviceCommand command;

        QString response;

        virtual void sendNotification() = 0;

        void resetDevice();
        void writeMessage(const QString& message);

    private:
        enum RrupDeviceParserState
        {
            ExpectingHeader,
            ExpectingMultiline
        };

        int port;
        int state;
        bool connected;
        QString line;
        QString address;
        QTextDecoder* decoder;
        QString fragments;

        RrupDeviceCommand translateCommand(const QString& command);

        Q_SLOT void readMessage();
        Q_SLOT void setConnected();
        Q_SLOT void setDisconnected();
};
