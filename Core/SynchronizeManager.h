#pragma once

#include "Shared.h"

#include "CasparData.h"
#include "CasparDevice.h"
#include "CasparMedia.h"
#include "CasparTemplate.h"

#include <QtCore/QObject>
#include <QtCore/QTimer>

class CORE_EXPORT SynchronizeManager : public QObject
{
    Q_OBJECT

    public:
        explicit SynchronizeManager(QObject* parent = 0);

        static SynchronizeManager& getInstance();

        void initialize();
        void uninitialize();

    protected:
        bool eventFilter(QObject* target, QEvent* event);

    private:
        QTimer synchronizeTimer;

        Q_SLOT void synchronize();
        Q_SLOT void deviceRemoved();
        Q_SLOT void deviceAdded(CasparDevice&);
        Q_SLOT void deviceConnectionStateChanged(CasparDevice&);
        Q_SLOT void deviceMediaChanged(const QList<CasparMedia>&, CasparDevice&);
        Q_SLOT void deviceTemplateChanged(const QList<CasparTemplate>&, CasparDevice&);
        Q_SLOT void deviceDataChanged(const QList<CasparData>&, CasparDevice&);
};
