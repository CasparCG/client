#include "InspectorAtemAudioGainWidget.h"

#include "Global.h"

#include "AtemDeviceManager.h"
#include "DatabaseManager.h"
#include "EventManager.h"
#include "Models/Atem/AtemAudioInputModel.h"

#include <QtGui/QApplication>

InspectorAtemAudioGainWidget::InspectorAtemAudioGainWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(atemDeviceChanged(const AtemDeviceChangedEvent&)), this, SLOT(atemDeviceChanged(const AtemDeviceChangedEvent&)));
}

void InspectorAtemAudioGainWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<AtemAudioGainCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<AtemAudioGainCommand*>(event.getCommand());

        this->comboBoxInput->clear();
        const QSharedPointer<AtemDevice> device = AtemDeviceManager::getInstance().getDeviceByName(this->model->getDeviceName());
        if (device != NULL)
        {
            if (this->inputs.isEmpty())
                this->inputs = device->inputInfos();

            loadAtemAudioInput(this->inputs);
        }

        this->comboBoxInput->setCurrentIndex(this->comboBoxInput->findData(this->command->getInput()));
        this->sliderGain->setValue(this->command->getGain() * 100);
        this->doubleSpinBoxGain->setValue(this->command->getGain());
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
    }

    blockAllSignals(false);
}

void InspectorAtemAudioGainWidget::atemDeviceChanged(const AtemDeviceChangedEvent& event)
{
    if (this->model != NULL)
    {
        // Should we update the device name?
        if (!event.getDeviceName().isEmpty() && event.getDeviceName() != this->model->getDeviceName())
        {
            const QSharedPointer<AtemDevice> device = AtemDeviceManager::getInstance().getDeviceByName(event.getDeviceName());
            this->inputs = device->inputInfos();

            loadAtemAudioInput(this->inputs);
        }
    }
}

void InspectorAtemAudioGainWidget::loadAtemAudioInput(QMap<quint16, QAtemConnection::InputInfo> inputs)
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxInput->blockSignals(true);

    this->comboBoxInput->addItem("Master", "0");

    foreach (quint16 key, inputs.keys())
    {
        if (inputs.value(key).type == 0 || inputs.value(key).type == 4)
            this->comboBoxInput->addItem(inputs.value(key).longText, inputs.value(key).index);
    }

    this->comboBoxInput->blockSignals(false);
}

void InspectorAtemAudioGainWidget::blockAllSignals(bool block)
{
    this->comboBoxInput->blockSignals(block);
    this->sliderGain->blockSignals(block);
    this->doubleSpinBoxGain->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
}

void InspectorAtemAudioGainWidget::inputChanged(int index)
{
    this->command->setInput(this->comboBoxInput->itemData(index).toString());
}

void InspectorAtemAudioGainWidget::sliderGainChanged(int value)
{
    this->command->setGain(static_cast<float>(value) / 100);

    this->doubleSpinBoxGain->blockSignals(true);
    this->doubleSpinBoxGain->setValue(static_cast<float>(value) / 100);
    this->doubleSpinBoxGain->blockSignals(false);
}

void InspectorAtemAudioGainWidget::doubleSpinBoxGainChanged(double value)
{
    this->command->setGain(static_cast<float>(value));

    this->sliderGain->blockSignals(true);
    this->sliderGain->setValue(value * 100);
    this->sliderGain->blockSignals(false);
}

void InspectorAtemAudioGainWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}
