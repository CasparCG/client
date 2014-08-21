#pragma once

#include "../Shared.h"
#include "ui_InspectorOutputWidget.h"

#include "AtemDevice.h"
#include "CasparDevice.h"
#include "TriCasterDevice.h"

#include "Commands/AbstractCommand.h"
#include "Events/MediaChangedEvent.h"
#include "Events/Inspector/DeviceChangedEvent.h"
#include "Events/Inspector/TemplateChangedEvent.h"
#include "Events/Library/LibraryItemSelectedEvent.h"
#include "Events/Library/LibraryFilterChangedEvent.h"
#include "Events/Rundown/EmptyRundownEvent.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>
#include <QtCore/QString>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorOutputWidget : public QWidget, Ui::InspectorOutputWidget
{
    Q_OBJECT

    public:
        explicit InspectorOutputWidget(QWidget* parent = 0);

    private:
        AbstractCommand* command;
        LibraryModel* model;
        QString delayType;
        QString libraryFilter;

        void checkEmptyDevice();
        void checkEmptyAtemDevice();
        void checkEmptyTriCasterDevice();
        void checkEmptyTarget();
        void blockAllSignals(bool block);

        void fillTargetCombo(const QString& type, QString deviceName = "");

        Q_SLOT void targetChanged(QString);
        Q_SLOT void deviceAdded(CasparDevice&);
        Q_SLOT void deviceRemoved();
        Q_SLOT void deviceNameChanged(QString);
        Q_SLOT void allowGpiChanged(int);
        Q_SLOT void allowRemoteTriggeringChanged(int);
        Q_SLOT void channelChanged(int);
        Q_SLOT void videolayerChanged(int);
        Q_SLOT void delayChanged(int);
        Q_SLOT void durationChanged(int);
        Q_SLOT void tricasterDeviceAdded(TriCasterDevice&);
        Q_SLOT void tricasterDeviceRemoved();
        Q_SLOT void tricasterDeviceNameChanged(QString);
        Q_SLOT void atemDeviceAdded(AtemDevice&);
        Q_SLOT void atemDeviceRemoved();
        Q_SLOT void atemDeviceNameChanged(QString);
        Q_SLOT void remoteTriggerIdChanged(QString);
        Q_SLOT void mediaChanged(const MediaChangedEvent&);
        Q_SLOT void templateChanged(const TemplateChangedEvent&);
        Q_SLOT void deviceChanged(const DeviceChangedEvent&);
        Q_SLOT void emptyRundown(const EmptyRundownEvent&);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
        Q_SLOT void libraryItemSelected(const LibraryItemSelectedEvent&);
        Q_SLOT void libraryFilterChanged(const LibraryFilterChangedEvent&);
};
