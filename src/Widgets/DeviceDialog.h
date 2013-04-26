#pragma once

#include "Shared.h"
#include "ui_DeviceDialog.h"

#include "CasparDevice.h"

#include "Models/DeviceModel.h"

#include <QtCore/QList>
#include <QtCore/QSharedPointer>

#include <QtGui/QDialog>
#include <QtGui/QWidget>

class WIDGETS_EXPORT DeviceDialog : public QDialog, Ui::DeviceDialog
{
    Q_OBJECT

    public:
        explicit DeviceDialog(QWidget* parent = 0);

        void setDeviceModel(const DeviceModel& model);
        void setEditMode(bool editMode);

        const QString getName() const;
        const QString getAddress() const;
        const QString getPort() const;
        const QString getUsername() const;
        const QString getPassword() const;
        const QString getDescription() const;
        const QString getShadow() const;

    protected:
        void accept();

    private:
        bool editMode;

        QSharedPointer<CasparDevice> device;

        Q_SLOT void nameChanged(QString);
        Q_SLOT void addressChanged(QString);
        Q_SLOT void testConnection();
        Q_SLOT void connectionStateChanged(CasparDevice&);
};
