#include "InspectorOutputWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "DeviceManager.h"
#include "EventManager.h"
#include "AtemDeviceManager.h"
#include "TriCasterDeviceManager.h"
#include "Commands/BlendModeCommand.h"
#include "Commands/GridCommand.h"
#include "Commands/BrightnessCommand.h"
#include "Commands/ClearOutputCommand.h"
#include "Commands/CommitCommand.h"
#include "Commands/ResetCommand.h"
#include "Commands/ContrastCommand.h"
#include "Commands/CustomCommand.h"
#include "Commands/ClipCommand.h"
#include "Commands/DeckLinkInputCommand.h"
#include "Commands/FillCommand.h"
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
#include "Commands/FadeToBlackCommand.h"
#include "Commands/VolumeCommand.h"
#include "Commands/ChromaCommand.h"
#include "Commands/PlayoutCommand.h"
#include "Commands/RotationCommand.h"
#include "Commands/OscOutputCommand.h"
#include "Commands/PerspectiveCommand.h"
#include "Commands/AnchorCommand.h"
#include "Commands/CropCommand.h"
#include "Commands/HtmlCommand.h"
#include "Commands/HttpGetCommand.h"
#include "Commands/HttpPostCommand.h"
#include "Commands/RouteChannelCommand.h"
#include "Commands/RouteVideolayerCommand.h"
#include "Commands/Atem/AtemInputCommand.h"
#include "Commands/Atem/AtemAudioInputStateCommand.h"
#include "Commands/Atem/AtemCutCommand.h"
#include "Commands/Atem/AtemAutoCommand.h"
#include "Commands/Atem/AtemKeyerStateCommand.h"
#include "Commands/Atem/AtemVideoFormatCommand.h"
#include "Commands/Atem/AtemAudioInputBalanceCommand.h"
#include "Commands/Atem/AtemAudioGainCommand.h"
#include "Commands/TriCaster/AutoCommand.h"
#include "Commands/TriCaster/InputCommand.h"
#include "Commands/TriCaster/PresetCommand.h"
#include "Commands/TriCaster/TakeCommand.h"
#include "Commands/TriCaster/NetworkSourceCommand.h"
#include "Commands/TriCaster/MacroCommand.h"
#include "Events/Inspector/ChannelChangedEvent.h"
#include "Events/Inspector/LabelChangedEvent.h"
#include "Events/Inspector/TargetChangedEvent.h"
#include "Events/Inspector/VideolayerChangedEvent.h"

#include <QtGui/QApplication>
#include <QtGui/QLineEdit>

