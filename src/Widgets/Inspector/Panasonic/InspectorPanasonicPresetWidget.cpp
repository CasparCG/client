#include "InspectorPanasonicPresetWidget.h"

#include "Global.h"

#include "EventManager.h"
#include "DatabaseManager.h"

InspectorPanasonicPresetWidget::InspectorPanasonicPresetWidget(QWidget *parent)
    : QWidget(parent),
      command(0)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
}

void InspectorPanasonicPresetWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    blockAllSignals(true);

    if (dynamic_cast<PanasonicPresetCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<PanasonicPresetCommand*>(event.getCommand());

        this->lineEditAddress->setText(this->command->getAddress());
        this->spinBoxPreset->setValue(this->command->getPreset());
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
    }

    checkEmptyAddress();

    blockAllSignals(false);
}

void InspectorPanasonicPresetWidget::blockAllSignals(bool block)
{
    this->lineEditAddress->blockSignals(block);
    this->spinBoxPreset->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
}

void InspectorPanasonicPresetWidget::checkEmptyAddress()
{
    if (this->lineEditAddress->text().isEmpty())
        this->lineEditAddress->setStyleSheet("border-color: firebrick;");
    else
        this->lineEditAddress->setStyleSheet("");
}

void InspectorPanasonicPresetWidget::addressChanged(QString address)
{
    this->command->setAddress(address);

    checkEmptyAddress();
}

void InspectorPanasonicPresetWidget::presetChanged(int preset)
{
    this->command->setPreset(preset);
}

void InspectorPanasonicPresetWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}
