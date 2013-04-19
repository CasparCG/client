#include "InspectorClearOutputWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "Events/PreviewEvent.h"
#include "Events/RundownItemSelectedEvent.h"
#include "Models/TweenModel.h"

#include <QtCore/QDebug>

#include <QtGui/QApplication>

InspectorClearOutputWidget::InspectorClearOutputWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    qApp->installEventFilter(this);
}

bool InspectorClearOutputWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        this->model = rundownItemSelectedEvent->getLibraryModel();

        blockAllSignals(true);

        if (dynamic_cast<ClearOutputCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->command = dynamic_cast<ClearOutputCommand*>(rundownItemSelectedEvent->getCommand());

            this->checkBoxClearChannel->setChecked(this->command->getClearChannel());
            this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
        }

        blockAllSignals(false);
    }

    return QObject::eventFilter(target, event);
}

void InspectorClearOutputWidget::blockAllSignals(bool block)
{
    this->checkBoxClearChannel->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
}

void InspectorClearOutputWidget::clearChannelChanged(int state)
{
    this->command->setClearChannel((state == Qt::Checked) ? true : false);
}

void InspectorClearOutputWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}

void InspectorClearOutputWidget::resetClearChannel(QString clearChannel)
{
    this->checkBoxClearChannel->setChecked(ClearOutput::DEFAULT_CLEAR_CHANNEL);
    this->command->setClearChannel(this->checkBoxClearChannel->isChecked());
}

void InspectorClearOutputWidget::resetTriggerOnNext(QString triggerOnNext)
{
    this->checkBoxTriggerOnNext->setChecked(ClearOutput::DEFAULT_TRIGGER_ON_NEXT);
    this->command->setTriggerOnNext(this->checkBoxTriggerOnNext->isChecked());
}
