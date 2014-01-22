#include "InspectorGpiOutputWidget.h"

#include "Events/Rundown/RundownItemSelectedEvent.h"

#include "Global.h"

InspectorGpiOutputWidget::InspectorGpiOutputWidget(QWidget *parent)
    : QWidget(parent),
      command(0)
{
    setupUi(this);

    qApp->installEventFilter(this);
}

bool InspectorGpiOutputWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Event::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);

        blockAllSignals(true);

        if (dynamic_cast<GpiOutputCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->command = dynamic_cast<GpiOutputCommand*>(rundownItemSelectedEvent->getCommand());

            this->spinBoxGpoPort->setValue(this->command->getGpoPort() + 1);
            this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
        }

        blockAllSignals(false);
    }

    return QObject::eventFilter(target, event);
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
