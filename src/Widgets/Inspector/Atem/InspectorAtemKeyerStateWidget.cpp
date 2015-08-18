#include "InspectorAtemKeyerStateWidget.h"

#include "Global.h"

#include "AtemDeviceManager.h"
#include "DatabaseManager.h"
#include "EventManager.h"
#include "Models/Atem/AtemKeyerModel.h"

#include <QtWidgets/QApplication>

InspectorAtemKeyerStateWidget::InspectorAtemKeyerStateWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(atemDeviceChanged(const AtemDeviceChangedEvent&)), this, SLOT(atemDeviceChanged(const AtemDeviceChangedEvent&)));

    loadAtemKeyer();
}

void InspectorAtemKeyerStateWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->command = nullptr;
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<AtemKeyerStateCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<AtemKeyerStateCommand*>(event.getCommand());

        this->comboBoxMixerStep->clear();
        const QSharedPointer<AtemDevice> device = AtemDeviceManager::getInstance().getDeviceByName(this->model->getDeviceName());
        if (device != NULL)
        {
            this->mixerEffects = device->mixerEffects();

            loadAtemMixerStep();
        }

        this->comboBoxMixerStep->setCurrentIndex(this->comboBoxMixerStep->findData(this->command->getMixerStep()));
        this->comboBoxKeyer->setCurrentIndex(this->comboBoxKeyer->findData(this->command->getKeyer()));
        this->checkBoxState->setChecked(this->command->getState());
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
    }

    checkEmptyMixerStep();

    blockAllSignals(false);
}

void InspectorAtemKeyerStateWidget::atemDeviceChanged(const AtemDeviceChangedEvent& event)
{
    if (this->model != NULL)
    {
        // Should we update the device name?
        if (!event.getDeviceName().isEmpty() && event.getDeviceName() != this->model->getDeviceName())
        {
            const QSharedPointer<AtemDevice> device = AtemDeviceManager::getInstance().getDeviceByName(event.getDeviceName());
            if (device != NULL)
            {
                this->mixerEffects = device->mixerEffects();

                loadAtemMixerStep();
                checkEmptyMixerStep();
            }
        }
    }
}

void InspectorAtemKeyerStateWidget::blockAllSignals(bool block)
{
    this->comboBoxMixerStep->blockSignals(block);
    this->comboBoxKeyer->blockSignals(block);
    this->checkBoxState->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
}

void InspectorAtemKeyerStateWidget::loadAtemMixerStep()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxMixerStep->blockSignals(true);

    this->comboBoxMixerStep->clear();
    for (int i = 0; i < this->mixerEffects; i++)
        this->comboBoxMixerStep->addItem(QString("%1").arg(i + 1), i);

    this->comboBoxMixerStep->blockSignals(false);
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

void InspectorAtemKeyerStateWidget::checkEmptyMixerStep()
{
    if (this->comboBoxMixerStep->isEnabled() && this->comboBoxMixerStep->currentText() == "")
        this->comboBoxMixerStep->setStyleSheet("border-color: firebrick;");
    else
        this->comboBoxMixerStep->setStyleSheet("");
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

void InspectorAtemKeyerStateWidget::mixerStepChanged(int index)
{
    this->command->setMixerStep(this->comboBoxMixerStep->itemData(index).toString());

    checkEmptyMixerStep();
}
