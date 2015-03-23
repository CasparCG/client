#pragma once

#include "Shared.h"

#include <QtCore/QObject>

class QObject;
class QTcpSocket;
class QTextDecoder;

class CASPAR_EXPORT AmcpDevice : public QObject
{
    Q_OBJECT

    public:
        explicit AmcpDevice(const QString& address, int port, QObject* parent = 0);
        virtual ~AmcpDevice();

        void connectDevice();
        void disconnectDevice();

        void setDisableCommands(bool disable);

        bool isConnected() const;
        int getPort() const;
        const QString& getAddress() const;

    protected:
        enum class AmcpDeviceCommand
        {
            NONE,
            CONNECTIONSTATE,
            LOAD,
            LOADBG,
            PLAY,
            STOP,
            CG,
            CLS,
            CINF,
            VERSION,
            TLS,
            INFO,
            INFOSYSTEM,
            DATALIST,
            DATARETRIEVE,
            CLEAR,
            SET,
            MIXER,
            CALL,
            REMOVE,
            ADD,
            SWAP,
            STATUS,
            ERROR,
            THUMBNAILLIST,
            THUMBNAILRETRIEVE
        };

        QTcpSocket* socket = nullptr;
        AmcpDeviceCommand command = AmcpDeviceCommand::NONE;

        QList<QString> response;

        virtual void sendNotification() = 0;

        void resetDevice();
        void writeMessage(const QString& message);

    private:
        enum class AmcpDeviceParserState
        {
            ExpectingHeader,
            ExpectingOneline,
            ExpectingTwoline,
            ExpectingMultiline
        };

        QString address;

        int port;
        int code;

        bool connected = false;
        bool disableCommands = false;

        QString fragments;

        QTextDecoder* decoder = nullptr;

        AmcpDeviceParserState state = AmcpDeviceParserState::ExpectingHeader;

        void parseLine(const QString& line);
        void parseHeader(const QString& line);
        void parseOneline(const QString& line);
        void parseTwoline(const QString& line);
        void parseMultiline(const QString& line);

        AmcpDeviceCommand translateCommand(const QString& command);

        void readMessage();
        void setConnected();
        void setDisconnected();
};
