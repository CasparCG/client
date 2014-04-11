#include "InspectorAtemAutoWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Models/Atem/AtemStepModel.h"
#include "Models/Atem/AtemAutoSpeedModel.h"
#include "Models/Atem/AtemAutoTransitionModel.h"

#include <QtGui/QApplication>

InspectorAtemAutoWidget::InspectorAtemAutoWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));

    loadAtemStep();
    loadAtemAutoTransition();
}

void InspectorAtemAutoWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<AtemAutoCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<AtemAutoCommand*>(event.getCommand());

        this->comboBoxStep->setCurrentIndex(this->comboBoxStep->findData(this->command->getStep()));
        this->spinBoxSpeed->setValue(this->command->getSpeed());
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
    }

    checkEmptyStep();

    blockAllSignals(false);
}

void InspectorAtemAutoWidget::blockAllSignals(bool block)
{
    this->comboBoxStep->blockSignals(block);
    this->spinBoxSpeed->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
}

void InspectorAtemAutoWidget::loadAtemStep()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxStep->blockSignals(true);

    QList<AtemStepModel> models = DatabaseManager::getInstance().getAtemStep();
    foreach (AtemStepModel model, models)
        this->comboBoxStep->addItem(model.getName(), model.getValue());

    this->comboBoxStep->blockSignals(false);
}

void InspectorAtemAutoWidget::loadAtemAutoTransition()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTransition->blockSignals(true);

    QList<AtemAutoTransitionModel> models = DatabaseManager::getInstance().getAtemAutoTransition();
    foreach (AtemAutoTransitionModel model, models)
        this->comboBoxTransition->addItem(model.getName(), model.getValue());

    this->comboBoxTransition->blockSignals(false);
}

void InspectorAtemAutoWidget::checkEmptyStep()
{
    if (this->comboBoxStep->isEnabled() && this->comboBoxStep->currentText() == "")
        this->comboBoxStep->setStyleSheet("border-color: firebrick;");
    else
        this->comboBoxStep->setStyleSheet("");
}

void InspectorAtemAutoWidget::stepChanged(int index)
{
    this->command->setStep(this->comboBoxStep->itemData(index).toString());
}

void InspectorAtemAutoWidget::speedChanged(int value)
{
    this->command->setSpeed(this->spinBoxSpeed->value());
}

void InspectorAtemAutoWidget::transitionChanged(int index)
{
    this->command->setTransition(this->comboBoxTransition->itemData(index).toString());
}

void InspectorAtemAutoWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}
