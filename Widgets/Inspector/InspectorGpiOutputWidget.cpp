#include "InspectorGpiOutputWidget.h"

#include "Events/RundownItemSelectedEvent.h"

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
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        if (dynamic_cast<GpiOutputCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->command = dynamic_cast<GpiOutputCommand*>(rundownItemSelectedEvent->getCommand());

            this->spinBoxGpoPort->setValue(this->command->getGpoPort() + 1);
        }
    }

    return QObject::eventFilter(target, event);
}

void InspectorGpiOutputWidget::gpoPortChanged(int port)
{
    this->command->setGpoPort(port - 1);
}

void InspectorGpiOutputWidget::resetGpoPort(QString)
{
    this->spinBoxGpoPort->setValue(Gpi::DEFAULT_GPO_PORT + 1);
    this->command->setGpoPort(this->spinBoxGpoPort->value() - 1);
}
