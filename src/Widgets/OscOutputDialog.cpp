#include "OscOutputDialog.h"

#include "DatabaseManager.h"

#include <QtCore/QDebug>

#include <QtGui/QCloseEvent>

#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QMessageBox>

OscOutputDialog::OscOutputDialog(QWidget* parent)
    : QDialog(parent),
      editMode(false)
{
    setupUi(this);

    this->lineEditDeviceName->setStyleSheet("border-color: firebrick;");
    this->lineEditAddress->setStyleSheet("border-color: firebrick;");

    this->lineEditDeviceName->installEventFilter(this);
}

bool OscOutputDialog::eventFilter(QObject* target, QEvent* event)
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

void OscOutputDialog::setDeviceModel(const OscOutputModel& model)
{
    this->editMode = true;

    setWindowTitle("Edit OSC Output");

    this->lineEditDeviceName->setText(model.getName());
    this->lineEditAddress->setText(model.getAddress());
    this->lineEditPort->setText(QString("%1").arg(model.getPort()));
    this->lineEditDescription->setText(model.getDescription());
}

const QString OscOutputDialog::getName() const
{
    return this->lineEditDeviceName->text();
}

const QString OscOutputDialog::getAddress() const
{
    return this->lineEditAddress->text();
}

const QString OscOutputDialog::getPort() const
{
    if (!this->lineEditPort->text().isEmpty())
        return this->lineEditPort->text();
    else
        return this->lineEditPort->placeholderText();
}

const QString OscOutputDialog::getDescription() const
{
    return this->lineEditDescription->text();
}

void OscOutputDialog::accept()
{
    if (!lookupName(this->lineEditDeviceName->text()) || !lookupAddress(this->lineEditAddress->text()))
        return;

    if (!this->editMode)
    {
        OscOutputModel model = DatabaseManager::getInstance().getOscOutputByName(this->lineEditDeviceName->text());
        if (!model.getName().isEmpty())
        {
            QMessageBox box(this);
            box.setWindowTitle("Add OSC Output");
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
        model = DatabaseManager::getInstance().getOscOutputByAddress(this->lineEditAddress->text());
        if (!model.getName().isEmpty())
        {
            QMessageBox box(this);
            box.setWindowTitle("Add OSC Output");
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

void OscOutputDialog::nameChanged(QString name)
{
    Q_UNUSED(name);

    if (this->lineEditDeviceName->text().isEmpty() || !lookupName(this->lineEditDeviceName->text()))
        this->lineEditDeviceName->setStyleSheet("border-color: firebrick;");
    else
        this->lineEditDeviceName->setStyleSheet("");
}

bool OscOutputDialog::lookupName(const QString& name)
{
    if (this->editMode)
        return true;

    if (name.isEmpty())
        return false;

    OscOutputModel model = DatabaseManager::getInstance().getOscOutputByName(name);
    if (model.getName() == name)
        return false;

    return true;
}


void OscOutputDialog::addressChanged(QString name)
{
    Q_UNUSED(name);

    if (this->lineEditAddress->text().isEmpty() || !lookupAddress(this->lineEditAddress->text()))
        this->lineEditAddress->setStyleSheet("border-color: firebrick;");
    else
        this->lineEditAddress->setStyleSheet("");
}

bool OscOutputDialog::lookupAddress(const QString& address)
{
    if (this->editMode)
        return true;

    if (address.isEmpty())
        return false;

    OscOutputModel model = DatabaseManager::getInstance().getOscOutputByAddress(address);
    if (model.getAddress() == address)
        return false;

    return true;
}
