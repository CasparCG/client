#include "InspectorOscOutputWidget.h"

#include "Global.h"

#include "EventManager.h"
#include "DatabaseManager.h"

InspectorOscOutputWidget::InspectorOscOutputWidget(QWidget *parent)
    : QWidget(parent),
      command(0)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(oscOutputChanged(const OscOutputChangedEvent&)), this, SLOT(oscOutputChanged(const OscOutputChangedEvent&)));

    loadOscOutput();
}

void InspectorOscOutputWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    blockAllSignals(true);

    if (dynamic_cast<OscOutputCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<OscOutputCommand*>(event.getCommand());

        this->comboBoxOutput->setCurrentIndex(this->comboBoxOutput->findText(this->command->getOutput()));
        this->lineEditPath->setText(this->command->getPath());
        this->lineEditMessage->setText(this->command->getMessage());
        this->comboBoxType->setCurrentIndex(this->comboBoxType->findText(this->command->getType()));
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
        this->checkBoxUseBundle->setChecked(this->command->getUseBundle());
    }

    checkEmptyOutput();
    checkEmptyPath();

    blockAllSignals(false);
}

void InspectorOscOutputWidget::oscOutputChanged(const OscOutputChangedEvent& event)
{
    blockAllSignals(true);

    loadOscOutput();

    checkEmptyOutput();
    checkEmptyPath();

    blockAllSignals(false);
}

void InspectorOscOutputWidget::loadOscOutput()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxOutput->blockSignals(true);

    this->comboBoxOutput->clear();
    QList<OscOutputModel> models = DatabaseManager::getInstance().getOscOutput();
    foreach (OscOutputModel model, models)
        this->comboBoxOutput->addItem(model.getName());

    this->comboBoxOutput->blockSignals(false);
}

void InspectorOscOutputWidget::blockAllSignals(bool block)
{
    this->comboBoxOutput->blockSignals(block);
    this->lineEditPath->blockSignals(block);
    this->lineEditMessage->blockSignals(block);
    this->comboBoxType->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
    this->checkBoxUseBundle->blockSignals(block);
}

void InspectorOscOutputWidget::checkEmptyOutput()
{
    if (this->comboBoxOutput->isEnabled() && this->comboBoxOutput->currentText() == "")
        this->comboBoxOutput->setStyleSheet("border-color: firebrick;");
    else
        this->comboBoxOutput->setStyleSheet("");
}

void InspectorOscOutputWidget::checkEmptyPath()
{
    if (this->lineEditPath->text().isEmpty())
        this->lineEditPath->setStyleSheet("border-color: firebrick;");
    else
        this->lineEditPath->setStyleSheet("");
}

void InspectorOscOutputWidget::outputChanged(QString output)
{
    this->command->setOutput(output);

    checkEmptyOutput();
}

void InspectorOscOutputWidget::pathChanged(QString path)
{
    this->command->setPath(path);

    checkEmptyPath();
}

void InspectorOscOutputWidget::messageChanged(QString message)
{
    this->command->setMessage(message);
}

void InspectorOscOutputWidget::typeChanged(QString type)
{
    this->command->setType(type);
}

void InspectorOscOutputWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}

void InspectorOscOutputWidget::useBundleChanged(int state)
{
    this->command->setUseBundle((state == Qt::Checked) ? true : false);
}
