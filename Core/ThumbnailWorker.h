#pragma once

#include "Shared.h"

#include "Models/CasparData.h"
#include "CasparDevice.h"
#include "Models/CasparMedia.h"
#include "Models/CasparTemplate.h"
#include "Models/CasparThumbnail.h"

#include "Models/ThumbnailModel.h"

#include <QtCore/QObject>
#include <QtCore/QTimer>

class CORE_EXPORT ThumbnailWorker : public QObject
{
    Q_OBJECT

    public:
        explicit ThumbnailWorker(const QList<ThumbnailModel>& thumbnailModels, QObject* parent = 0);

        void start();

    private:
        QTimer thumbnailTimer;

        int currentId;
        QString currentName;
        QString currentAddress;
        QString currentTimestamp;

        QList<ThumbnailModel> thumbnailModels;

        Q_SLOT void process();
        Q_SLOT void deviceThumbnailRetrieveChanged(const QString& data, CasparDevice& device);
};
