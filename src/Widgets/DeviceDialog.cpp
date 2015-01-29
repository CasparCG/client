#include "DeviceDialog.h"

#include "DatabaseManager.h"

#include <QtCore/QDebug>

#include <QtGui/QCloseEvent>

#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QMessageBox>

DeviceDialog::DeviceDialog(QWidget* parent)
    : QDialog(parent),
      editMode(false)
{
    setupUi(this);

    this->lineEditDeviceName->setStyleSheet("border-color: firebrick;");
    this->lineEditAddress->setStyleSheet("border-color: firebrick;");

    this->lineEditDeviceName->installEventFilter(this);
}

bool DeviceDialog::eventFilter(QObject* target, QEvent* event)
{
    if (target == this->lineEditDeviceName)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->text() == "," || keyEvent->text() == ":")
                return true;
        }
    }

    return QDialog::eventFilter(target, event);
}

void DeviceDialog::setDeviceModel(const DeviceModel& model)
{
    this->editMode = true;

    setWindowTitle("Edit CasparCG Server");

    this->lineEditDeviceName->setText(model.getName());
    this->lineEditAddress->setText(model.getAddress());
    this->lineEditPort->setText(QString("%1").arg(model.getPort()));
    this->lineEditUsername->setText(model.getUsername());
    this->lineEditPassword->setText(model.getPassword());
    this->lineEditDescription->setText(model.getDescription());
    this->checkBoxShadow->setChecked((model.getShadow() == "Yes") ? true : false);
    this->checkBoxPreview->setChecked((model.getPreviewChannel() > 0) ? true : false);
    this->checkBoxLocked->setChecked((model.getLockedChannel() > 0) ? true : false);
    this->spinBoxPreviewChannel->setValue((model.getPreviewChannel() > 0) ? model.getPreviewChannel() : 2);
    this->spinBoxLockedChannel->setValue((model.getLockedChannel() > 0) ? model.getLockedChannel() : 1);
    this->spinBoxPreviewChannel->setEnabled(this->checkBoxPreview->isChecked());
    this->spinBoxLockedChannel->setEnabled(this->checkBoxLocked->isChecked());
}

const QString DeviceDialog::getName() const
{
    return this->lineEditDeviceName->text();
}

const QString DeviceDialog::getAddress() const
{
    return this->lineEditAddress->text();
}

const QString DeviceDialog::getPort() const
{
    if (!this->lineEditPort->text().isEmpty())
        return this->lineEditPort->text();
    else
        return this->lineEditPort->placeholderText();
}

const QString DeviceDialog::getUsername() const
{
    return this->lineEditUsername->text();
}

const QString DeviceDialog::getPassword() const
{
    return this->lineEditPassword->text();
}

const QString DeviceDialog::getDescription() const
{
    return this->lineEditDescription->text();
}

const QString DeviceDialog::getShadow() const
{
    return this->checkBoxShadow->checkState() == Qt::Checked ? "Yes" : "No";
}

int DeviceDialog::getPreviewChannel() const
{
    return (this->checkBoxPreview->isChecked() == true) ? this->spinBoxPreviewChannel->value() : 0;
}

int DeviceDialog::getLockedChannel() const
{
    return (this->checkBoxLocked->isChecked() == true) ? this->spinBoxLockedChannel->value() : 0;
}

void DeviceDialog::accept()
{
    if (this->lineEditDeviceName->text().isEmpty() || this->lineEditAddress->text().isEmpty())
        return;

    if (!this->editMode)
    {
        DeviceModel model = DatabaseManager::getInstance().getDeviceByName(this->lineEditDeviceName->text());
        if (!model.getName().isEmpty())
        {
            QMessageBox box(this);
            box.setWindowTitle("Add Device");
            box.setWindowIcon(QIcon(":/Graphics/Images/CasparCG.png"));
            box.setText("The name already exists in the database. Please choose a unique name.");
            box.setIconPixmap(QPixmap(":/Graphics/Images/Attention.png"));
            box.setStandardButtons(QMessageBox::Ok);
            box.buttons().at(0)->setFocusPolicy(Qt::NoFocus);
            box.exec();

            this->lineEditDeviceName->setFocus();
            this->lineEditDeviceName->selectAll();

            return;
        }

        /*
        model = DatabaseManager::getInstance().getDeviceByAddress(this->lineEditAddress->text());
        if (!model.getName().isEmpty())
        {
            QMessageBox box(this);
            box.setWindowTitle("Add Device");
            box.setWindowIcon(QIcon(":/Graphics/Images/CasparCG.png"));
            box.setText("The address already exists in the database. Please choose a unique address.");
            box.setIconPixmap(QPixmap(":/Graphics/Images/Attention.png"));
            box.setStandardButtons(QMessageBox::Ok);
            box.buttons().at(0)->setFocusPolicy(Qt::NoFocus);
            box.exec();

            this->lineEditAddress->setFocus();
            this->lineEditAddress->selectAll();

            return;
        }
        */
    }

    QDialog::accept();
}

void DeviceDialog::testConnection()
{
    if (this->lineEditPort->text().isEmpty())
        this->device = QSharedPointer<CasparDevice>(new CasparDevice(this->lineEditAddress->text()));
    else
        this->device = QSharedPointer<CasparDevice>(new CasparDevice(this->lineEditAddress->text(), this->lineEditPort->text().toInt()));

    QObject::connect(this->device.data(), SIGNAL(connectionStateChanged(CasparDevice&)), this, SLOT(connectionStateChanged(CasparDevice&)));
    this->device->connectDevice();
}

void DeviceDialog::nameChanged(QString name)
{
    Q_UNUSED(name);

    if (this->lineEditDeviceName->text().isEmpty())
        this->lineEditDeviceName->setStyleSheet("border-color: firebrick;");
    else
        this->lineEditDeviceName->setStyleSheet("");
}

void DeviceDialog::addressChanged(QString name)
{
    Q_UNUSED(name);

    if (this->lineEditAddress->text().isEmpty())
        this->lineEditAddress->setStyleSheet("border-color: firebrick;");
    else
        this->lineEditAddress->setStyleSheet("");
}

void DeviceDialog::connectionStateChanged(CasparDevice& device)
{
    QObject::disconnect(this->device.data(), SIGNAL(connectionStateChanged(CasparDevice&)), this, SLOT(connectionStateChanged(CasparDevice&)));

    if (device.isConnected())
    {
        QMessageBox box(this);
        box.setWindowTitle("Test Connection");
        box.setText(QString("Successfully connected to CasparCG server: %1:%2").arg(device.getAddress()).arg(device.getPort()));
        box.setIconPixmap(QPixmap(":/Graphics/Images/Information.png"));
        box.setStandardButtons(QMessageBox::Ok);
        box.buttons().at(0)->setFocusPolicy(Qt::NoFocus);
        box.exec();
    }

    this->device->disconnectDevice();
}

void DeviceDialog::previewChannelChanged(int state)
{
    this->spinBoxPreviewChannel->setEnabled(state == Qt::Checked);
    if (state == Qt::Unchecked)
        this->spinBoxPreviewChannel->setValue(2);
}

void DeviceDialog::lockedChannelChanged(int state)
{
    this->spinBoxLockedChannel->setEnabled(state == Qt::Checked);
    if (state == Qt::Unchecked)
        this->spinBoxLockedChannel->setValue(1);
}
