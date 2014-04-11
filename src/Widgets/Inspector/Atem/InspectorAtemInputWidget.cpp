#include "InspectorAtemInputWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Models/Atem/AtemSwitcherModel.h"
#include "Models/Atem/AtemInputModel.h"

#include <QtGui/QApplication>

InspectorAtemInputWidget::InspectorAtemInputWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));

    loadAtemSwitcher();
    loadAtemInput();
}

void InspectorAtemInputWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<AtemInputCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<AtemInputCommand*>(event.getCommand());

        this->comboBoxSwitcher->setCurrentIndex(this->comboBoxSwitcher->findData(this->command->getSwitcher()));
        this->comboBoxInput->setCurrentIndex(this->comboBoxInput->findData(this->command->getInput()));
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
    }

    blockAllSignals(false);
}

void InspectorAtemInputWidget::blockAllSignals(bool block)
{
    this->comboBoxSwitcher->blockSignals(block);
    this->comboBoxInput->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
}

void InspectorAtemInputWidget::loadAtemSwitcher()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxSwitcher->blockSignals(true);

    QList<AtemSwitcherModel> models = DatabaseManager::getInstance().getAtemSwitcher();
    foreach (AtemSwitcherModel model, models)
        this->comboBoxSwitcher->addItem(model.getName(), model.getValue());

    this->comboBoxSwitcher->blockSignals(false);
}

void InspectorAtemInputWidget::loadAtemInput()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxInput->blockSignals(true);

    QList<AtemInputModel> models = DatabaseManager::getInstance().getAtemInput();
    foreach (AtemInputModel model, models)
        this->comboBoxInput->addItem(model.getName(), model.getValue());

    this->comboBoxInput->blockSignals(false);
}

void InspectorAtemInputWidget::switcherChanged(int index)
{
    this->command->setSwitcher(this->comboBoxSwitcher->itemData(index).toString());
}

void InspectorAtemInputWidget::inputChanged(int index)
{
    this->command->setInput(this->comboBoxInput->itemData(index).toString());
}

void InspectorAtemInputWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}
