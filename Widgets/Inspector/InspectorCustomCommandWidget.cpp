#include "InspectorCustomCommandWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "Events/RundownItemSelectedEvent.h"
#include "Models/FormatModel.h"

InspectorCustomCommandWidget::InspectorCustomCommandWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    qApp->installEventFilter(this);
}

bool InspectorCustomCommandWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        this->model = rundownItemSelectedEvent->getLibraryModel();

        blockAllSignals(true);

        if (dynamic_cast<CustomCommand*>(rundownItemSelectedEvent->getCommand()))
        {       
            this->command = dynamic_cast<CustomCommand*>(rundownItemSelectedEvent->getCommand());

            this->lineEditStop->setText(this->command->getStopCommand());
            this->lineEditPlay->setText(this->command->getPlayCommand());
            this->lineEditLoad->setText(this->command->getLoadCommand());
            this->lineEditPause->setText(this->command->getPauseCommand());
            this->lineEditNext->setText(this->command->getNextCommand());
            this->lineEditUpdate->setText(this->command->getUpdateCommand());
            this->lineEditInvoke->setText(this->command->getInvokeCommand());
            this->lineEditClear->setText(this->command->getClearCommand());
            this->lineEditClearVideolayer->setText(this->command->getClearVideolayerCommand());
            this->lineEditClearChannel->setText(this->command->getClearChannelCommand());
        }

        blockAllSignals(false);
    }

    return QObject::eventFilter(target, event);
}

void InspectorCustomCommandWidget::blockAllSignals(bool block)
{
    this->lineEditStop->blockSignals(block);
    this->lineEditPlay->blockSignals(block);
    this->lineEditLoad->blockSignals(block);
    this->lineEditPause->blockSignals(block);
    this->lineEditNext->blockSignals(block);
    this->lineEditUpdate->blockSignals(block);
    this->lineEditInvoke->blockSignals(block);
    this->lineEditClear->blockSignals(block);
    this->lineEditClearVideolayer->blockSignals(block);
    this->lineEditClearChannel->blockSignals(block);
}

void InspectorCustomCommandWidget::stopCommandChanged(QString command)
{
    this->command->setStopCommand(command);
}

void InspectorCustomCommandWidget::playCommandChanged(QString command)
{
    this->command->setPlayCommand(command);
}

void InspectorCustomCommandWidget::loadCommandChanged(QString command)
{
    this->command->setLoadCommand(command);
}

void InspectorCustomCommandWidget::pauseCommandChanged(QString command)
{
    this->command->setPauseCommand(command);
}

void InspectorCustomCommandWidget::nextCommandChanged(QString command)
{
    this->command->setNextCommand(command);
}

void InspectorCustomCommandWidget::updateCommandChanged(QString command)
{
    this->command->setUpdateCommand(command);
}

void InspectorCustomCommandWidget::invokeCommandChanged(QString command)
{
    this->command->setInvokeCommand(command);
}

void InspectorCustomCommandWidget::clearCommandChanged(QString command)
{
    this->command->setClearCommand(command);
}

void InspectorCustomCommandWidget::clearVideolayerCommandChanged(QString command)
{
    this->command->setClearVideolayerCommand(command);
}

void InspectorCustomCommandWidget::clearChannelCommandChanged(QString command)
{
    this->command->setClearChannelCommand(command);
}
