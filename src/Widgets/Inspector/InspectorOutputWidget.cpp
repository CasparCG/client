#include "InspectorOutputWidget.h"

#include "Global.h"

#include "DeviceManager.h"
#include "EventManager.h"
#include "DatabaseManager.h"
#include "Commands/BlendModeCommand.h"
#include "Commands/GridCommand.h"
#include "Commands/BrightnessCommand.h"
#include "Commands/ClearOutputCommand.h"
#include "Commands/CommitCommand.h"
#include "Commands/ContrastCommand.h"
#include "Commands/CustomCommand.h"
#include "Commands/CropCommand.h"
#include "Commands/DeckLinkInputCommand.h"
#include "Commands/GeometryCommand.h"
#include "Commands/GpiOutputCommand.h"
#include "Commands/GroupCommand.h"
#include "Commands/FileRecorderCommand.h"
#include "Commands/KeyerCommand.h"
#include "Commands/LevelsCommand.h"
#include "Commands/OpacityCommand.h"
#include "Commands/PrintCommand.h"
#include "Commands/SaturationCommand.h"
#include "Commands/SeparatorCommand.h"
#include "Commands/SolidColorCommand.h"
#include "Commands/VolumeCommand.h"
#include "Commands/ChromaCommand.h"
#include "Events/Library/LibraryItemSelectedEvent.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Events/Rundown/EmptyRundownEvent.h"

#include <QtGui/QApplication>
#include <QtGui/QLineEdit>

InspectorOutputWidget::InspectorOutputWidget(QWidget* parent)
    : QWidget(parent),
      command(NULL), model(NULL)
{
    setupUi(this);

    this->comboBoxTarget->lineEdit()->setStyleSheet("background-color: transparent; border-width: 0px;");

    QObject::connect(&DeviceManager::getInstance(), SIGNAL(deviceRemoved()), this, SLOT(deviceRemoved()));
    QObject::connect(&DeviceManager::getInstance(), SIGNAL(deviceAdded(CasparDevice&)), this, SLOT(deviceAdded(CasparDevice&)));

    qApp->installEventFilter(this);
}

