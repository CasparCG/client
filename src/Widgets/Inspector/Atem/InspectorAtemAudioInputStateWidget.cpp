#include "InspectorAtemAudioInputStateWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Models/Atem/AtemAudioInputModel.h"

#include <QtGui/QApplication>

InspectorAtemAudioInputStateWidget::InspectorAtemAudioInputStateWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));

    loadAtemInput();
    loadAtemInputState();
}

void InspectorAtemAudioInputStateWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<AtemAudioInputStateCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<AtemAudioInputStateCommand*>(event.getCommand());

        this->comboBoxInput->setCurrentIndex(this->comboBoxInput->findData(this->command->getInput()));
        this->comboBoxState->setCurrentIndex(this->comboBoxState->findData(this->command->getState()));
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
    }

    blockAllSignals(false);
}

void InspectorAtemAudioInputStateWidget::blockAllSignals(bool block)
{
    this->comboBoxInput->blockSignals(block);
    this->comboBoxState->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
}

void InspectorAtemAudioInputStateWidget::loadAtemInput()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxInput->blockSignals(true);

    QList<AtemAudioInputModel> models = DatabaseManager::getInstance().getAtemAudioInput();
    foreach (AtemAudioInputModel model, models)
        this->comboBoxInput->addItem(model.getName(), model.getValue());

    this->comboBoxInput->blockSignals(false);
}

void InspectorAtemAudioInputStateWidget::loadAtemInputState()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxState->blockSignals(true);

    QList<AtemAudioInputStateModel> models = DatabaseManager::getInstance().getAtemAudioInputState();
    foreach (AtemAudioInputStateModel model, models)
        this->comboBoxState->addItem(model.getName(), model.getValue());

    this->comboBoxState->blockSignals(false);
}

void InspectorAtemAudioInputStateWidget::inputChanged(int index)
{
    this->command->setInput(this->comboBoxInput->itemData(index).toString());
}

void InspectorAtemAudioInputStateWidget::stateChanged(int index)
{
    this->command->setState(this->comboBoxState->itemData(index).toString());
}

void InspectorAtemAudioInputStateWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}
