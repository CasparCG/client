#include "AddDeviceDialog.h"

#include "DatabaseManager.h"

#include <QtCore/QDebug>

#include <QtGui/QAbstractButton>
#include <QtGui/QMessageBox>

AddDeviceDialog::AddDeviceDialog(QWidget* parent)
    : QDialog(parent), isClosing(false)
{
    setupUi(this);
}

void AddDeviceDialog::closeEvent(QCloseEvent* event)
{
    this->isClosing = true;
    QDialog::closeEvent(event);
}

void AddDeviceDialog::lookupName()
{
    if (this->isClosing)
        return;

    DeviceModel model = DatabaseManager::getInstance().getDeviceByName(this->lineEditDeviceName->text());
    if (!model.getName().isEmpty())
    {
        QMessageBox box;
        box.setWindowTitle("Add device");
        box.setText("The name already exists in the database. Please choose a unique name for the device.");
        box.setIcon(QMessageBox::Critical);
        box.setStandardButtons(QMessageBox::Ok);
        box.buttons().at(0)->setFocusPolicy(Qt::NoFocus);
        box.exec();

        this->lineEditDeviceName->setFocus();
        this->lineEditDeviceName->selectAll();
    }
}

void AddDeviceDialog::lookupAddress()
{
    if (this->isClosing)
        return;

    DeviceModel model = DatabaseManager::getInstance().getDeviceByAddress(this->lineEditAddress->text());
    if (!model.getName().isEmpty())
    {
        QMessageBox box;
        box.setWindowTitle("Add device");
        box.setText("The address already exists in the database. Please choose a unique address for the device.");
        box.setIcon(QMessageBox::Critical);
        box.setStandardButtons(QMessageBox::Ok);
        box.buttons().at(0)->setFocusPolicy(Qt::NoFocus);
        box.exec();

        this->lineEditAddress->setFocus();
        this->lineEditAddress->selectAll();

        return;
    }

    this->device = new CasparDevice(this);

    QObject::connect(this->device, SIGNAL(connectionStateChanged(CasparDevice&)), this, SLOT(deviceConnectionStateChanged(CasparDevice&)));
    QObject::connect(this->device, SIGNAL(versionChanged(const CasparVersion&, CasparDevice&)), this, SLOT(deviceVersionChanged(const CasparVersion&, CasparDevice&)));

    if (this->lineEditPort->text().isEmpty())
        this->device->connect(this->lineEditAddress->text());
    else
        this->device->connect(this->lineEditAddress->text(), this->lineEditPort->text().toInt());
}

const QString AddDeviceDialog::getName() const
{
    return this->lineEditDeviceName->text();
}

const QString AddDeviceDialog::getAddress() const
{
    return this->lineEditAddress->text();
}

const QString AddDeviceDialog::getPort() const
{
    if (!this->lineEditPort->text().isEmpty())
        return this->lineEditPort->text();
    else
        return this->lineEditPort->placeholderText();
}

const QString AddDeviceDialog::getUsername() const
{
    return this->lineEditUsername->text();
}

const QString AddDeviceDialog::getPassword() const
{
    return this->lineEditPassword->text();
}

const QString AddDeviceDialog::getDescription() const
{
    return this->lineEditDescription->text();
}

const QString AddDeviceDialog::getVersion() const
{
    if (!this->lineEditVersion->text().isEmpty())
        return this->lineEditVersion->text();
    else
        return this->lineEditVersion->placeholderText();
}

const QString AddDeviceDialog::getShadow() const
{
    return this->checkBoxShadow->checkState() == Qt::Checked ? "Yes" : "No";
}

void AddDeviceDialog::deviceConnectionStateChanged(CasparDevice& device)
{
    if (device.isConnected())
        device.refreshVersion();
}

void AddDeviceDialog::deviceVersionChanged(const CasparVersion& version, CasparDevice& device)
{
    this->pushButtonOK->setEnabled(true);
    this->lineEditVersion->setText(version.getVersion());

    device.disconnect();
}
