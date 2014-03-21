#include "InspectorGpiOutputWidget.h"

#include "Global.h"

#include "EventManager.h"

InspectorGpiOutputWidget::InspectorGpiOutputWidget(QWidget *parent)
    : QWidget(parent),
      command(0)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
}

void InspectorGpiOutputWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    blockAllSignals(true);

    if (dynamic_cast<GpiOutputCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<GpiOutputCommand*>(event.getCommand());

        this->spinBoxGpoPort->setValue(this->command->getGpoPort() + 1);
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
    }

    blockAllSignals(false);
}

void InspectorGpiOutputWidget::blockAllSignals(bool block)
{
    this->spinBoxGpoPort->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
}

void InspectorGpiOutputWidget::gpoPortChanged(int port)
{
    this->command->setGpoPort(port - 1);
}

void InspectorGpiOutputWidget::resetGpoPort(QString)
{
    this->spinBoxGpoPort->setValue(GpiOutput::DEFAULT_PORT + 1);
    this->command->setGpoPort(this->spinBoxGpoPort->value() - 1);
}

void InspectorGpiOutputWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}
