#include "TriCasterDeviceDialog.h"

#include "DatabaseManager.h"

#include <QtCore/QDebug>

#include <QtGui/QAbstractButton>
#include <QtGui/QCloseEvent>
#include <QtGui/QMessageBox>

TriCasterDeviceDialog::TriCasterDeviceDialog(QWidget* parent)
    : QDialog(parent),
      editMode(false)
{
    setupUi(this);

    this->lineEditDeviceName->setStyleSheet("border-color: firebrick;");
    this->lineEditAddress->setStyleSheet("border-color: firebrick;");

    this->lineEditDeviceName->installEventFilter(this);
}

bool TriCasterDeviceDialog::eventFilter(QObject* target, QEvent* event)
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

void TriCasterDeviceDialog::setDeviceModel(const TriCasterDeviceModel& model)
{
    this->editMode = true;

    setWindowTitle("Edit TriCaster Mixer");

    this->lineEditDeviceName->setText(model.getName());
    this->lineEditAddress->setText(model.getAddress());
    this->lineEditPort->setText(QString("%1").arg(model.getPort()));
    this->lineEditDescription->setText(model.getDescription());
}

const QString TriCasterDeviceDialog::getName() const
{
    return this->lineEditDeviceName->text();
}

const QString TriCasterDeviceDialog::getAddress() const
{
    return this->lineEditAddress->text();
}

const QString TriCasterDeviceDialog::getPort() const
{
    if (!this->lineEditPort->text().isEmpty())
        return this->lineEditPort->text();
    else
        return this->lineEditPort->placeholderText();
}

const QString TriCasterDeviceDialog::getDescription() const
{
    return this->lineEditDescription->text();
}

void TriCasterDeviceDialog::accept()
{
    if (this->lineEditDeviceName->text().isEmpty() || this->lineEditAddress->text().isEmpty())
        return;

    if (!this->editMode)
    {
        TriCasterDeviceModel model = DatabaseManager::getInstance().getTriCasterDeviceByName(this->lineEditDeviceName->text());
        if (!model.getName().isEmpty())
        {
            QMessageBox box(this);
            box.setWindowTitle("Add TriCaster Mixer");
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

        model = DatabaseManager::getInstance().getTriCasterDeviceByAddress(this->lineEditAddress->text());
        if (!model.getName().isEmpty())
        {
            QMessageBox box(this);
            box.setWindowTitle("Add TriCaster Mixer");
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
    }

    QDialog::accept();
}

void TriCasterDeviceDialog::testConnection()
{
    if (this->lineEditPort->text().isEmpty())
        this->device = QSharedPointer<TriCasterDevice>(new TriCasterDevice(this->lineEditAddress->text()));
    else
        this->device = QSharedPointer<TriCasterDevice>(new TriCasterDevice(this->lineEditAddress->text(), this->lineEditPort->text().toInt()));

    QObject::connect(this->device.data(), SIGNAL(connectionStateChanged(TriCasterDevice&)), this, SLOT(connectionStateChanged(TriCasterDevice&)));
    this->device->connectDevice();
}

void TriCasterDeviceDialog::nameChanged(QString name)
{
    if (this->lineEditDeviceName->text().isEmpty())
        this->lineEditDeviceName->setStyleSheet("border-color: firebrick;");
    else
        this->lineEditDeviceName->setStyleSheet("");
}

void TriCasterDeviceDialog::addressChanged(QString name)
{
    if (this->lineEditAddress->text().isEmpty())
        this->lineEditAddress->setStyleSheet("border-color: firebrick;");
    else
        this->lineEditAddress->setStyleSheet("");
}

void TriCasterDeviceDialog::connectionStateChanged(TriCasterDevice& device)
{
    QObject::disconnect(this->device.data(), SIGNAL(connectionStateChanged(TriCasterDevice&)), this, SLOT(connectionStateChanged(TriCasterDevice&)));

    if (device.isConnected())
    {
        QMessageBox box(this);
        box.setWindowTitle("Test Connection");
        box.setText(QString("Successfully connected to TriCaster mixer: %1:%2").arg(device.getAddress()).arg(device.getPort()));
        box.setIconPixmap(QPixmap(":/Graphics/Images/Information.png"));
        box.setStandardButtons(QMessageBox::Ok);
        box.buttons().at(0)->setFocusPolicy(Qt::NoFocus);
        box.exec();
    }

    this->device->disconnectDevice();
}
