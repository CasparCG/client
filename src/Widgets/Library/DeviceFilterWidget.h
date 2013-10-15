#pragma once

#include "../Shared.h"
#include "ui_DeviceFilterWidget.h"

#include "CasparDevice.h"

#include <QtGui/QWidget>

class WIDGETS_EXPORT DeviceFilterWidget : public QWidget, Ui::DeviceFilterWidget
{
    Q_OBJECT

    public:
        explicit DeviceFilterWidget(QWidget* parent = 0);

        QList<QString> getDeviceFilter();

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        bool sendEvents;

        void blockAllSignals(bool block);

        Q_SLOT void deviceAdded(CasparDevice&);
        Q_SLOT void deviceRemoved();
        Q_SLOT void dataChanged(const QModelIndex&, const QModelIndex&);
};
