#include "InspectorMetadataWidget.h"

#include "Global.h"

#include "EventManager.h"
#include "DeviceManager.h"
#include "Commands/BlendModeCommand.h"
#include "Commands/BrightnessCommand.h"
#include "Commands/CommitCommand.h"
#include "Commands/ContrastCommand.h"
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
#include "Events/LibraryItemSelectedEvent.h"
#include "Events/RundownItemSelectedEvent.h"

#include <QtGui/QApplication>

InspectorMetadataWidget::InspectorMetadataWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL)
{
    setupUi(this);

    qApp->installEventFilter(this);
}

bool InspectorMetadataWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::LibraryItemSelected))
    {
        LibraryItemSelectedEvent* libraryItemSelectedEvent = dynamic_cast<LibraryItemSelectedEvent*>(event);
        this->model = libraryItemSelectedEvent->getLibraryModel();

        blockAllSignals(true);

        this->lineEditTarget->setReadOnly(true);
        this->lineEditLabel->setReadOnly(true);

        this->lineEditType->setText(this->model->getType());
        this->lineEditTarget->setText(this->model->getName());
        this->lineEditLabel->clear();

        blockAllSignals(false);
    }
    else if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        this->model = rundownItemSelectedEvent->getLibraryModel();

        blockAllSignals(true);

        this->lineEditTarget->setReadOnly(false);
        this->lineEditTarget->setEnabled(true);
        this->lineEditLabel->setReadOnly(false);

        this->lineEditType->setText(this->model->getType());
        this->lineEditTarget->setText(this->model->getName());
        this->lineEditLabel->setText(this->model->getLabel());

        if (dynamic_cast<GpiOutputCommand*>(rundownItemSelectedEvent->getCommand()) ||
            dynamic_cast<GroupCommand*>(rundownItemSelectedEvent->getCommand()) ||
            dynamic_cast<SeparatorCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->lineEditTarget->setEnabled(false);
        }
        else if (dynamic_cast<PrintCommand*>(rundownItemSelectedEvent->getCommand()) ||
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
                 dynamic_cast<SolidColorCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->lineEditTarget->setEnabled(false);
        }

        blockAllSignals(false);
    }
    else if (event->type() == static_cast<QEvent::Type>(Enum::EventType::EmptyRundown))
    {
        blockAllSignals(true);

        this->lineEditTarget->setReadOnly(true);
        this->lineEditLabel->setReadOnly(true);

        this->lineEditType->clear();
        this->lineEditTarget->clear();
        this->lineEditLabel->clear();

        blockAllSignals(false);
    }

    return QObject::eventFilter(target, event);
}

void InspectorMetadataWidget::blockAllSignals(bool block)
{
    this->lineEditType->blockSignals(block);
    this->lineEditTarget->blockSignals(block);
    this->lineEditLabel->blockSignals(block);
}

void InspectorMetadataWidget::labelChanged(QString name)
{
    EventManager::getInstance().fireLabelChangedEvent(this->lineEditLabel->text());
}

void InspectorMetadataWidget::targetChanged(QString name)
{
    EventManager::getInstance().fireTargetChangedEvent(this->lineEditTarget->text());
}
