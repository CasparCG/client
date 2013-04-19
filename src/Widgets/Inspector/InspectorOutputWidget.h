#pragma once

#include "../Shared.h"
#include "ui_InspectorOutputWidget.h"

#include "CasparDevice.h"

#include "Animations/BorderAnimation.h"
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
        BorderAnimation* deviceAnimation;

        void checkEmptyDevice();
        void blockAllSignals(bool block);

        Q_SLOT void deviceAdded(CasparDevice&);
        Q_SLOT void deviceRemoved();
        Q_SLOT void deviceNameChanged(QString);
        Q_SLOT void allowGpiChanged(int);
        Q_SLOT void channelChanged(int);
        Q_SLOT void videolayerChanged(int);
        Q_SLOT void delayChanged(int);
        Q_SLOT void resetAllowGpi(QString);
        Q_SLOT void resetChannel(QString);
        Q_SLOT void resetVideolayer(QString);
        Q_SLOT void resetDelay(QString);
};
