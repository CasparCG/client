#include "InspectorAtemAudioInputBalanceWidget.h"

#include "Global.h"

#include "AtemDeviceManager.h"
#include "DatabaseManager.h"
#include "EventManager.h"
#include "Models/Atem/AtemAudioInputModel.h"

#include <QtGui/QApplication>

InspectorAtemAudioInputBalanceWidget::InspectorAtemAudioInputBalanceWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(atemDeviceChanged(const AtemDeviceChangedEvent&)), this, SLOT(atemDeviceChanged(const AtemDeviceChangedEvent&)));
}

void InspectorAtemAudioInputBalanceWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<AtemAudioInputBalanceCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<AtemAudioInputBalanceCommand*>(event.getCommand());

        this->comboBoxInput->clear();
        const QSharedPointer<AtemDevice> device = AtemDeviceManager::getInstance().getDeviceByName(this->model->getDeviceName());
        if (device != NULL)
        {
            if (this->inputs.isEmpty())
                this->inputs = device->inputInfos();

            loadAtemAudioInput(this->inputs);
        }

        this->comboBoxInput->setCurrentIndex(this->comboBoxInput->findData(this->command->getInput()));
        this->sliderBalance->setValue(this->command->getBalance() * 100);
        this->doubleSpinBoxBalance->setValue(this->command->getBalance());
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext()); 
    }

    blockAllSignals(false);
}

void InspectorAtemAudioInputBalanceWidget::atemDeviceChanged(const AtemDeviceChangedEvent& event)
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

void InspectorAtemAudioInputBalanceWidget::loadAtemAudioInput(QMap<quint16, QAtemConnection::InputInfo> inputs)
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values
    this->comboBoxInput->blockSignals(true);

    this->comboBoxInput->clear();
    this->comboBoxInput->addItem("Master", "0");

    foreach (quint16 key, inputs.keys())
    {
        if (inputs.value(key).type == 0 || inputs.value(key).type == 4)
            this->comboBoxInput->addItem(inputs.value(key).longText, inputs.value(key).index);
    }

    this->comboBoxInput->blockSignals(false);
}

void InspectorAtemAudioInputBalanceWidget::blockAllSignals(bool block)
{
    this->comboBoxInput->blockSignals(block);
    this->sliderBalance->blockSignals(block);
    this->doubleSpinBoxBalance->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
}

void InspectorAtemAudioInputBalanceWidget::inputChanged(int index)
{
    this->command->setInput(this->comboBoxInput->itemData(index).toString());
}

void InspectorAtemAudioInputBalanceWidget::sliderBalanceChanged(int value)
{
    this->command->setBalance(static_cast<float>(value) / 100);

    this->doubleSpinBoxBalance->blockSignals(true);
    this->doubleSpinBoxBalance->setValue(static_cast<float>(value) / 100);
    this->doubleSpinBoxBalance->blockSignals(false);
}

void InspectorAtemAudioInputBalanceWidget::doubleSpinBoxBalanceChanged(double value)
{
    this->command->setBalance(static_cast<float>(value));

    this->sliderBalance->blockSignals(true);
    this->sliderBalance->setValue(value * 100);
    this->sliderBalance->blockSignals(false);
}

void InspectorAtemAudioInputBalanceWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}
