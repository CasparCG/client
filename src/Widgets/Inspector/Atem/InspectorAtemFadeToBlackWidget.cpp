#include "InspectorAtemFadeToBlackWidget.h"

#include "Global.h"

#include "AtemDeviceManager.h"
#include "DatabaseManager.h"
#include "EventManager.h"
#include "Models/Atem/AtemMixerStepModel.h"

#include <QtWidgets/QApplication>

InspectorAtemFadeToBlackWidget::InspectorAtemFadeToBlackWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(atemDeviceChanged(const AtemDeviceChangedEvent&)), this, SLOT(atemDeviceChanged(const AtemDeviceChangedEvent&)));
}

void InspectorAtemFadeToBlackWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->command = nullptr;
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<AtemFadeToBlackCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<AtemFadeToBlackCommand*>(event.getCommand());

        this->comboBoxMixerStep->clear();
        const QSharedPointer<AtemDevice> device = AtemDeviceManager::getInstance().getDeviceByName(this->model->getDeviceName());
        if (device != NULL)
        {
            this->mixerEffects = device->mixerEffects();

            loadAtemMixerStep();
        }

        this->comboBoxMixerStep->setCurrentIndex(this->comboBoxMixerStep->findData(this->command->getMixerStep()));
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
    }

    checkEmptyMixerStep();

    blockAllSignals(false);
}

void InspectorAtemFadeToBlackWidget::atemDeviceChanged(const AtemDeviceChangedEvent& event)
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

void InspectorAtemFadeToBlackWidget::blockAllSignals(bool block)
{
    this->comboBoxMixerStep->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
}

void InspectorAtemFadeToBlackWidget::loadAtemMixerStep()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxMixerStep->blockSignals(true);

    this->comboBoxMixerStep->clear();
    for (int i = 0; i < this->mixerEffects; i++)
        this->comboBoxMixerStep->addItem(QString("%1").arg(i + 1), i);

    this->comboBoxMixerStep->blockSignals(false);
}

void InspectorAtemFadeToBlackWidget::checkEmptyMixerStep()
{
    if (this->comboBoxMixerStep->isEnabled() && this->comboBoxMixerStep->currentText() == "")
        this->comboBoxMixerStep->setStyleSheet("border-color: firebrick;");
    else
        this->comboBoxMixerStep->setStyleSheet("");
}

void InspectorAtemFadeToBlackWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}

void InspectorAtemFadeToBlackWidget::mixerStepChanged(int index)
{
    this->command->setMixerStep(this->comboBoxMixerStep->itemData(index).toString());

    checkEmptyMixerStep();
}