bool InspectorOutputWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Event::EventType::LibraryItemSelected))
    {
        LibraryItemSelectedEvent* libraryItemSelectedEvent = dynamic_cast<LibraryItemSelectedEvent*>(event);
        this->model = libraryItemSelectedEvent->getLibraryModel();

        blockAllSignals(true);

        this->comboBoxDevice->setEnabled(false);
        this->comboBoxTarget->setEnabled(false);
        this->spinBoxChannel->setEnabled(false);
        this->spinBoxVideolayer->setEnabled(false);
        this->spinBoxDelay->setEnabled(false);

        this->comboBoxDevice->setCurrentIndex(this->comboBoxDevice->findText(this->model->getDeviceName()));

        fillTargetCombo(this->model->getType());

        checkEmptyDevice();
        checkEmptyTarget();

        blockAllSignals(false);
    }
    else if (event->type() == static_cast<QEvent::Type>(Event::EventType::EmptyRundown))
    {
        blockAllSignals(true);

        this->comboBoxDevice->setEnabled(false);
        this->comboBoxDevice->setCurrentIndex(-1);
        this->comboBoxTarget->setEnabled(false);
        this->comboBoxTarget->clear();

        checkEmptyDevice();
        checkEmptyTarget();

        blockAllSignals(false);
    }
    else if (event->type() == static_cast<QEvent::Type>(Event::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        this->model = rundownItemSelectedEvent->getLibraryModel();

        blockAllSignals(true);

        this->comboBoxDevice->setEnabled(true);
        this->comboBoxTarget->setEnabled(true);
        this->spinBoxChannel->setEnabled(true);
        this->spinBoxVideolayer->setEnabled(true);
        this->spinBoxDelay->setEnabled(true);

        if (rundownItemSelectedEvent->getCommand() != NULL && rundownItemSelectedEvent->getLibraryModel() != NULL)
        {
            this->command = rundownItemSelectedEvent->getCommand();

            this->comboBoxDevice->setEnabled(true);
            this->comboBoxTarget->setEnabled(true);
            this->spinBoxChannel->setEnabled(true);
            this->spinBoxVideolayer->setEnabled(true);

            int index = this->comboBoxDevice->findText(this->model->getDeviceName());
            if (index == -1)
                this->spinBoxChannel->setMaximum(1);
            else
            {
                const QStringList& channelFormats = DatabaseManager::getInstance().getDeviceByName(this->model->getDeviceName()).getChannelFormats().split(",");
                this->spinBoxChannel->setMaximum(channelFormats.count());
            }

            this->comboBoxDevice->setCurrentIndex(index);
            this->spinBoxChannel->setValue(this->command->getChannel());
            this->spinBoxVideolayer->setValue(this->command->getVideolayer());
            this->spinBoxDelay->setValue(this->command->getDelay());
            this->checkBoxAllowGpi->setChecked(this->command->getAllowGpi());

            fillTargetCombo(this->model->getType());

            if (dynamic_cast<CommitCommand*>(rundownItemSelectedEvent->getCommand()) ||
                dynamic_cast<PrintCommand*>(rundownItemSelectedEvent->getCommand()) ||
                dynamic_cast<FileRecorderCommand*>(rundownItemSelectedEvent->getCommand()) ||
                dynamic_cast<GridCommand*>(rundownItemSelectedEvent->getCommand()))
            {
                this->comboBoxTarget->setEnabled(false);
                this->spinBoxVideolayer->setEnabled(false);

                this->spinBoxVideolayer->setValue(Output::DEFAULT_VIDEOLAYER);
            }
            else if (dynamic_cast<GroupCommand*>(rundownItemSelectedEvent->getCommand()) ||
                     dynamic_cast<SeparatorCommand*>(rundownItemSelectedEvent->getCommand()) ||
                     dynamic_cast<GpiOutputCommand*>(rundownItemSelectedEvent->getCommand()))
            {
                this->comboBoxDevice->setEnabled(false);
                this->comboBoxTarget->setEnabled(false);
                this->spinBoxChannel->setEnabled(false);
                this->spinBoxVideolayer->setEnabled(false);
                this->spinBoxDelay->setEnabled(false);

                this->comboBoxDevice->setCurrentIndex(-1);
                this->spinBoxChannel->setValue(Output::DEFAULT_CHANNEL);
                this->spinBoxVideolayer->setValue(Output::DEFAULT_VIDEOLAYER);
                this->spinBoxDelay->setValue(Output::DEFAULT_DELAY);
                this->checkBoxAllowGpi->setChecked(Output::DEFAULT_ALLOW_GPI);
            }
            else if (dynamic_cast<CustomCommand*>(rundownItemSelectedEvent->getCommand()))
            {
                this->comboBoxTarget->setEnabled(false);
                this->spinBoxChannel->setEnabled(false);
                this->spinBoxVideolayer->setEnabled(false);

                this->spinBoxChannel->setValue(Output::DEFAULT_CHANNEL);
                this->spinBoxVideolayer->setValue(Output::DEFAULT_VIDEOLAYER);
            }
            else if (dynamic_cast<DeckLinkInputCommand*>(rundownItemSelectedEvent->getCommand()) ||
                     dynamic_cast<BlendModeCommand*>(rundownItemSelectedEvent->getCommand()) ||
                     dynamic_cast<BrightnessCommand*>(rundownItemSelectedEvent->getCommand()) ||
                     dynamic_cast<ContrastCommand*>(rundownItemSelectedEvent->getCommand()) ||
                     dynamic_cast<CropCommand*>(rundownItemSelectedEvent->getCommand()) ||
                     dynamic_cast<GeometryCommand*>(rundownItemSelectedEvent->getCommand()) ||
                     dynamic_cast<KeyerCommand*>(rundownItemSelectedEvent->getCommand()) ||
                     dynamic_cast<LevelsCommand*>(rundownItemSelectedEvent->getCommand()) ||
                     dynamic_cast<OpacityCommand*>(rundownItemSelectedEvent->getCommand()) ||
                     dynamic_cast<SaturationCommand*>(rundownItemSelectedEvent->getCommand()) ||
                     dynamic_cast<VolumeCommand*>(rundownItemSelectedEvent->getCommand()) ||
                     dynamic_cast<SolidColorCommand*>(rundownItemSelectedEvent->getCommand()) ||
                     dynamic_cast<ClearOutputCommand*>(rundownItemSelectedEvent->getCommand()) ||
                     dynamic_cast<ChromaCommand*>(rundownItemSelectedEvent->getCommand()))
            {
                this->comboBoxTarget->setEnabled(false);
            }
        }

        checkEmptyDevice();
        checkEmptyTarget();

        blockAllSignals(false);
    }
    else if (event->type() == static_cast<QEvent::Type>(Event::EventType::DeviceChanged))
    {
        if (this->model == NULL)
            return true;

        DeviceChangedEvent* deviceChangedEvent = dynamic_cast<DeviceChangedEvent*>(event);
        if (!deviceChangedEvent->getDeviceName().isEmpty())
        {
            this->model->setDeviceName(deviceChangedEvent->getDeviceName());

            fillTargetCombo(this->model->getType());
        }

        checkEmptyDevice();
        checkEmptyTarget();
    }

    return QObject::eventFilter(target, event);
}

