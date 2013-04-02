#include "InspectorOutputWidget.h"

#include "Global.h"

#include "DeviceManager.h"
#include "Commands/CommitCommand.h"
#include "Commands/GpiOutputCommand.h"
#include "Commands/GroupCommand.h"
#include "Commands/FileRecorderCommand.h"
#include "Commands/SeparatorCommand.h"
#include "Commands/PrintCommand.h"
#include "Commands/GridCommand.h"
#include "Events/DeviceChangedEvent.h"
#include "Events/LibraryItemSelectedEvent.h"
#include "Events/RundownItemSelectedEvent.h"

#include <QtGui/QApplication>

InspectorOutputWidget::InspectorOutputWidget(QWidget* parent)
    : QWidget(parent),
      command(NULL), model(NULL)
{
    setupUi(this);

    this->animation = new BorderAnimation(this->comboBoxDevice, this);

    connect(&DeviceManager::getInstance(), SIGNAL(deviceRemoved()), this, SLOT(deviceRemoved()));
    connect(&DeviceManager::getInstance(), SIGNAL(deviceAdded(CasparDevice&)), this, SLOT(deviceAdded(CasparDevice&)));

    qApp->installEventFilter(this);
}

bool InspectorOutputWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::LibraryItemSelected))
    {
        LibraryItemSelectedEvent* libraryItemSelectedEvent = dynamic_cast<LibraryItemSelectedEvent*>(event);
        this->model = libraryItemSelectedEvent->getLibraryModel();

         blockAllSignals(true);

        this->comboBoxDevice->setEnabled(false);

        this->comboBoxDevice->setCurrentIndex(this->comboBoxDevice->findText(this->model->getDeviceName()));

        checkEmptyDevice();

         blockAllSignals(false);
    }
    else if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownIsEmpty))
    {
        blockAllSignals(true);

        this->comboBoxDevice->setEnabled(false);

        this->comboBoxDevice->setCurrentIndex(-1);

        checkEmptyDevice();

        blockAllSignals(false);
    }
    else if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        this->model = rundownItemSelectedEvent->getLibraryModel();

        blockAllSignals(true);

        this->comboBoxDevice->setEnabled(true);
        this->spinBoxChannel->setEnabled(true);
        this->spinBoxVideolayer->setEnabled(true);
        this->spinBoxDelay->setEnabled(true);

        if (rundownItemSelectedEvent->getCommand() != NULL && rundownItemSelectedEvent->getLibraryModel() != NULL)
        {
            this->command = rundownItemSelectedEvent->getCommand();

            this->comboBoxDevice->setEnabled(true);
            this->spinBoxChannel->setEnabled(true);
            this->spinBoxVideolayer->setEnabled(true);

            this->comboBoxDevice->setCurrentIndex(this->comboBoxDevice->findText(this->model->getDeviceName()));
            this->spinBoxChannel->setValue(this->command->getChannel());
            this->spinBoxVideolayer->setValue(this->command->getVideolayer());
            this->spinBoxDelay->setValue(this->command->getDelay());
            this->checkBoxAllowGpi->setChecked(this->command->getAllowGpi());

            if (dynamic_cast<CommitCommand*>(rundownItemSelectedEvent->getCommand()) ||
                dynamic_cast<PrintCommand*>(rundownItemSelectedEvent->getCommand()) ||
                dynamic_cast<FileRecorderCommand*>(rundownItemSelectedEvent->getCommand()) ||
                dynamic_cast<GridCommand*>(rundownItemSelectedEvent->getCommand()))
            {
                this->spinBoxVideolayer->setEnabled(false);

                this->spinBoxVideolayer->setValue(Output::DEFAULT_VIDEOLAYER);
            }
            else if (dynamic_cast<GroupCommand*>(rundownItemSelectedEvent->getCommand()) ||
                     dynamic_cast<SeparatorCommand*>(rundownItemSelectedEvent->getCommand()) ||
                     dynamic_cast<GpiOutputCommand*>(rundownItemSelectedEvent->getCommand()))
            {
                this->comboBoxDevice->setEnabled(false);
                this->spinBoxChannel->setEnabled(false);
                this->spinBoxVideolayer->setEnabled(false);
                this->spinBoxDelay->setEnabled(false);

                this->comboBoxDevice->setCurrentIndex(-1);
                this->spinBoxChannel->setValue(Output::DEFAULT_CHANNEL);
                this->spinBoxVideolayer->setValue(Output::DEFAULT_VIDEOLAYER);
                this->spinBoxDelay->setValue(Output::DEFAULT_DELAY);
                this->checkBoxAllowGpi->setChecked(Output::DEFAULT_ALLOW_GPI);
            } 
        }

        checkEmptyDevice();

        blockAllSignals(false);
    }

    return QObject::eventFilter(target, event);
}

void InspectorOutputWidget::blockAllSignals(bool block)
{
    this->comboBoxDevice->blockSignals(block);
    this->spinBoxChannel->blockSignals(block);
    this->spinBoxVideolayer->blockSignals(block);
    this->spinBoxDelay->blockSignals(block);
    this->checkBoxAllowGpi->blockSignals(block);
}

void InspectorOutputWidget::checkEmptyDevice()
{
    (this->comboBoxDevice->isEnabled() && this->comboBoxDevice->currentText() == "") ? this->animation->start() : this->animation->stop();
}

void InspectorOutputWidget::deviceRemoved()
{
    this->comboBoxDevice->clear();
    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
        this->comboBoxDevice->addItem(model.getName());
}

void InspectorOutputWidget::deviceAdded(CasparDevice& device)
{
    int index = this->comboBoxDevice->currentIndex();

    this->comboBoxDevice->addItem(DeviceManager::getInstance().getDeviceModelByAddress(device.getAddress()).getName());

    if (index == -1)
        this->comboBoxDevice->setCurrentIndex(index);
}

void InspectorOutputWidget::deviceNameChanged(QString deviceName)
{
    checkEmptyDevice();

    qApp->postEvent(qApp, new DeviceChangedEvent(this->comboBoxDevice->currentText()));
}

void InspectorOutputWidget::allowGpiChanged(int state)
{
    this->command->setAllowGpi((state == Qt::Checked) ? true : false);
}

void InspectorOutputWidget::channelChanged(int channel)
{
    this->command->setChannel(channel);
}

void InspectorOutputWidget::videolayerChanged(int videolayer)
{
    this->command->setVideolayer(videolayer);
}

void InspectorOutputWidget::delayChanged(int delay)
{
    this->command->setDelay(delay);
}

void InspectorOutputWidget::resetAllowGpi(QString)
{
    this->checkBoxAllowGpi->setChecked(Output::DEFAULT_ALLOW_GPI);
    this->command->setAllowGpi(this->checkBoxAllowGpi->isChecked());
}

void InspectorOutputWidget::resetChannel(QString channel)
{
    this->spinBoxChannel->setValue(Output::DEFAULT_CHANNEL);
    this->command->setChannel(this->spinBoxChannel->value());
}

void InspectorOutputWidget::resetVideolayer(QString videolayer)
{
    this->spinBoxVideolayer->setValue(Output::DEFAULT_VIDEOLAYER);
    this->command->setVideolayer(this->spinBoxVideolayer->value());
}

void InspectorOutputWidget::resetDelay(QString delay)
{
    this->spinBoxDelay->setValue(Output::DEFAULT_DELAY);
    this->command->setDelay(this->spinBoxDelay->value());
}
