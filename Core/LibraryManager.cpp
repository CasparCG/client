#include "LibraryManager.h"
#include "DatabaseManager.h"
#include "DeviceManager.h"
#include "Events/AutoRefreshLibraryEvent.h"
#include "Events/DataChangedEvent.h"
#include "Events/MediaChangedEvent.h"
#include "Events/StatusbarEvent.h"
#include "Events/RefreshLibraryEvent.h"
#include "Events/TemplateChangedEvent.h"
#include "Models/DeviceModel.h"

#include "Global.h"

#include "CasparDevice.h"

#include <QtCore/QSharedPointer>
#include <QtCore/QTime>
#include <QtCore/QTimer>

#include <QtGui/QApplication>

Q_GLOBAL_STATIC(LibraryManager, libraryManager)

LibraryManager::LibraryManager(QObject* parent)
    : QObject(parent)
{
    QObject::connect(&this->refreshTimer, SIGNAL(timeout()), this, SLOT(refresh()));
    QObject::connect(&DeviceManager::getInstance(), SIGNAL(deviceRemoved()), this, SLOT(deviceRemoved()));
    QObject::connect(&DeviceManager::getInstance(), SIGNAL(deviceAdded(CasparDevice&)), this, SLOT(deviceAdded(CasparDevice&)));

    qApp->installEventFilter(this);
}

LibraryManager& LibraryManager::getInstance()
{
    return *libraryManager();
}

void LibraryManager::initialize()
{
    qApp->postEvent(qApp, new RefreshLibraryEvent(0));
    qApp->postEvent(qApp, new AutoRefreshLibraryEvent((DatabaseManager::getInstance().getConfigurationByName("AutoRefreshLibrary").getValue() == "true") ? true : false,
                                                       DatabaseManager::getInstance().getConfigurationByName("RefreshLibraryInterval").getValue().toInt() * 1000));
}

void LibraryManager::uninitialize()
{

}

bool LibraryManager::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RefreshLibrary))
    {
        RefreshLibraryEvent* refreshLibraryEvent = dynamic_cast<RefreshLibraryEvent*>(event);
        QTimer::singleShot(refreshLibraryEvent->getDelay(), this, SLOT(refresh()));
    }
    else if (event->type() == static_cast<QEvent::Type>(Enum::EventType::AutoRefreshLibrary))
    {
        AutoRefreshLibraryEvent* autoRefreshLibraryEvent = dynamic_cast<AutoRefreshLibraryEvent*>(event);
        if (this->refreshTimer.interval() != autoRefreshLibraryEvent->getInterval())
            this->refreshTimer.setInterval(autoRefreshLibraryEvent->getInterval());

        if (this->refreshTimer.isActive() != autoRefreshLibraryEvent->getAutoRefresh())
        {
            if (autoRefreshLibraryEvent->getAutoRefresh())
                this->refreshTimer.start();
            else
                this->refreshTimer.stop();
        }
    }

    return QObject::eventFilter(target, event);
}

void LibraryManager::refresh()
{
    DeviceManager::getInstance().refresh();

    if (DeviceManager::getInstance().getConnectionCount() == 0)
        return;

    qApp->postEvent(qApp, new StatusbarEvent("Refreshing library..."));

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "Yes")
            continue;

        const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getConnectionByName(model.getName());
        if (device->isConnected())
        {
            device->refreshMedia();
            device->refreshTemplate();
            device->refreshData();
        }
    }

    this->refreshTimer.setInterval(DatabaseManager::getInstance().getConfigurationByName("RefreshLibraryInterval").getValue().toInt() * 1000);
}

void LibraryManager::deviceRemoved()
{
    qApp->postEvent(qApp, new MediaChangedEvent());
    qApp->postEvent(qApp, new TemplateChangedEvent());
    qApp->postEvent(qApp, new DataChangedEvent());
}

void LibraryManager::deviceAdded(CasparDevice& device)
{
    QObject::connect(&device, SIGNAL(connectionStateChanged(CasparDevice&)), this, SLOT(deviceConnectionStateChanged(CasparDevice&)));
    QObject::connect(&device, SIGNAL(mediaChanged(const QList<CasparMedia>&, CasparDevice&)), this, SLOT(deviceMediaChanged(const QList<CasparMedia>&, CasparDevice&)));
    QObject::connect(&device, SIGNAL(templateChanged(const QList<CasparTemplate>&, CasparDevice&)), this, SLOT(deviceTemplateChanged(const QList<CasparTemplate>&, CasparDevice&)));
    QObject::connect(&device, SIGNAL(dataChanged(const QList<CasparData>&, CasparDevice&)), this, SLOT(deviceDataChanged(const QList<CasparData>&, CasparDevice&)));
}

