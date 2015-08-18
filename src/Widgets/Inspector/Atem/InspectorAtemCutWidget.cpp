#include "InspectorAtemCutWidget.h"

#include "Global.h"

#include "AtemDeviceManager.h"
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
    QObject::connect(&EventManager::getInstance(), SIGNAL(atemDeviceChanged(const AtemDeviceChangedEvent&)), this, SLOT(atemDeviceChanged(const AtemDeviceChangedEvent&)));

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

        this->comboBoxMixerStep->clear();
        const QSharedPointer<AtemDevice> device = AtemDeviceManager::getInstance().getDeviceByName(this->model->getDeviceName());
        if (device != NULL)
        {
            this->mixerEffects = device->mixerEffects();

            loadAtemMixerStep();
        }

        this->comboBoxMixerStep->setCurrentIndex(this->comboBoxMixerStep->findData(this->command->getMixerStep()));
        this->comboBoxStep->setCurrentIndex(this->comboBoxStep->findData(this->command->getStep()));
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
    }

    checkEmptyStep();
    checkEmptyMixerStep();

    blockAllSignals(false);
}

void InspectorAtemCutWidget::atemDeviceChanged(const AtemDeviceChangedEvent& event)
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

    this->comboBoxMixerStep->clear();
    for (int i = 0; i < this->mixerEffects; i++)
        this->comboBoxMixerStep->addItem(QString("%1").arg(i + 1), i);

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

void InspectorAtemCutWidget::checkEmptyMixerStep()
{
    if (this->comboBoxMixerStep->isEnabled() && this->comboBoxMixerStep->currentText() == "")
        this->comboBoxMixerStep->setStyleSheet("border-color: firebrick;");
    else
        this->comboBoxMixerStep->setStyleSheet("");
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

    checkEmptyMixerStep();
}
