#include "InspectorCustomCommandWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Models/FormatModel.h"

InspectorCustomCommandWidget::InspectorCustomCommandWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
}

void InspectorCustomCommandWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<CustomCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<CustomCommand*>(event.getCommand());

        this->lineEditStop->setText(this->command->getStopCommand());
        this->lineEditPlay->setText(this->command->getPlayCommand());
        this->lineEditLoad->setText(this->command->getLoadCommand());
        this->lineEditPause->setText(this->command->getPauseCommand());
        this->lineEditNext->setText(this->command->getNextCommand());
        this->lineEditUpdate->setText(this->command->getUpdateCommand());
        this->lineEditInvoke->setText(this->command->getInvokeCommand());
        this->lineEditPreview->setText(this->command->getPreviewCommand());
        this->lineEditClear->setText(this->command->getClearCommand());
        this->lineEditClearVideolayer->setText(this->command->getClearVideolayerCommand());
        this->lineEditClearChannel->setText(this->command->getClearChannelCommand());
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
    }

    blockAllSignals(false);
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
    this->lineEditPreview->blockSignals(block);
    this->lineEditClear->blockSignals(block);
    this->lineEditClearVideolayer->blockSignals(block);
    this->lineEditClearChannel->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
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

void InspectorCustomCommandWidget::previewCommandChanged(QString command)
{
    this->command->setPreviewCommand(command);
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

void InspectorCustomCommandWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}

