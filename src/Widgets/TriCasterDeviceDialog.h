#pragma once

#include "Shared.h"
#include "ui_TriCasterDeviceDialog.h"

#include "TriCasterDevice.h"

#include "Models/TriCaster/TriCasterDeviceModel.h"

#include <QtCore/QList>
#include <QtCore/QSharedPointer>

#include <QtGui/QDialog>
#include <QtGui/QWidget>

class WIDGETS_EXPORT TriCasterDeviceDialog : public QDialog, Ui::TriCasterDeviceDialog
{
    Q_OBJECT

    public:
        explicit TriCasterDeviceDialog(QWidget* parent = 0);

        void setDeviceModel(const TriCasterDeviceModel& model);

        const QString getName() const;
        const QString getAddress() const;
        const QString getPort() const;
        const QString getDescription() const;

    protected:
        void accept();
        bool eventFilter(QObject* target, QEvent* event);

    private:
        bool editMode;

        QSharedPointer<TriCasterDevice> device;

        Q_SLOT void nameChanged(QString);
        Q_SLOT void addressChanged(QString);
        Q_SLOT void testConnection();
        Q_SLOT void connectionStateChanged(TriCasterDevice&);
};
