#include "InspectorMetadataWidget.h"

#include "Global.h"

#include "DeviceManager.h"
#include "Commands/BlendModeCommand.h"
#include "Commands/BrightnessCommand.h"
#include "Commands/CommitCommand.h"
#include "Commands/ContrastCommand.h"
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
#include "Commands/VolumeCommand.h"
#include "Events/LibraryItemSelectedEvent.h"
#include "Events/RundownIsEmptyEvent.h"
#include "Events/RundownItemChangedEvent.h"
#include "Events/RundownItemSelectedEvent.h"

#include <QtGui/QApplication>

InspectorMetadataWidget::InspectorMetadataWidget(QWidget* parent)
    : QWidget(parent), preview(false), model(NULL)
{
    setupUi(this);

    QObject::connect(&DeviceManager::getInstance(), SIGNAL(deviceRemoved()), this, SLOT(deviceRemoved()));
    QObject::connect(&DeviceManager::getInstance(), SIGNAL(deviceAdded(CasparDevice&)), this, SLOT(deviceAdded(CasparDevice&)));

    qApp->installEventFilter(this);
}

bool InspectorMetadataWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::LibraryItemSelected))
    {
        this->preview = false;

        this->lineEditLabel->setReadOnly(true);
        this->comboBoxDevice->setEnabled(false);
        this->lineEditName->setReadOnly(true);

        LibraryItemSelectedEvent* libraryItemSelectedEvent = dynamic_cast<LibraryItemSelectedEvent*>(event);
        this->model = libraryItemSelectedEvent->getLibraryModel();

        this->lineEditType->setText(this->model->getType());
        this->comboBoxDevice->setCurrentIndex(this->comboBoxDevice->findText(this->model->getDeviceName()));
        this->lineEditName->setText(this->model->getName());

        checkEmptyDevice();

        this->preview = true;
    }
    else if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemSelected))
    {
        this->preview = false;

        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        this->model = rundownItemSelectedEvent->getLibraryModel();

        this->lineEditLabel->setReadOnly(false);
        this->comboBoxDevice->setEnabled(true);
        this->lineEditName->setReadOnly(false);

        this->lineEditType->setText(this->model->getType());
        this->lineEditLabel->setText(this->model->getLabel());
        this->comboBoxDevice->setCurrentIndex(this->comboBoxDevice->findText(this->model->getDeviceName()));
        this->lineEditName->setText(this->model->getName());

        if (dynamic_cast<GpiOutputCommand*>(rundownItemSelectedEvent->getCommand()) ||
            dynamic_cast<GroupCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->lineEditLabel->setReadOnly(false);
            this->comboBoxDevice->setEnabled(false);
            this->lineEditName->setReadOnly(true);
        }
        else if (dynamic_cast<FileRecorderCommand*>(rundownItemSelectedEvent->getCommand()) ||
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
                 dynamic_cast<VolumeCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->lineEditName->setReadOnly(true);
        }

        checkEmptyDevice();

        this->preview = true;
    }
    else if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownIsEmpty))
    {
        this->preview = false;

        this->lineEditLabel->setReadOnly(true);
        this->comboBoxDevice->setEnabled(false);
        this->lineEditName->setReadOnly(true);

        this->lineEditType->clear();
        this->lineEditLabel->clear();
        this->comboBoxDevice->setCurrentIndex(-1);
        this->lineEditName->clear();

        checkEmptyDevice();

        this->preview = true;
    }

    return QObject::eventFilter(target, event);
}

void InspectorMetadataWidget::checkEmptyDevice()
{
    if (this->comboBoxDevice->isEnabled() && this->comboBoxDevice->currentText() == "")
        this->comboBoxDevice->setStyleSheet("border-color: red;");
    else
        this->comboBoxDevice->setStyleSheet("");
}

void InspectorMetadataWidget::deviceRemoved()
{
    this->comboBoxDevice->clear();
    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
        this->comboBoxDevice->addItem(model.getName());
}

void InspectorMetadataWidget::deviceAdded(CasparDevice& device)
{
    int index = this->comboBoxDevice->currentIndex();

    this->comboBoxDevice->addItem(DeviceManager::getInstance().getDeviceModelByAddress(device.getAddress()).getName());

    if (index == -1)
        this->comboBoxDevice->setCurrentIndex(index);
}

void InspectorMetadataWidget::labelChanged(QString name)
{
    if (!this->lineEditLabel->isReadOnly())
        qApp->postEvent(qApp, new RundownItemChangedEvent(this->lineEditLabel->text(), this->lineEditName->text(), this->comboBoxDevice->currentText()));
}

void InspectorMetadataWidget::nameChanged(QString name)
{
    if (!this->lineEditName->isReadOnly())
        qApp->postEvent(qApp, new RundownItemChangedEvent(this->lineEditLabel->text(), this->lineEditName->text(), this->comboBoxDevice->currentText()));
}

void InspectorMetadataWidget::deviceNameChanged(QString deviceName)
{
    if (this->comboBoxDevice->isEnabled())
        checkEmptyDevice();

    if (this->comboBoxDevice->isEnabled())
        qApp->postEvent(qApp, new RundownItemChangedEvent(this->lineEditLabel->text(), this->lineEditName->text(), this->comboBoxDevice->currentText()));
}
