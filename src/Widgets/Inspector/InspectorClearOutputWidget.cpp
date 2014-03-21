#include "InspectorClearOutputWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/PreviewEvent.h"
#include "Models/TweenModel.h"

#include <QtCore/QDebug>

#include <QtGui/QApplication>

InspectorClearOutputWidget::InspectorClearOutputWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
}

void InspectorClearOutputWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<ClearOutputCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<ClearOutputCommand*>(event.getCommand());

        this->checkBoxClearChannel->setChecked(this->command->getClearChannel());
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
    }

    blockAllSignals(false);
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
