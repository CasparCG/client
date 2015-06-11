#include "InspectorSonyPresetWidget.h"

#include "Global.h"

#include "EventManager.h"
#include "DatabaseManager.h"

InspectorSonyPresetWidget::InspectorSonyPresetWidget(QWidget *parent)
    : QWidget(parent),
      command(0)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
}

void InspectorSonyPresetWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->command = nullptr;

    blockAllSignals(true);

    if (dynamic_cast<SonyPresetCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<SonyPresetCommand*>(event.getCommand());

        this->lineEditAddress->setText(this->command->getAddress());
        this->spinBoxPreset->setValue(this->command->getPreset());
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
    }

    checkEmptyAddress();

    blockAllSignals(false);
}

void InspectorSonyPresetWidget::blockAllSignals(bool block)
{
    this->lineEditAddress->blockSignals(block);
    this->spinBoxPreset->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
}

void InspectorSonyPresetWidget::checkEmptyAddress()
{
    if (this->lineEditAddress->text().isEmpty())
        this->lineEditAddress->setStyleSheet("border-color: firebrick;");
    else
        this->lineEditAddress->setStyleSheet("");
}

void InspectorSonyPresetWidget::addressChanged(QString address)
{
    this->command->setAddress(address);

    checkEmptyAddress();
}

void InspectorSonyPresetWidget::presetChanged(int preset)
{
    this->command->setPreset(preset);
}

void InspectorSonyPresetWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}
