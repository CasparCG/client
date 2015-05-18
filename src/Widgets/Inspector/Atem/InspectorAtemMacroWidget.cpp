#include "InspectorAtemMacroWidget.h"

#include "Global.h"

#include "AtemDeviceManager.h"
#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"

#include <QtWidgets/QApplication>

InspectorAtemMacroWidget::InspectorAtemMacroWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(atemDeviceChanged(const AtemDeviceChangedEvent&)), this, SLOT(atemDeviceChanged(const AtemDeviceChangedEvent&)));
}

void InspectorAtemMacroWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<AtemMacroCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<AtemMacroCommand*>(event.getCommand());

        this->comboBoxMacro->clear();
        const QSharedPointer<AtemDevice> device = AtemDeviceManager::getInstance().getDeviceByName(this->model->getDeviceName());
        if (device != NULL)
        {
            this->macros = device->macroInfos();

            loadAtemMacro();
        }

        this->comboBoxMacro->setCurrentIndex(this->comboBoxMacro->findData(this->command->getMacro()));
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
    }

    blockAllSignals(false);
}

void InspectorAtemMacroWidget::atemDeviceChanged(const AtemDeviceChangedEvent& event)
{
    if (this->model != NULL)
    {
        // Should we update the device name?
        if (!event.getDeviceName().isEmpty() && event.getDeviceName() != this->model->getDeviceName())
        {
            const QSharedPointer<AtemDevice> device = AtemDeviceManager::getInstance().getDeviceByName(event.getDeviceName());
            if (device != NULL)
            {
                this->macros = device->macroInfos();

                loadAtemMacro();
            }
        }
    }
}

void InspectorAtemMacroWidget::loadAtemMacro()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values
    this->comboBoxMacro->blockSignals(true);

    this->comboBoxMacro->clear();
    foreach (QAtem::MacroInfo macroInfo, this->macros)
    {
        if (!macroInfo.used)
            continue;

        this->comboBoxMacro->addItem(macroInfo.name, macroInfo.index);
    }

    this->comboBoxMacro->blockSignals(false);
}

void InspectorAtemMacroWidget::blockAllSignals(bool block)
{
    this->comboBoxMacro->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
}

void InspectorAtemMacroWidget::macroChanged(int index)
{
    this->command->setMacro(this->comboBoxMacro->itemData(index).toString());
}

void InspectorAtemMacroWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}
