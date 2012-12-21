#pragma once

#include "Shared.h"
#include "ui_AddDeviceDialog.h"

#include "CasparDevice.h"
#include "CasparVersion.h"

#include <QtGui/QDialog>
#include <QtGui/QWidget>

class WIDGETS_EXPORT AddDeviceDialog : public QDialog, Ui::AddDeviceDialog
{
    Q_OBJECT

    public:
        explicit AddDeviceDialog(QWidget* parent = 0);

        const QString getName() const;
        const QString getAddress() const;
        const QString getPort() const;
        const QString getUsername() const;
        const QString getPassword() const;
        const QString getDescription() const;
        const QString getVersion() const;
        const QString getShadow() const;

    protected:
        void closeEvent(QCloseEvent* event);

    private:
        bool isClosing;
        CasparDevice* device;

        Q_SLOT void lookupName();
        Q_SLOT void lookupAddress();
        Q_SLOT void deviceConnectionStateChanged(CasparDevice&);
        Q_SLOT void deviceVersionChanged(const CasparVersion&, CasparDevice&);
};
