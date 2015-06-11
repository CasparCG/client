#include "InspectorAtemInputWidget.h"

#include "Global.h"

#include "AtemDeviceManager.h"
#include "DatabaseManager.h"
#include "EventManager.h"
#include "Models/Atem/AtemSwitcherModel.h"
#include "Models/Atem/AtemMixerStepModel.h"

#include <QtWidgets/QApplication>

InspectorAtemInputWidget::InspectorAtemInputWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    loadAtemMixerStep();

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(atemDeviceChanged(const AtemDeviceChangedEvent&)), this, SLOT(atemDeviceChanged(const AtemDeviceChangedEvent&)));
}

void InspectorAtemInputWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->command = nullptr;
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<AtemInputCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<AtemInputCommand*>(event.getCommand());

        this->comboBoxInput->clear();
        const QSharedPointer<AtemDevice> device = AtemDeviceManager::getInstance().getDeviceByName(this->model->getDeviceName());
        if (device != NULL)
        {
            this->inputs = device->inputInfos();

            loadAtemSwitcher();
            loadAtemInput();
        }

        this->comboBoxMixerStep->setCurrentIndex(this->comboBoxMixerStep->findData(this->command->getMixerStep()));
        this->comboBoxInput->setCurrentIndex(this->comboBoxInput->findData(this->command->getInput()));
        this->comboBoxSwitcher->setCurrentIndex(this->comboBoxSwitcher->findData(this->command->getSwitcher()));
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
    }

    blockAllSignals(false);
}

void InspectorAtemInputWidget::atemDeviceChanged(const AtemDeviceChangedEvent& event)
{
    if (this->model != NULL)
    {
        // Should we update the device name?
        if (!event.getDeviceName().isEmpty() && event.getDeviceName() != this->model->getDeviceName())
        {
            const QSharedPointer<AtemDevice> device = AtemDeviceManager::getInstance().getDeviceByName(event.getDeviceName());
            if (device != NULL)
            {
                this->inputs = device->inputInfos();

                loadAtemSwitcher();
                loadAtemInput();
            }
        }
    }
}

void InspectorAtemInputWidget::loadAtemMixerStep()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxMixerStep->blockSignals(true);

    QList<AtemMixerStepModel> models = DatabaseManager::getInstance().getAtemMixerStep();
    foreach (AtemMixerStepModel model, models)
        this->comboBoxMixerStep->addItem(model.getName(), model.getValue());

    this->comboBoxMixerStep->blockSignals(false);
}

void InspectorAtemInputWidget::loadAtemInput()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values
    this->comboBoxInput->blockSignals(true);

    this->comboBoxInput->clear();
    foreach (quint16 key, this->inputs.keys())
    {
        if (inputs.value(key).internalType == 129) // Aux.
            continue;

        this->comboBoxInput->addItem(this->inputs.value(key).longText, this->inputs.value(key).index);
    }

    this->comboBoxInput->blockSignals(false);
}

void InspectorAtemInputWidget::blockAllSignals(bool block)
{
    this->comboBoxMixerStep->blockSignals(block);
    this->comboBoxSwitcher->blockSignals(block);
    this->comboBoxInput->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
}

void InspectorAtemInputWidget::loadAtemSwitcher()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxSwitcher->blockSignals(true);

    this->comboBoxSwitcher->clear();

    QList<AtemSwitcherModel> models = DatabaseManager::getInstance().getAtemSwitcher();
    foreach (AtemSwitcherModel model, models)
        this->comboBoxSwitcher->addItem(model.getName(), model.getValue());

    foreach (quint16 key, this->inputs.keys())
    {
        if (inputs.value(key).internalType == 129) // Aux.
            this->comboBoxSwitcher->addItem(this->inputs.value(key).longText, this->inputs.value(key).index);
    }

    this->comboBoxSwitcher->blockSignals(false);
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

void InspectorAtemInputWidget::mixerStepChanged(int index)
{
    this->command->setMixerStep(this->comboBoxMixerStep->itemData(index).toString());
}