void LibraryManager::deviceConnectionStateChanged(CasparDevice& device)
{
    if (device.isConnected())
    {
        const DeviceModel& model = DeviceManager::getInstance().getDeviceModelByAddress(device.getAddress());
        if (model.getShadow() == "Yes")
            return;

        device.refreshMedia();
        device.refreshTemplate();
        device.refreshData();
    }
}

void LibraryManager::deviceMediaChanged(const QList<CasparMedia>& mediaItems, CasparDevice& device)
{
    QTime time;
    time.start();

    QList<LibraryModel> insertModels;
    QList<LibraryModel> deleteModels;
    QList<LibraryModel> libraryModels = DatabaseManager::getInstance().getLibraryMediaByDeviceAddress(device.getAddress());

    // Find library items to delete.
    foreach (const LibraryModel& libraryModel, libraryModels)
    {
        bool found = false;
        foreach (CasparMedia mediaItem, mediaItems)
        {
            if (mediaItem.getName() == libraryModel.getName())
                found = true;
        }

        if (!found)
            deleteModels.push_back(libraryModel);
    }

    // Find library items to insert.
    foreach (CasparMedia mediaItem, mediaItems)
    {
        bool found = false;
        foreach (const LibraryModel& libraryModel, libraryModels)
        {
            if (libraryModel.getName() == mediaItem.getName())
                found = true;
        }

        if (!found)
            insertModels.push_back(LibraryModel(0, mediaItem.getName(), mediaItem.getName(), "", mediaItem.getType()));
    }

    if (deleteModels.count() > 0 || insertModels.count() > 0)
    {
        DatabaseManager::getInstance().updateLibraryMedia(device.getAddress(), deleteModels, insertModels);
        qApp->postEvent(qApp, new MediaChangedEvent());
    }

    qDebug() << QString("LibraryManager::deviceMediaChanged: %1 msec").arg(time.elapsed());
}

void LibraryManager::deviceTemplateChanged(const QList<CasparTemplate>& templateItems, CasparDevice& device)
{
    QTime time;
    time.start();

    QList<LibraryModel> insertModels;
    QList<LibraryModel> deleteModels;
    QList<LibraryModel> libraryModels = DatabaseManager::getInstance().getLibraryTemplateByDeviceAddress(device.getAddress());

    // Find library items to delete.
    foreach (const LibraryModel& libraryModel, libraryModels)
    {
        bool found = false;
        foreach (CasparTemplate templateItem, templateItems)
        {
            if (templateItem.getName() == libraryModel.getName())
                found = true;
        }

        if (!found)
            deleteModels.push_back(libraryModel);
    }

    // Find library items to insert.
    foreach (CasparTemplate templateItem, templateItems)
    {
        bool found = false;
        foreach (const LibraryModel& libraryModel, libraryModels)
        {
            if (libraryModel.getName() == templateItem.getName())
                found = true;
        }

        if (!found)
            insertModels.push_back(LibraryModel(0, templateItem.getName(), templateItem.getName(), "", "TEMPLATE"));
    }

    if (deleteModels.count() > 0 || insertModels.count() > 0)
    {
        DatabaseManager::getInstance().updateLibraryTemplate(device.getAddress(), deleteModels, insertModels);
        qApp->postEvent(qApp, new TemplateChangedEvent());
    }

    qDebug() << QString("LibraryManager::deviceTemplateChanged: %1 msec").arg(time.elapsed());
}

void LibraryManager::deviceDataChanged(const QList<CasparData>& dataItems, CasparDevice& device)
{
    QTime time;
    time.start();

    QList<LibraryModel> insertModels;
    QList<LibraryModel> deleteModels;
    QList<LibraryModel> libraryModels = DatabaseManager::getInstance().getLibraryDataByDeviceAddress(device.getAddress());

    // Find library items to delete.
    foreach (const LibraryModel& libraryModel, libraryModels)
    {
        bool found = false;
        foreach (CasparData dataItem, dataItems)
        {
            if (dataItem.getName() == libraryModel.getName())
                found = true;
        }

        if (!found)
            deleteModels.push_back(libraryModel);
    }

    // Find library items to insert.
    foreach (CasparData dataItem, dataItems)
    {
        bool found = false;
        foreach (const LibraryModel& libraryModel, libraryModels)
        {
            if (libraryModel.getName() == dataItem.getName())
                found = true;
        }

        if (!found)
            insertModels.push_back(LibraryModel(0, dataItem.getName(), dataItem.getName(), "", "DATA"));
    }

    if (deleteModels.count() > 0 || insertModels.count() > 0)
    {
        DatabaseManager::getInstance().updateLibraryData(device.getAddress(), deleteModels, insertModels);
        qApp->postEvent(qApp, new DataChangedEvent());
    }

    qDebug() << QString("LibraryManager::deviceDataChanged: %1 msec").arg(time.elapsed());
}
