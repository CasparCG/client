#include "InspectorTakeWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Models/TriCaster/TriCasterStepModel.h"

#include <QtGui/QApplication>

InspectorTakeWidget::InspectorTakeWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));

    loadTriCasterStep();
}

void InspectorTakeWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<TakeCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<TakeCommand*>(event.getCommand());

        this->comboBoxStep->setCurrentIndex(this->comboBoxStep->findData(this->command->getStep()));
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
    }

    checkEmptyStep();

    blockAllSignals(false);
}

void InspectorTakeWidget::blockAllSignals(bool block)
{
    this->comboBoxStep->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
}

void InspectorTakeWidget::loadTriCasterStep()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxStep->blockSignals(true);

    QList<TriCasterStepModel> models = DatabaseManager::getInstance().getTriCasterStep();
    foreach (TriCasterStepModel model, models)
        this->comboBoxStep->addItem(model.getName(), model.getValue());

    this->comboBoxStep->blockSignals(false);
}

void InspectorTakeWidget::checkEmptyStep()
{
    if (this->comboBoxStep->isEnabled() && this->comboBoxStep->currentText() == "")
        this->comboBoxStep->setStyleSheet("border-color: firebrick;");
    else
        this->comboBoxStep->setStyleSheet("");
}

void InspectorTakeWidget::stepChanged(int index)
{
    this->command->setStep(this->comboBoxStep->itemData(index).toString());

    checkEmptyStep();
}

void InspectorTakeWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}
