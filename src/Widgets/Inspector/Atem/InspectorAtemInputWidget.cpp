#include "InspectorAtemInputWidget.h"

#include "Global.h"

#include "AtemDeviceManager.h"
#include "DatabaseManager.h"
#include "EventManager.h"
#include "Models/Atem/AtemSwitcherModel.h"

#include <QtGui/QApplication>

InspectorAtemInputWidget::InspectorAtemInputWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(atemDeviceChanged(const AtemDeviceChangedEvent&)), this, SLOT(atemDeviceChanged(const AtemDeviceChangedEvent&)));

    loadAtemSwitcher();
}

void InspectorAtemInputWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<AtemInputCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<AtemInputCommand*>(event.getCommand());

        this->comboBoxInput->clear();
        const QSharedPointer<AtemDevice> device = AtemDeviceManager::getInstance().getDeviceByName(this->model->getDeviceName());
        if (device != NULL)
        {
            if (this->inputs.isEmpty())
                this->inputs = device->inputInfos();

            loadAtemInput(this->inputs);
        }

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
            this->inputs = device->inputInfos();

            loadAtemInput(this->inputs);
        }
    }
}

void InspectorAtemInputWidget::loadAtemInput(QMap<quint16, QAtemConnection::InputInfo> inputs)
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values
    this->comboBoxInput->blockSignals(true);

    this->comboBoxInput->clear();
    foreach (quint16 key, inputs.keys())
        this->comboBoxInput->addItem(inputs.value(key).longText, inputs.value(key).index);

    this->comboBoxInput->blockSignals(false);
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
