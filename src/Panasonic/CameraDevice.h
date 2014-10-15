#pragma once

#include "Shared.h"

#include <QtCore/QObject>
#include <QtCore/QString>

#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>

class PANASONIC_EXPORT CameraDevice : public QObject
{
    Q_OBJECT

    public:
        explicit CameraDevice(QObject* parent = 0);

        void selectPreset(const QString& address, const QString& preset);

    private:
        QNetworkAccessManager* networkManager;

        Q_SLOT void selectPresetFinished(QNetworkReply*);
};
