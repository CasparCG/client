#include "InspectorMetadataWidget.h"

#include "Global.h"

#include "CasparDevice.h"

#include "EventManager.h"
#include "DeviceManager.h"
#include "DatabaseManager.h"
#include "Commands/BlendModeCommand.h"
#include "Commands/BrightnessCommand.h"
#include "Commands/CommitCommand.h"
#include "Commands/ContrastCommand.h"
#include "Commands/CustomCommand.h"
#include "Commands/SolidColorCommand.h"
#include "Commands/CropCommand.h"
#include "Commands/DeckLinkInputCommand.h"
#include "Commands/FileRecorderCommand.h"
#include "Commands/GeometryCommand.h"
#include "Commands/GpiOutputCommand.h"
#include "Commands/GridCommand.h"
#include "Commands/GroupCommand.h"
#include "Commands/KeyerCommand.h"
#include "Commands/LevelsCommand.h"
#include "Commands/OpacityCommand.h"
#include "Commands/SaturationCommand.h"
#include "Commands/SeparatorCommand.h"
#include "Commands/VolumeCommand.h"
#include "Commands/PrintCommand.h"
#include "Events/DeviceChangedEvent.h"
#include "Events/LibraryItemSelectedEvent.h"
#include "Events/RundownItemSelectedEvent.h"

#include <QtCore/QSharedPointer>

#include <QtGui/QApplication>

InspectorMetadataWidget::InspectorMetadataWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL)
{
    setupUi(this);

    this->comboBoxTarget->lineEdit()->setStyleSheet("background-color: transparent; border-width: 0px;");

    qApp->installEventFilter(this);
}

bool InspectorMetadataWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Event::EventType::LibraryItemSelected))
    {
        LibraryItemSelectedEvent* libraryItemSelectedEvent = dynamic_cast<LibraryItemSelectedEvent*>(event);
        this->model = libraryItemSelectedEvent->getLibraryModel();

        blockAllSignals(true);

        this->labelType->setEnabled(false);
        this->lineEditLabel->setEnabled(false);
        this->comboBoxTarget->setEnabled(false);

        this->lineEditType->setText(this->model->getType());
        this->lineEditLabel->clear();

        fillTargetCombo(this->model->getType());

        checkEmptyDevice();

        blockAllSignals(false);
    }
    else if (event->type() == static_cast<QEvent::Type>(Event::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        this->model = rundownItemSelectedEvent->getLibraryModel();

        blockAllSignals(true);

        this->labelType->setEnabled(true);
        this->lineEditLabel->setEnabled(true);
        this->comboBoxTarget->setEnabled(true);

        this->lineEditLabel->setReadOnly(false);

        this->lineEditType->setText(this->model->getType());
        this->lineEditLabel->setText(this->model->getLabel());

        fillTargetCombo(this->model->getType());

        if (dynamic_cast<GpiOutputCommand*>(rundownItemSelectedEvent->getCommand()) ||
            dynamic_cast<GroupCommand*>(rundownItemSelectedEvent->getCommand()) ||
            dynamic_cast<SeparatorCommand*>(rundownItemSelectedEvent->getCommand()) ||
            dynamic_cast<PrintCommand*>(rundownItemSelectedEvent->getCommand()) ||
            dynamic_cast<FileRecorderCommand*>(rundownItemSelectedEvent->getCommand()) ||
            dynamic_cast<DeckLinkInputCommand*>(rundownItemSelectedEvent->getCommand()) ||
            dynamic_cast<BlendModeCommand*>(rundownItemSelectedEvent->getCommand()) ||
            dynamic_cast<BrightnessCommand*>(rundownItemSelectedEvent->getCommand()) ||
            dynamic_cast<CommitCommand*>(rundownItemSelectedEvent->getCommand()) ||
            dynamic_cast<ContrastCommand*>(rundownItemSelectedEvent->getCommand()) ||
            dynamic_cast<CropCommand*>(rundownItemSelectedEvent->getCommand()) ||
            dynamic_cast<GeometryCommand*>(rundownItemSelectedEvent->getCommand()) ||
            dynamic_cast<GridCommand*>(rundownItemSelectedEvent->getCommand()) ||
            dynamic_cast<KeyerCommand*>(rundownItemSelectedEvent->getCommand()) ||
            dynamic_cast<LevelsCommand*>(rundownItemSelectedEvent->getCommand()) ||
            dynamic_cast<OpacityCommand*>(rundownItemSelectedEvent->getCommand()) ||
            dynamic_cast<SaturationCommand*>(rundownItemSelectedEvent->getCommand()) ||
            dynamic_cast<VolumeCommand*>(rundownItemSelectedEvent->getCommand()) ||
            dynamic_cast<SolidColorCommand*>(rundownItemSelectedEvent->getCommand()) ||
            dynamic_cast<CustomCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->comboBoxTarget->setEnabled(false);
        }

        checkEmptyDevice();

        blockAllSignals(false);
    }
    else if (event->type() == static_cast<QEvent::Type>(Event::EventType::EmptyRundown))
    {
        blockAllSignals(true);

        this->labelType->setEnabled(false);
        this->lineEditLabel->setEnabled(false);
        this->comboBoxTarget->setEnabled(false);

        this->lineEditType->clear();
        this->lineEditLabel->clear();
        this->comboBoxTarget->clear();

        checkEmptyDevice();

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
    }

    return QObject::eventFilter(target, event);
}

void InspectorMetadataWidget::blockAllSignals(bool block)
{
    this->lineEditType->blockSignals(block);
    this->lineEditLabel->blockSignals(block);
    this->comboBoxTarget->blockSignals(block);
}

void InspectorMetadataWidget::fillTargetCombo(const QString& type)
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

void InspectorMetadataWidget::checkEmptyDevice()
{
    if (this->comboBoxTarget->isEnabled() && this->comboBoxTarget->currentText() == "")
        this->comboBoxTarget->setStyleSheet("border-color: red;");
    else
        this->comboBoxTarget->setStyleSheet("");
}

void InspectorMetadataWidget::labelChanged(QString name)
{
    EventManager::getInstance().fireLabelChangedEvent(this->lineEditLabel->text());
}

void InspectorMetadataWidget::targetChanged(QString name)
{
    checkEmptyDevice();

    EventManager::getInstance().fireTargetChangedEvent(this->comboBoxTarget->currentText());
}
