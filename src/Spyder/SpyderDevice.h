#pragma once

#include "Shared.h"

#include <QtCore/QObject>
#include <QtCore/QString>

class SPYDER_EXPORT SpyderDevice : public QObject
{
    Q_OBJECT

    public:
        explicit SpyderDevice(QObject* parent = 0);

        void selectPreset(int preset, const QString& address, int port);

    private:
        int preset;
};