void InspectorOutputWidget::blockAllSignals(bool block)
{
    this->comboBoxDevice->blockSignals(block);
    this->comboBoxTarget->blockSignals(block);
    this->spinBoxChannel->blockSignals(block);
    this->spinBoxVideolayer->blockSignals(block);
    this->spinBoxDelay->blockSignals(block);
    this->checkBoxAllowGpi->blockSignals(block);
}

void InspectorOutputWidget::fillTargetCombo(const QString& type)
{
    this->comboBoxTarget->clear();

    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model->getDeviceName());
    if (device == NULL)
        return;

    const DeviceModel deviceModel = DeviceManager::getInstance().getDeviceModelByName(this->model->getDeviceName());

    QList<LibraryModel> models = DatabaseManager::getInstance().getLibraryByDeviceId(deviceModel.getId());
    if (models.count() == 0)
        return;

    foreach (LibraryModel model, models)
    {
        if (type == "DATA" && model.getType() == "DATA")
            this->comboBoxTarget->addItem(model.getName());
        else if (type == "MOVIE" && model.getType() == "MOVIE")
            this->comboBoxTarget->addItem(model.getName());
        else if (type == Rundown::AUDIO && model.getType() == Rundown::AUDIO)
            this->comboBoxTarget->addItem(model.getName());
        else if (type == Rundown::TEMPLATE && model.getType() == Rundown::TEMPLATE)
            this->comboBoxTarget->addItem(model.getName());
        else if ((type == Rundown::IMAGE || type == Rundown::IMAGESCROLLER) && model.getType() == "STILL")
            this->comboBoxTarget->addItem(model.getName());
    }

    this->comboBoxTarget->setCurrentIndex(this->comboBoxTarget->findText(this->model->getName()));
}

void InspectorOutputWidget::checkEmptyDevice()
{
    if (this->comboBoxDevice->isEnabled() && this->comboBoxDevice->currentText() == "")
        this->comboBoxDevice->setStyleSheet("border-color: red;");
    else
        this->comboBoxDevice->setStyleSheet("");
}

void InspectorOutputWidget::checkEmptyTarget()
{
    if (this->comboBoxTarget->isEnabled() && this->comboBoxTarget->currentText() == "")
        this->comboBoxTarget->setStyleSheet("border-color: red;");
    else
        this->comboBoxTarget->setStyleSheet("");
}

void InspectorOutputWidget::deviceRemoved()
{
    blockAllSignals(true);

    this->comboBoxDevice->clear();
    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
        this->comboBoxDevice->addItem(model.getName());

    blockAllSignals(false);
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
    const QStringList& channelFormats = DatabaseManager::getInstance().getDeviceByName(deviceName).getChannelFormats().split(",");
    this->spinBoxChannel->setMaximum(channelFormats.count());

    checkEmptyDevice();
    checkEmptyTarget();

    EventManager::getInstance().fireDeviceChangedEvent(this->comboBoxDevice->currentText());
}

void InspectorOutputWidget::targetChanged(QString name)
{
    checkEmptyTarget();

    EventManager::getInstance().fireTargetChangedEvent(this->comboBoxTarget->currentText());
}

void InspectorOutputWidget::channelChanged(int channel)
{
    this->command->setChannel(channel);

    EventManager::getInstance().fireChannelChangedEvent(channel);
}

void InspectorOutputWidget::videolayerChanged(int videolayer)
{
    this->command->setVideolayer(videolayer);

    EventManager::getInstance().fireVideolayerChangedEvent(videolayer);
}

void InspectorOutputWidget::delayChanged(int delay)
{
    this->command->setDelay(delay);
}

void InspectorOutputWidget::allowGpiChanged(int state)
{
    this->command->setAllowGpi((state == Qt::Checked) ? true : false);
}
