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
    blockAllSignals(true);

    if (dynamic_cast<SonyPresetCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<SonyPresetCommand*>(event.getCommand());

        this->lineEditAddress->setText(this->command->getAddress());
        this->lineEditPreset->setText(this->command->getPreset());
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
    }

    checkEmptyAddress();
    checkEmptyPreset();

    blockAllSignals(false);
}

void InspectorSonyPresetWidget::blockAllSignals(bool block)
{
    this->lineEditAddress->blockSignals(block);
    this->lineEditPreset->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
}

void InspectorSonyPresetWidget::checkEmptyAddress()
{
    if (this->lineEditAddress->text().isEmpty())
        this->lineEditAddress->setStyleSheet("border-color: firebrick;");
    else
        this->lineEditAddress->setStyleSheet("");
}

void InspectorSonyPresetWidget::checkEmptyPreset()
{
    if (this->lineEditPreset->text().isEmpty())
        this->lineEditPreset->setStyleSheet("border-color: firebrick;");
    else
        this->lineEditPreset->setStyleSheet("");
}

void InspectorSonyPresetWidget::addressChanged(QString address)
{
    this->command->setAddress(address);

    checkEmptyAddress();
}

void InspectorSonyPresetWidget::presetChanged(QString preset)
{
    this->command->setPreset(preset);

    checkEmptyPreset();
}

void InspectorSonyPresetWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}
