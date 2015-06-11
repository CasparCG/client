#include "InspectorAtemCutWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Models/Atem/AtemStepModel.h"
#include "Models/Atem/AtemMixerStepModel.h"

#include <QtWidgets/QApplication>

InspectorAtemCutWidget::InspectorAtemCutWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));

    loadAtemMixerStep();
    loadAtemStep();
}

void InspectorAtemCutWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->command = nullptr;
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<AtemCutCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<AtemCutCommand*>(event.getCommand());

        this->comboBoxMixerStep->setCurrentIndex(this->comboBoxMixerStep->findData(this->command->getMixerStep()));
        this->comboBoxStep->setCurrentIndex(this->comboBoxStep->findData(this->command->getStep()));
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
    }

    checkEmptyStep();

    blockAllSignals(false);
}

void InspectorAtemCutWidget::blockAllSignals(bool block)
{
    this->comboBoxMixerStep->blockSignals(block);
    this->comboBoxStep->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
}

void InspectorAtemCutWidget::loadAtemMixerStep()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxMixerStep->blockSignals(true);

    QList<AtemMixerStepModel> models = DatabaseManager::getInstance().getAtemMixerStep();
    foreach (AtemMixerStepModel model, models)
        this->comboBoxMixerStep->addItem(model.getName(), model.getValue());

    this->comboBoxMixerStep->blockSignals(false);
}

void InspectorAtemCutWidget::loadAtemStep()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxStep->blockSignals(true);

    QList<AtemStepModel> models = DatabaseManager::getInstance().getAtemStep();
    foreach (AtemStepModel model, models)
    {
        this->comboBoxStep->addItem(model.getName(), model.getValue());
        break; // We can't do CUT on DSK.
    }

    this->comboBoxStep->blockSignals(false);
}

void InspectorAtemCutWidget::checkEmptyStep()
{
    if (this->comboBoxStep->isEnabled() && this->comboBoxStep->currentText() == "")
        this->comboBoxStep->setStyleSheet("border-color: firebrick;");
    else
        this->comboBoxStep->setStyleSheet("");
}

void InspectorAtemCutWidget::stepChanged(int index)
{
    this->command->setStep(this->comboBoxStep->itemData(index).toString());

    checkEmptyStep();
}

void InspectorAtemCutWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}

void InspectorAtemCutWidget::mixerStepChanged(int index)
{
    this->command->setMixerStep(this->comboBoxMixerStep->itemData(index).toString());
}
