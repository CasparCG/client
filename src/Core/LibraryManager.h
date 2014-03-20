#pragma once

#include "Shared.h"

#include "Global.h"

#include "CasparDevice.h"

#include "ThumbnailWorker.h"
#include "Events/Library/RefreshLibraryEvent.h"
#include "Events/Library/AutoRefreshLibraryEvent.h"
#include "Models/CasparData.h"
#include "Models/CasparMedia.h"
#include "Models/CasparTemplate.h"
#include "Models/CasparThumbnail.h"

#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtCore/QTimer>

class CORE_EXPORT LibraryManager : public QObject
{
    Q_OBJECT

    public:
        explicit LibraryManager(QObject* parent = 0);

        static LibraryManager& getInstance();

        void initialize();
        void uninitialize();

    private:
        QTimer refreshTimer;
        QList<QSharedPointer<ThumbnailWorker> > thumbnailWorkers;

        Q_SLOT void refresh();
        Q_SLOT void deviceRemoved();
        Q_SLOT void deviceAdded(CasparDevice&);
        Q_SLOT void versionChanged(const QString&, CasparDevice&);
        Q_SLOT void infoChanged(const QList<QString>&, CasparDevice&);
        Q_SLOT void connectionStateChanged(CasparDevice&);
        Q_SLOT void mediaChanged(const QList<CasparMedia>&, CasparDevice&);
        Q_SLOT void templateChanged(const QList<CasparTemplate>&, CasparDevice&);
        Q_SLOT void dataChanged(const QList<CasparData>&, CasparDevice&);
        Q_SLOT void thumbnailChanged(const QList<CasparThumbnail>&, CasparDevice&);

        Q_SLOT void refreshLibrary(const RefreshLibraryEvent&);
        Q_SLOT void autoRefreshLibrary(const AutoRefreshLibraryEvent&);
};
