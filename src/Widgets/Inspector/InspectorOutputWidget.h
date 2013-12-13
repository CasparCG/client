#pragma once

#include "../Shared.h"
#include "ui_InspectorOutputWidget.h"

#include "CasparDevice.h"
#include "TriCasterDevice.h"

#include "Commands/AbstractCommand.h"
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

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        AbstractCommand* command;
        LibraryModel* model;

        void checkEmptyDevice();
        void checkEmptyTriCasterDevice();
        void checkEmptyTarget();
        void blockAllSignals(bool block);

        void fillTargetCombo(const QString& type);

        Q_SLOT void targetChanged(QString);
        Q_SLOT void deviceAdded(CasparDevice&);
        Q_SLOT void deviceRemoved();
        Q_SLOT void deviceNameChanged(QString);
        Q_SLOT void allowGpiChanged(int);
        Q_SLOT void allowRemoteTriggeringChanged(int);
        Q_SLOT void channelChanged(int);
        Q_SLOT void videolayerChanged(int);
        Q_SLOT void delayChanged(int);
        Q_SLOT void tricasterDeviceAdded(TriCasterDevice&);
        Q_SLOT void tricasterDeviceRemoved();
        Q_SLOT void tricasterDeviceNameChanged(QString);
};
