#pragma once

#include "Shared.h"
#include "ui_AtemDeviceDialog.h"

#include "AtemDevice.h"

#include "Models/Atem/AtemDeviceModel.h"

#include <QtCore/QList>
#include <QtCore/QSharedPointer>

#include <QtGui/QDialog>
#include <QtGui/QWidget>

class WIDGETS_EXPORT AtemDeviceDialog : public QDialog, Ui::AtemDeviceDialog
{
    Q_OBJECT

    public:
        explicit AtemDeviceDialog(QWidget* parent = 0);

        void setDeviceModel(const AtemDeviceModel& model);

        const QString getName() const;
        const QString getAddress() const;
        const QString getDescription() const;

    protected:
        void accept();
        bool eventFilter(QObject* target, QEvent* event);

    private:
        bool editMode;

        QSharedPointer<AtemDevice> device;

        Q_SLOT void nameChanged(QString);
        Q_SLOT void addressChanged(QString);
        Q_SLOT void testConnection();
        Q_SLOT void connectionStateChanged(AtemDevice&);
};
