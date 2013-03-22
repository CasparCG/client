#include "AddDeviceDialog.h"

#include "DatabaseManager.h"

#include <QtCore/QDebug>

#include <QtGui/QAbstractButton>
#include <QtGui/QCloseEvent>
#include <QtGui/QMessageBox>

AddDeviceDialog::AddDeviceDialog(QWidget* parent)
    : QDialog(parent), editMode(false)
{
    setupUi(this);

    this->addressAnimation = new BorderAnimation(this->lineEditAddress);
    this->deviceNameAnimation = new BorderAnimation(this->lineEditDeviceName);
    this->addressAnimation->start();
    this->deviceNameAnimation->start();
}

void AddDeviceDialog::setDeviceModel(const DeviceModel& model)
{
    this->lineEditDeviceName->setText(model.getName());
    this->lineEditAddress->setText(model.getAddress());
    this->lineEditPort->setText(QString("%1").arg(model.getPort()));
    this->lineEditUsername->setText(model.getUsername());
    this->lineEditPassword->setText(model.getPassword());
    this->lineEditDescription->setText(model.getDescription());
    this->checkBoxShadow->setChecked((model.getShadow() == "Yes") ? true : false);
}

void AddDeviceDialog::setEditMode(bool editMode)
{
    this->editMode = editMode;
    this->lineEditDeviceName->setEnabled(!this->editMode);
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

const QString AddDeviceDialog::getShadow() const
{
    return this->checkBoxShadow->checkState() == Qt::Checked ? "Yes" : "No";
}

void AddDeviceDialog::accept()
{
    if (this->lineEditDeviceName->text().isEmpty() || this->lineEditAddress->text().isEmpty())
        return;

    if (!this->editMode)
    {
        DeviceModel model = DatabaseManager::getInstance().getDeviceByName(this->lineEditDeviceName->text());
        if (!model.getName().isEmpty())
        {
            QMessageBox box;
            box.setWindowTitle("Add Device");
            box.setText("The name already exists in the database. Please choose a unique name.");
            box.setIconPixmap(QPixmap(":/Graphics/Images/Attention.png"));
            box.setStandardButtons(QMessageBox::Ok);
            box.buttons().at(0)->setFocusPolicy(Qt::NoFocus);
            box.exec();

            this->lineEditDeviceName->setFocus();
            this->lineEditDeviceName->selectAll();

            return;
        }

        model = DatabaseManager::getInstance().getDeviceByAddress(this->lineEditAddress->text());
        if (!model.getName().isEmpty())
        {
            QMessageBox box;
            box.setWindowTitle("Add Device");
            box.setText("The address already exists in the database. Please choose a unique address.");
            box.setIconPixmap(QPixmap(":/Graphics/Images/Attention.png"));
            box.setStandardButtons(QMessageBox::Ok);
            box.buttons().at(0)->setFocusPolicy(Qt::NoFocus);
            box.exec();

            this->lineEditAddress->setFocus();
            this->lineEditAddress->selectAll();

            return;
        }
    }

    QDialog::accept();
}

void AddDeviceDialog::testConnection()
{
    this->device = QSharedPointer<CasparDevice>(new CasparDevice());

    QObject::connect(this->device.data(), SIGNAL(connectionStateChanged(CasparDevice&)), this, SLOT(connectionStateChanged(CasparDevice&)));

    if (this->lineEditPort->text().isEmpty())
        this->device->connectDevice(this->lineEditAddress->text());
    else
        this->device->connectDevice(this->lineEditAddress->text(), this->lineEditPort->text().toInt());
}

void AddDeviceDialog::nameChanged(QString name)
{
    if (!this->lineEditDeviceName->text().isEmpty())
        this->deviceNameAnimation->stop();
    else
        this->deviceNameAnimation->start();
}

void AddDeviceDialog::addressChanged(QString name)
{
    if (!this->lineEditAddress->text().isEmpty())
        this->addressAnimation->stop();
    else
        this->addressAnimation->start();
}

void AddDeviceDialog::connectionStateChanged(CasparDevice& device)
{
    this->device->disconnect();

    QMessageBox box;
    box.setWindowTitle("Test Connection");
    box.setText(QString("Successfully connected to device: %1:%2").arg(device.getAddress()).arg(device.getPort()));
    box.setIconPixmap(QPixmap(":/Graphics/Images/Information.png"));
    box.setStandardButtons(QMessageBox::Ok);
    box.buttons().at(0)->setFocusPolicy(Qt::NoFocus);
    box.exec();
}