InspectorOutputWidget::InspectorOutputWidget(QWidget* parent)
    : QWidget(parent),
      command(NULL), model(NULL), delayType(""), libraryFilter("")
{
    setupUi(this);

    this->comboBoxAtemDevice->setVisible(false);
    this->comboBoxTriCasterDevice->setVisible(false);

    this->delayType = DatabaseManager::getInstance().getConfigurationByName("DelayType").getValue();

    this->comboBoxTarget->lineEdit()->setStyleSheet("background-color: transparent; border-width: 0px;");

    QObject::connect(&DeviceManager::getInstance(), SIGNAL(deviceRemoved()), this, SLOT(deviceRemoved()));
    QObject::connect(&DeviceManager::getInstance(), SIGNAL(deviceAdded(CasparDevice&)), this, SLOT(deviceAdded(CasparDevice&)));

    QObject::connect(&AtemDeviceManager::getInstance(), SIGNAL(deviceRemoved()), this, SLOT(atemDeviceRemoved()));
    QObject::connect(&AtemDeviceManager::getInstance(), SIGNAL(deviceAdded(AtemDevice&)), this, SLOT(atemDeviceAdded(AtemDevice&)));

    QObject::connect(&TriCasterDeviceManager::getInstance(), SIGNAL(deviceRemoved()), this, SLOT(tricasterDeviceRemoved()));
    QObject::connect(&TriCasterDeviceManager::getInstance(), SIGNAL(deviceAdded(TriCasterDevice&)), this, SLOT(tricasterDeviceAdded(TriCasterDevice&)));

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(libraryItemSelected(const LibraryItemSelectedEvent&)), this, SLOT(libraryItemSelected(const LibraryItemSelectedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(emptyRundown(const EmptyRundownEvent&)), this, SLOT(emptyRundown(const EmptyRundownEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(deviceChanged(const DeviceChangedEvent&)), this, SLOT(deviceChanged(const DeviceChangedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(mediaChanged(const MediaChangedEvent&)), this, SLOT(mediaChanged(const MediaChangedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(templateChanged(const TemplateChangedEvent&)), this, SLOT(templateChanged(const TemplateChangedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(libraryFilterChanged(const LibraryFilterChangedEvent&)), this, SLOT(libraryFilterChanged(const LibraryFilterChangedEvent&)));
}

void InspectorOutputWidget::libraryFilterChanged(const LibraryFilterChangedEvent& event)
{
    this->libraryFilter = event.getFilter();
}

void InspectorOutputWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    this->comboBoxDevice->setVisible(true);
    this->comboBoxAtemDevice->setVisible(false);
    this->comboBoxTriCasterDevice->setVisible(false);

    this->comboBoxDevice->setEnabled(true);
    this->comboBoxTarget->setEnabled(true);
    this->spinBoxChannel->setEnabled(true);
    this->spinBoxVideolayer->setEnabled(true);
    this->spinBoxDelay->setEnabled(true);
    this->spinBoxDuration->setEnabled(true);
    this->checkBoxAllowGpi->setEnabled(true);
    this->checkBoxAllowRemoteTriggering->setEnabled(true);
    this->labelRemoteTriggerIdField->setEnabled(true);
    this->lineEditRemoteTriggerId->setEnabled(true);

    this->labelDelayMillisecond->setVisible(true);
    this->labelDurationMillisecond->setVisible(true);
    if (this->delayType == Output::DEFAULT_DELAY_IN_FRAMES)
    {
        this->labelDelayMillisecond->setText("frm");
        this->labelDurationMillisecond->setText("frm");
    }
    else if (this->delayType == Output::DEFAULT_DELAY_IN_MILLISECONDS)
    {
        this->labelDelayMillisecond->setText("ms");
        this->labelDurationMillisecond->setText("ms");
    }

    if (event.getCommand() != NULL && event.getLibraryModel() != NULL)
    {
        this->command = event.getCommand();

        int index = this->comboBoxDevice->findText(this->model->getDeviceName());
        if (index == -1)
            this->spinBoxChannel->setMaximum(1);
        else
        {
            const QStringList& channelFormats = DatabaseManager::getInstance().getDeviceByName(this->model->getDeviceName()).getChannelFormats().split(",");
            this->spinBoxChannel->setMaximum(channelFormats.count());
        }

        this->comboBoxDevice->setCurrentIndex(index);
        this->comboBoxAtemDevice->setCurrentIndex(this->comboBoxAtemDevice->findText(this->model->getDeviceName()));
        this->comboBoxTriCasterDevice->setCurrentIndex(this->comboBoxTriCasterDevice->findText(this->model->getDeviceName()));
        this->spinBoxChannel->setValue(this->command->getChannel());
        this->spinBoxVideolayer->setValue(this->command->getVideolayer());
        this->spinBoxDelay->setValue(this->command->getDelay());
        this->spinBoxDuration->setValue(this->command->getDuration());
        this->checkBoxAllowGpi->setChecked(this->command->getAllowGpi());
        this->checkBoxAllowRemoteTriggering->setChecked(this->command->getAllowRemoteTriggering());
        this->lineEditRemoteTriggerId->setText(this->command->getRemoteTriggerId());

        if (!this->checkBoxAllowRemoteTriggering->isChecked())
        {
            this->labelRemoteTriggerIdField->setEnabled(false);
            this->lineEditRemoteTriggerId->setEnabled(false);
        }

        fillTargetCombo(this->model->getType());

        if (dynamic_cast<FileRecorderCommand*>(event.getCommand()))
        {
            this->comboBoxTarget->setEnabled(false);
            this->spinBoxVideolayer->setEnabled(false);

            this->comboBoxTarget->setCurrentIndex(-1);
            this->spinBoxVideolayer->setValue(Output::DEFAULT_VIDEOLAYER);
        }
        else if (dynamic_cast<CommitCommand*>(event.getCommand()) ||
                 dynamic_cast<PrintCommand*>(event.getCommand()))
        {
            this->comboBoxTarget->setEnabled(false);
            this->spinBoxVideolayer->setEnabled(false);
            this->spinBoxDuration->setEnabled(false);

            this->comboBoxTarget->setCurrentIndex(-1);
            this->spinBoxVideolayer->setValue(Output::DEFAULT_VIDEOLAYER);
            this->spinBoxDuration->setValue(Output::DEFAULT_DURATION);
        }
        else if (dynamic_cast<GridCommand*>(event.getCommand()))
        {
            this->comboBoxTarget->setEnabled(false);
            this->spinBoxVideolayer->setEnabled(false);

            this->spinBoxVideolayer->setValue(Output::DEFAULT_VIDEOLAYER);
        }
        else if (dynamic_cast<GroupCommand*>(event.getCommand()))
        {
            this->comboBoxDevice->setEnabled(false);
            this->comboBoxTarget->setEnabled(false);
            this->spinBoxChannel->setEnabled(false);
            this->spinBoxVideolayer->setEnabled(false);
            this->spinBoxDelay->setEnabled(false);
            this->spinBoxDuration->setEnabled(false);

            this->labelDelayMillisecond->setText("ms");
            this->labelDurationMillisecond->setText("ms");

            this->comboBoxDevice->setCurrentIndex(-1);
            this->comboBoxTarget->setCurrentIndex(-1);
            this->spinBoxChannel->setValue(Output::DEFAULT_CHANNEL);
            this->spinBoxVideolayer->setValue(Output::DEFAULT_VIDEOLAYER);
            this->spinBoxDelay->setValue(Output::DEFAULT_DELAY);
        }
        else if (dynamic_cast<GpiOutputCommand*>(event.getCommand()) ||
                 dynamic_cast<OscOutputCommand*>(event.getCommand()) ||
                 dynamic_cast<HttpGetCommand*>(event.getCommand()) ||
                 dynamic_cast<HttpPostCommand*>(event.getCommand()) ||
                 dynamic_cast<PlayoutCommand*>(event.getCommand()))
        {
            this->comboBoxDevice->setEnabled(false);
            this->comboBoxTarget->setEnabled(false);
            this->spinBoxChannel->setEnabled(false);
            this->spinBoxVideolayer->setEnabled(false);
            this->spinBoxDuration->setEnabled(false);

            this->labelDelayMillisecond->setText("ms");
            this->labelDurationMillisecond->setText("ms");

            this->comboBoxDevice->setCurrentIndex(-1);
            this->comboBoxTarget->setCurrentIndex(-1);
            this->spinBoxChannel->setValue(Output::DEFAULT_CHANNEL);
            this->spinBoxVideolayer->setValue(Output::DEFAULT_VIDEOLAYER);
            this->spinBoxDuration->setValue(Output::DEFAULT_DURATION);
        }
        else if (dynamic_cast<SeparatorCommand*>(event.getCommand()))
        {
            this->comboBoxDevice->setEnabled(false);
            this->comboBoxTarget->setEnabled(false);
            this->spinBoxChannel->setEnabled(false);
            this->spinBoxVideolayer->setEnabled(false);
            this->spinBoxDelay->setEnabled(false);
            this->spinBoxDuration->setEnabled(false);
            this->checkBoxAllowGpi->setEnabled(false);
            this->checkBoxAllowRemoteTriggering->setEnabled(false);
            this->labelRemoteTriggerIdField->setEnabled(false);
            this->lineEditRemoteTriggerId->setEnabled(false);

            this->comboBoxDevice->setCurrentIndex(-1);
            this->comboBoxTarget->setCurrentIndex(-1);
            this->spinBoxChannel->setValue(Output::DEFAULT_CHANNEL);
            this->spinBoxVideolayer->setValue(Output::DEFAULT_VIDEOLAYER);
            this->spinBoxDelay->setValue(Output::DEFAULT_DELAY);
            this->spinBoxDuration->setValue(Output::DEFAULT_DURATION);
            this->checkBoxAllowGpi->setChecked(Output::DEFAULT_ALLOW_GPI);
            this->checkBoxAllowRemoteTriggering->setChecked(Output::DEFAULT_ALLOW_REMOTE_TRIGGERING);
            this->lineEditRemoteTriggerId->setText(Output::DEFAULT_REMOTE_TRIGGER_ID);
        }
        else if (dynamic_cast<CustomCommand*>(event.getCommand()))
        {
            this->comboBoxTarget->setEnabled(false);
            this->spinBoxChannel->setEnabled(false);
            this->spinBoxVideolayer->setEnabled(false);

            this->comboBoxTarget->setCurrentIndex(-1);
            this->spinBoxChannel->setValue(Output::DEFAULT_CHANNEL);
            this->spinBoxVideolayer->setValue(Output::DEFAULT_VIDEOLAYER);
        }
        else if (dynamic_cast<ClearOutputCommand*>(event.getCommand()) ||
                 dynamic_cast<ResetCommand*>(event.getCommand()))
        {
            this->comboBoxTarget->setEnabled(false);
            this->spinBoxDuration->setEnabled(false);

            this->comboBoxTarget->setCurrentIndex(-1);
            this->spinBoxDuration->setValue(Output::DEFAULT_DURATION);
        }
        else if (dynamic_cast<DeckLinkInputCommand*>(event.getCommand()) ||
                 dynamic_cast<BlendModeCommand*>(event.getCommand()) ||
                 dynamic_cast<BrightnessCommand*>(event.getCommand()) ||
                 dynamic_cast<ContrastCommand*>(event.getCommand()) ||
                 dynamic_cast<ClipCommand*>(event.getCommand()) ||
                 dynamic_cast<CropCommand*>(event.getCommand()) ||
                 dynamic_cast<FillCommand*>(event.getCommand()) ||
                 dynamic_cast<PerspectiveCommand*>(event.getCommand()) ||
                 dynamic_cast<RotationCommand*>(event.getCommand()) ||
                 dynamic_cast<AnchorCommand*>(event.getCommand()) ||
                 dynamic_cast<KeyerCommand*>(event.getCommand()) ||
                 dynamic_cast<LevelsCommand*>(event.getCommand()) ||
                 dynamic_cast<OpacityCommand*>(event.getCommand()) ||
                 dynamic_cast<SaturationCommand*>(event.getCommand()) ||
                 dynamic_cast<VolumeCommand*>(event.getCommand()) ||
                 dynamic_cast<SolidColorCommand*>(event.getCommand()) ||
                 dynamic_cast<FadeToBlackCommand*>(event.getCommand()) ||
                 dynamic_cast<HtmlCommand*>(event.getCommand()) ||
                 dynamic_cast<RouteChannelCommand*>(event.getCommand()) ||
                 dynamic_cast<RouteVideolayerCommand*>(event.getCommand()) ||
                 dynamic_cast<ChromaCommand*>(event.getCommand()))
        {
            this->comboBoxTarget->setEnabled(false);

            this->comboBoxTarget->setCurrentIndex(-1);
        }
        else if (dynamic_cast<InputCommand*>(event.getCommand()) ||
                 dynamic_cast<PresetCommand*>(event.getCommand()) ||
                 dynamic_cast<AutoCommand*>(event.getCommand()) ||
                 dynamic_cast<TakeCommand*>(event.getCommand()) ||
                 dynamic_cast<NetworkSourceCommand*>(event.getCommand()) ||
                 dynamic_cast<MacroCommand*>(event.getCommand()))
        {

            this->comboBoxTarget->setEnabled(false);
            this->spinBoxChannel->setEnabled(false);
            this->spinBoxVideolayer->setEnabled(false);
            this->spinBoxDuration->setEnabled(false);

            this->comboBoxDevice->setVisible(false);
            this->comboBoxAtemDevice->setVisible(false);
            this->comboBoxTriCasterDevice->setVisible(true);

            this->labelDelayMillisecond->setText("ms");
            this->labelDurationMillisecond->setText("ms");

            this->comboBoxDevice->setCurrentIndex(-1);
            this->comboBoxTarget->setCurrentIndex(-1);
            this->comboBoxAtemDevice->setCurrentIndex(-1);
            this->spinBoxChannel->setValue(Output::DEFAULT_CHANNEL);
            this->spinBoxVideolayer->setValue(Output::DEFAULT_VIDEOLAYER);
            this->spinBoxDuration->setValue(Output::DEFAULT_DURATION);
        }
        else if (dynamic_cast<AtemInputCommand*>(event.getCommand()) ||
                 dynamic_cast<AtemCutCommand*>(event.getCommand()) ||
                 dynamic_cast<AtemAutoCommand*>(event.getCommand()) ||
                 dynamic_cast<AtemKeyerStateCommand*>(event.getCommand()) ||
                 dynamic_cast<AtemVideoFormatCommand*>(event.getCommand()) ||
                 dynamic_cast<AtemAudioInputStateCommand*>(event.getCommand()) ||
                 dynamic_cast<AtemAudioGainCommand*>(event.getCommand()) ||
                 dynamic_cast<AtemAudioInputBalanceCommand*>(event.getCommand()))
        {
            this->comboBoxTarget->setEnabled(false);
            this->spinBoxChannel->setEnabled(false);
            this->spinBoxVideolayer->setEnabled(false);
            this->spinBoxDuration->setEnabled(false);

            this->comboBoxDevice->setVisible(false);
            this->comboBoxAtemDevice->setVisible(true);
            this->comboBoxTriCasterDevice->setVisible(false);

            this->labelDelayMillisecond->setText("ms");
            this->labelDurationMillisecond->setText("ms");

            this->comboBoxDevice->setCurrentIndex(-1);
            this->comboBoxTarget->setCurrentIndex(-1);
            this->comboBoxTriCasterDevice->setCurrentIndex(-1);
            this->spinBoxChannel->setValue(Output::DEFAULT_CHANNEL);
            this->spinBoxVideolayer->setValue(Output::DEFAULT_VIDEOLAYER);
            this->spinBoxDuration->setValue(Output::DEFAULT_DURATION);
        }
    }

    checkEmptyDevice();
    checkEmptyAtemDevice();
    checkEmptyTriCasterDevice();
    checkEmptyTarget();

    blockAllSignals(false);
}

void InspectorOutputWidget::libraryItemSelected(const LibraryItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    this->comboBoxDevice->setVisible(true);
    this->comboBoxAtemDevice->setVisible(false);
    this->comboBoxTriCasterDevice->setVisible(false);

    this->comboBoxDevice->setEnabled(false);
    this->comboBoxTarget->setEnabled(false);
    this->spinBoxChannel->setEnabled(false);
    this->spinBoxVideolayer->setEnabled(false);
    this->spinBoxDelay->setEnabled(false);
    this->spinBoxDuration->setEnabled(false);
    this->checkBoxAllowGpi->setEnabled(false);
    this->checkBoxAllowRemoteTriggering->setEnabled(false);
    this->labelRemoteTriggerIdField->setEnabled(false);
    this->lineEditRemoteTriggerId->setEnabled(false);

    this->labelDelayMillisecond->setText("");
    this->labelDelayMillisecond->setVisible(false);
    this->labelDurationMillisecond->setText("");
    this->labelDurationMillisecond->setVisible(false);

    this->comboBoxDevice->setCurrentIndex(this->comboBoxDevice->findText(this->model->getDeviceName()));
    this->comboBoxAtemDevice->setCurrentIndex(this->comboBoxAtemDevice->findText(this->model->getDeviceName()));
    this->comboBoxTriCasterDevice->setCurrentIndex(this->comboBoxTriCasterDevice->findText(this->model->getDeviceName()));
    this->checkBoxAllowGpi->setChecked(Output::DEFAULT_ALLOW_GPI);
    this->checkBoxAllowRemoteTriggering->setChecked(Output::DEFAULT_ALLOW_REMOTE_TRIGGERING);
    this->lineEditRemoteTriggerId->setText(Output::DEFAULT_REMOTE_TRIGGER_ID);

    fillTargetCombo(this->model->getType());

    checkEmptyDevice();
    checkEmptyAtemDevice();
    checkEmptyTriCasterDevice();
    checkEmptyTarget();

    blockAllSignals(false);
}

void InspectorOutputWidget::emptyRundown(const EmptyRundownEvent& event)
{
    blockAllSignals(true);

    this->model = NULL;

    this->comboBoxDevice->setVisible(true);
    this->comboBoxAtemDevice->setVisible(false);
    this->comboBoxTriCasterDevice->setVisible(false);

    this->comboBoxDevice->setEnabled(false);
    this->comboBoxTarget->setEnabled(false);
    this->spinBoxChannel->setEnabled(false);
    this->spinBoxVideolayer->setEnabled(false);
    this->spinBoxDelay->setEnabled(false);
    this->spinBoxDuration->setEnabled(false);
    this->checkBoxAllowGpi->setEnabled(false);
    this->checkBoxAllowRemoteTriggering->setEnabled(false);
    this->labelRemoteTriggerIdField->setEnabled(false);
    this->lineEditRemoteTriggerId->setEnabled(false);

    this->labelDelayMillisecond->setText("");
    this->labelDelayMillisecond->setVisible(false);
    this->labelDurationMillisecond->setText("");
    this->labelDurationMillisecond->setVisible(false);

    this->comboBoxDevice->setCurrentIndex(-1);
    this->comboBoxAtemDevice->setCurrentIndex(-1);
    this->comboBoxTriCasterDevice->setCurrentIndex(-1);
    this->comboBoxTarget->clear();
    this->spinBoxChannel->setValue(Output::DEFAULT_CHANNEL);
    this->spinBoxVideolayer->setValue(Output::DEFAULT_VIDEOLAYER);
    this->spinBoxDelay->setValue(Output::DEFAULT_DELAY);
    this->spinBoxDuration->setValue(Output::DEFAULT_DURATION);
    this->checkBoxAllowGpi->setChecked(Output::DEFAULT_ALLOW_GPI);
    this->checkBoxAllowRemoteTriggering->setChecked(Output::DEFAULT_ALLOW_REMOTE_TRIGGERING);
    this->lineEditRemoteTriggerId->setText(Output::DEFAULT_REMOTE_TRIGGER_ID);

    checkEmptyDevice();
    checkEmptyAtemDevice();
    checkEmptyTriCasterDevice();
    checkEmptyTarget();

    blockAllSignals(false);
}

void InspectorOutputWidget::deviceChanged(const DeviceChangedEvent& event)
{
    if (this->model == NULL)
        return;

    blockAllSignals(true);

    if (!event.getDeviceName().isEmpty())
        fillTargetCombo(this->model->getType(), event.getDeviceName());

    checkEmptyDevice();
    checkEmptyAtemDevice();
    checkEmptyTriCasterDevice();
    checkEmptyTarget();

    blockAllSignals(false);
}

void InspectorOutputWidget::mediaChanged(const MediaChangedEvent& event)
{
    if (this->model == NULL)
        return;

    blockAllSignals(true);

    fillTargetCombo(this->model->getType());

    blockAllSignals(false);
}

void InspectorOutputWidget::templateChanged(const TemplateChangedEvent& event)
{
    if (this->model == NULL)
        return;

    blockAllSignals(true);

    fillTargetCombo(this->model->getType());

    blockAllSignals(false);
}

void InspectorOutputWidget::blockAllSignals(bool block)
{
    this->comboBoxDevice->blockSignals(block);
    this->comboBoxAtemDevice->blockSignals(block);
    this->comboBoxTriCasterDevice->blockSignals(block);
    this->comboBoxTarget->blockSignals(block);
    this->spinBoxChannel->blockSignals(block);
    this->spinBoxVideolayer->blockSignals(block);
    this->spinBoxDelay->blockSignals(block);
    this->spinBoxDuration->blockSignals(block);
    this->checkBoxAllowGpi->blockSignals(block);
    this->checkBoxAllowRemoteTriggering->blockSignals(block);
    this->lineEditRemoteTriggerId->blockSignals(block);
}

void InspectorOutputWidget::fillTargetCombo(const QString& type, QString deviceName)
{
    if (this->model == NULL)
        return;

    if (deviceName.isEmpty())
        deviceName = this->model->getDeviceName();

    if (deviceName.isEmpty())
        return;

    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(deviceName);
    if (device == NULL)
        return;

    const QSharedPointer<DeviceModel> deviceModel = DeviceManager::getInstance().getDeviceModelByName(deviceName);
    if (deviceModel == NULL)
        return;

    QList<LibraryModel> models;
    if (this->libraryFilter.isEmpty())
        models = DatabaseManager::getInstance().getLibraryByDeviceId(deviceModel->getId());
    else
        models = DatabaseManager::getInstance().getLibraryByDeviceIdAndFilter(deviceModel->getId(), this->libraryFilter);

    if (models.count() > 0)
    {
        this->comboBoxTarget->clear();
        foreach (LibraryModel model, models)
        {
            if (type == "MOVIE" && model.getType() == "MOVIE")
                this->comboBoxTarget->addItem(model.getName());
            else if (type == Rundown::AUDIO && model.getType() == Rundown::AUDIO)
                this->comboBoxTarget->addItem(model.getName());
            else if (type == Rundown::TEMPLATE && model.getType() == Rundown::TEMPLATE)
                this->comboBoxTarget->addItem(model.getName());
            else if ((type == Rundown::STILL || type == Rundown::IMAGESCROLLER) && model.getType() == "STILL")
                this->comboBoxTarget->addItem(model.getName());
        }

        this->comboBoxTarget->setCurrentIndex(this->comboBoxTarget->findText(this->model->getName()));
    }
}

void InspectorOutputWidget::checkEmptyDevice()
{
    if (this->comboBoxDevice->isEnabled() && this->comboBoxDevice->currentText() == "")
        this->comboBoxDevice->setStyleSheet("border-color: firebrick;");
    else
        this->comboBoxDevice->setStyleSheet("");
}

void InspectorOutputWidget::checkEmptyAtemDevice()
{
    if (this->comboBoxAtemDevice->isEnabled() && this->comboBoxAtemDevice->currentText() == "")
        this->comboBoxAtemDevice->setStyleSheet("border-color: firebrick;");
    else
        this->comboBoxAtemDevice->setStyleSheet("");
}

void InspectorOutputWidget::checkEmptyTriCasterDevice()
{
    if (this->comboBoxTriCasterDevice->isEnabled() && this->comboBoxTriCasterDevice->currentText() == "")
        this->comboBoxTriCasterDevice->setStyleSheet("border-color: firebrick;");
    else
        this->comboBoxTriCasterDevice->setStyleSheet("");
}

void InspectorOutputWidget::checkEmptyTarget()
{
    if (this->comboBoxTarget->isEnabled() && this->comboBoxTarget->currentText() == "")
        this->comboBoxTarget->setStyleSheet("border-color: firebrick;");
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
    const QSharedPointer<DeviceModel> model = DeviceManager::getInstance().getDeviceModelByAddress(device.getAddress());
    if (model == NULL || model->getShadow() == "Yes")
        return; // Don't add shadow systems.

    int index = this->comboBoxDevice->currentIndex();

    this->comboBoxDevice->addItem(model->getName());

    if (index == -1)
        this->comboBoxDevice->setCurrentIndex(index);
}

void InspectorOutputWidget::deviceNameChanged(QString deviceName)
{
    if (deviceName.isEmpty())
        return;

    const QStringList& channelFormats = DatabaseManager::getInstance().getDeviceByName(deviceName).getChannelFormats().split(",");
    this->spinBoxChannel->setMaximum(channelFormats.count());

    checkEmptyDevice();
    checkEmptyAtemDevice();
    checkEmptyTriCasterDevice();
    checkEmptyTarget();

    EventManager::getInstance().fireDeviceChangedEvent(DeviceChangedEvent(this->comboBoxDevice->currentText()));
}

void InspectorOutputWidget::targetChanged(QString name)
{
    checkEmptyTarget();

    EventManager::getInstance().fireTargetChangedEvent(TargetChangedEvent(this->comboBoxTarget->currentText()));
    if (this->model->getLabel() == this->comboBoxTarget->getPreviousText())
        EventManager::getInstance().fireLabelChangedEvent(LabelChangedEvent(this->comboBoxTarget->currentText()));
}

void InspectorOutputWidget::channelChanged(int channel)
{
    this->command->setChannel(channel);

    EventManager::getInstance().fireChannelChangedEvent(ChannelChangedEvent(channel));
}

void InspectorOutputWidget::videolayerChanged(int videolayer)
{
    this->command->setVideolayer(videolayer);

    EventManager::getInstance().fireVideolayerChangedEvent(VideolayerChangedEvent(videolayer));
}

void InspectorOutputWidget::delayChanged(int delay)
{
    this->command->setDelay(delay);
}

void InspectorOutputWidget::durationChanged(int duration)
{
    this->command->setDuration(duration);
}

void InspectorOutputWidget::allowGpiChanged(int state)
{
    this->command->setAllowGpi((state == Qt::Checked) ? true : false);
}

void InspectorOutputWidget::allowRemoteTriggeringChanged(int state)
{
    this->command->setAllowRemoteTriggering((state == Qt::Checked) ? true : false);

    this->labelRemoteTriggerIdField->setEnabled(this->command->getAllowRemoteTriggering());
    this->lineEditRemoteTriggerId->setEnabled(this->command->getAllowRemoteTriggering());
}

void InspectorOutputWidget::remoteTriggerIdChanged(QString id)
{
    this->command->setRemoteTriggerId(id);
}

void InspectorOutputWidget::tricasterDeviceRemoved()
{
    blockAllSignals(true);

    this->comboBoxTriCasterDevice->clear();
    foreach (const TriCasterDeviceModel& model, TriCasterDeviceManager::getInstance().getDeviceModels())
        this->comboBoxTriCasterDevice->addItem(model.getName());

    blockAllSignals(false);
}

void InspectorOutputWidget::tricasterDeviceAdded(TriCasterDevice& device)
{
    TriCasterDeviceModel model = TriCasterDeviceManager::getInstance().getDeviceModelByAddress(device.getAddress());

    int index = this->comboBoxTriCasterDevice->currentIndex();

    this->comboBoxTriCasterDevice->addItem(model.getName());

    if (index == -1)
        this->comboBoxTriCasterDevice->setCurrentIndex(index);
}

void InspectorOutputWidget::tricasterDeviceNameChanged(QString deviceName)
{
    checkEmptyDevice();
    checkEmptyAtemDevice();
    checkEmptyTriCasterDevice();
    checkEmptyTarget();

    EventManager::getInstance().fireTriCasterDeviceChangedEvent(TriCasterDeviceChangedEvent(this->comboBoxTriCasterDevice->currentText()));
}

void InspectorOutputWidget::atemDeviceRemoved()
{
    blockAllSignals(true);

    this->comboBoxAtemDevice->clear();
    foreach (const AtemDeviceModel& model, AtemDeviceManager::getInstance().getDeviceModels())
        this->comboBoxAtemDevice->addItem(model.getName());

    blockAllSignals(false);
}

void InspectorOutputWidget::atemDeviceAdded(AtemDevice& device)
{
    AtemDeviceModel model = AtemDeviceManager::getInstance().getDeviceModelByAddress(device.getAddress());

    int index = this->comboBoxAtemDevice->currentIndex();

    this->comboBoxAtemDevice->addItem(model.getName());

    if (index == -1)
        this->comboBoxAtemDevice->setCurrentIndex(index);
}

void InspectorOutputWidget::atemDeviceNameChanged(QString deviceName)
{
    checkEmptyDevice();
    checkEmptyAtemDevice();
    checkEmptyTriCasterDevice();
    checkEmptyTarget();

    EventManager::getInstance().fireAtemDeviceChangedEvent(AtemDeviceChangedEvent(this->comboBoxAtemDevice->currentText()));
}
