#pragma once

#include "Shared.h"
#include "ui_OscOutputDialog.h"

#include "TriCasterDevice.h"

#include "Models/OscOutputModel.h"

#include <QtCore/QList>
#include <QtCore/QSharedPointer>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QDialog>
#include <QtWidgets/QWidget>
#else
#include <QtGui/QDialog>
#include <QtGui/QWidget>
#endif

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

        Q_SLOT void nameChanged(QString);
        Q_SLOT void addressChanged(QString);
};
