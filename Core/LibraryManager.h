#pragma once

#include "Shared.h"
#include "ThumbnailWorker.h"

#include "Models/CasparData.h"
#include "CasparDevice.h"
#include "Models/CasparMedia.h"
#include "Models/CasparTemplate.h"
#include "Models/CasparThumbnail.h"

#include <QtCore/QObject>
#include <QtCore/QTimer>

class CORE_EXPORT LibraryManager : public QObject
{
    Q_OBJECT

    public:
        explicit LibraryManager(QObject* parent = 0);

        static LibraryManager& getInstance();

        void initialize();
        void uninitialize();

    protected:
        bool eventFilter(QObject* target, QEvent* event);

    private:
        QTimer refreshTimer;

        ThumbnailWorker* thumbnailWorker;

        Q_SLOT void refresh();
        Q_SLOT void deviceRemoved();
        Q_SLOT void deviceAdded(CasparDevice&);
        Q_SLOT void deviceConnectionStateChanged(CasparDevice&);
        Q_SLOT void deviceMediaChanged(const QList<CasparMedia>&, CasparDevice&);
        Q_SLOT void deviceTemplateChanged(const QList<CasparTemplate>&, CasparDevice&);
        Q_SLOT void deviceDataChanged(const QList<CasparData>&, CasparDevice&);
        Q_SLOT void deviceThumbnailChanged(const QList<CasparThumbnail>&, CasparDevice&);
};
