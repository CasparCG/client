#pragma once

#include "Shared.h"
#include "ui_OscOutputDialog.h"

#include "TriCasterDevice.h"

#include "Models/OscOutputModel.h"

#include <QtCore/QList>
#include <QtCore/QSharedPointer>

#include <QtWidgets/QDialog>
#include <QtWidgets/QWidget>

class WIDGETS_EXPORT OscOutputDialog : public QDialog, Ui::OscOutputDialog
{
    Q_OBJECT

    public:
        explicit OscOutputDialog(QWidget* parent = 0);

        void setDeviceModel(const OscOutputModel& model);

        const QString getName() const;
        const QString getAddress() const;
        const QString getPort() const;
        const QString getDescription() const;

    protected:
        void accept();
        bool eventFilter(QObject* target, QEvent* event);

    private:
        bool editMode;

        bool lookupName(const QString& name);
        bool lookupAddress(const QString& address);

        Q_SLOT void nameChanged(QString);
        Q_SLOT void addressChanged(QString);
};
