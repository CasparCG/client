#pragma once

#include "Shared.h"

#include <QtCore/QObject>
#include <QtCore/QString>

#define VISCA_COMMAND           0x01
#define VISCA_COMMAND_CONTROL   0x02
#define VISCA_INQUIRY           0x09
#define VISCA_TERMINATOR        0xFF

#define VISCA_RESET_SEQNR       0x01

class SONY_EXPORT ViscaDevice : public QObject
{
    Q_OBJECT

    public:
        explicit ViscaDevice(QObject* parent = 0);

        void selectPreset(const QString& address, const QString& preset);

    private:
        QString preset;
        QString address;

        Q_SLOT void sendClearSequence();
        Q_SLOT void sendRecallPreset();
};
