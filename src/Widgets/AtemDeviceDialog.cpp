#include "AtemDeviceDialog.h"

#include "DatabaseManager.h"

#include <QtCore/QDebug>

#include <QtGui/QAbstractButton>
#include <QtGui/QCloseEvent>
#include <QtGui/QMessageBox>

AtemDeviceDialog::AtemDeviceDialog(QWidget* parent)
    : QDialog(parent),
      editMode(false)
{
    setupUi(this);

    this->lineEditDeviceName->setStyleSheet("border-color: firebrick;");
    this->lineEditAddress->setStyleSheet("border-color: firebrick;");

    this->lineEditDeviceName->installEventFilter(this);
}

bool AtemDeviceDialog::eventFilter(QObject* target, QEvent* event)
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

void AtemDeviceDialog::setDeviceModel(const AtemDeviceModel& model)
{
    this->editMode = true;

    setWindowTitle("Edit ATEM Mixer");

    this->lineEditDeviceName->setText(model.getName());
    this->lineEditAddress->setText(model.getAddress());
    this->lineEditDescription->setText(model.getDescription());
}

const QString AtemDeviceDialog::getName() const
{
    return this->lineEditDeviceName->text();
}

const QString AtemDeviceDialog::getAddress() const
{
    return this->lineEditAddress->text();
}

const QString AtemDeviceDialog::getDescription() const
{
    return this->lineEditDescription->text();
}

void AtemDeviceDialog::accept()
{
    if (this->lineEditDeviceName->text().isEmpty() || this->lineEditAddress->text().isEmpty())
        return;

    if (!this->editMode)
    {
        AtemDeviceModel model = DatabaseManager::getInstance().getAtemDeviceByName(this->lineEditDeviceName->text());
        if (!model.getName().isEmpty())
        {
            QMessageBox box(this);
            box.setWindowTitle("Add ATEM Mixer");
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

        model = DatabaseManager::getInstance().getAtemDeviceByAddress(this->lineEditAddress->text());
        if (!model.getName().isEmpty())
        {
            QMessageBox box(this);
            box.setWindowTitle("Add ATEM Mixer");
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

void AtemDeviceDialog::testConnection()
{
    this->device = QSharedPointer<AtemDevice>(new AtemDevice(this->lineEditAddress->text()));

    QObject::connect(this->device.data(), SIGNAL(connectionStateChanged(AtemDevice&)), this, SLOT(connectionStateChanged(AtemDevice&)));
    this->device->connectDevice();
}

void AtemDeviceDialog::nameChanged(QString name)
{
    if (this->lineEditDeviceName->text().isEmpty())
        this->lineEditDeviceName->setStyleSheet("border-color: firebrick;");
    else
        this->lineEditDeviceName->setStyleSheet("");
}

void AtemDeviceDialog::addressChanged(QString name)
{
    if (this->lineEditAddress->text().isEmpty())
        this->lineEditAddress->setStyleSheet("border-color: firebrick;");
    else
        this->lineEditAddress->setStyleSheet("");
}

void AtemDeviceDialog::connectionStateChanged(AtemDevice& device)
{
    QObject::disconnect(this->device.data(), SIGNAL(connectionStateChanged(AtemDevice&)), this, SLOT(connectionStateChanged(AtemDevice&)));

    if (device.isConnected())
    {
        QMessageBox box(this);
        box.setWindowTitle("Test Connection");
        box.setText(QString("Successfully connected to ATEM mixer: %1").arg(device.getAddress()));
        box.setIconPixmap(QPixmap(":/Graphics/Images/Information.png"));
        box.setStandardButtons(QMessageBox::Ok);
        box.buttons().at(0)->setFocusPolicy(Qt::NoFocus);
        box.exec();
    }

    this->device->disconnectDevice();
}
