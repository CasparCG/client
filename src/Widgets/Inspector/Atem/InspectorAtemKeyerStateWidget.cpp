#include "InspectorAtemKeyerStateWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Models/Atem/AtemKeyerModel.h"

#include <QtGui/QApplication>

InspectorAtemKeyerStateWidget::InspectorAtemKeyerStateWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));

    loadAtemKeyer();
}

void InspectorAtemKeyerStateWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<AtemKeyerStateCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<AtemKeyerStateCommand*>(event.getCommand());

        this->comboBoxKeyer->setCurrentIndex(this->comboBoxKeyer->findData(this->command->getKeyer()));
        this->checkBoxState->setChecked(this->command->getState());
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
    }

    blockAllSignals(false);
}

void InspectorAtemKeyerStateWidget::blockAllSignals(bool block)
{
    this->comboBoxKeyer->blockSignals(block);
    this->checkBoxState->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
}

void InspectorAtemKeyerStateWidget::loadAtemKeyer()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxKeyer->blockSignals(true);

    QList<AtemKeyerModel> models = DatabaseManager::getInstance().getAtemKeyer();
    foreach (AtemKeyerModel model, models)
        this->comboBoxKeyer->addItem(model.getName(), model.getValue());

    this->comboBoxKeyer->blockSignals(false);
}

void InspectorAtemKeyerStateWidget::keyerChanged(int index)
{
    this->command->setKeyer(this->comboBoxKeyer->itemData(index).toString());
}

void InspectorAtemKeyerStateWidget::stateChanged(int state)
{
    this->command->setState((state == Qt::Checked) ? true : false);
}

void InspectorAtemKeyerStateWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}
