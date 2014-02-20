#include "InspectorNetworkSourceWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/TriCaster/TriCasterNetworkTargetModel.h"

#include <QtGui/QApplication>

InspectorNetworkSourceWidget::InspectorNetworkSourceWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    loadTriCasterNetworkTarget();

    qApp->installEventFilter(this);
}

bool InspectorNetworkSourceWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Event::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        this->model = rundownItemSelectedEvent->getLibraryModel();

        blockAllSignals(true);

        if (dynamic_cast<NetworkSourceCommand*>(rundownItemSelectedEvent->getCommand()))
        {  
            this->command = dynamic_cast<NetworkSourceCommand*>(rundownItemSelectedEvent->getCommand());

            this->comboBoxTarget->setCurrentIndex(this->comboBoxTarget->findData(this->command->getTarget()));
            this->lineEditSource->setText(this->command->getSource());
            this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
        }

        blockAllSignals(false);
    }

    return QObject::eventFilter(target, event);
}

void InspectorNetworkSourceWidget::blockAllSignals(bool block)
{
    this->comboBoxTarget->blockSignals(block);
    this->lineEditSource->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
}

void InspectorNetworkSourceWidget::loadTriCasterNetworkTarget()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTarget->blockSignals(true);

    QList<TriCasterNetworkTargetModel> models = DatabaseManager::getInstance().getTriCasterNetworkTarget();
    foreach (TriCasterNetworkTargetModel model, models)
        this->comboBoxTarget->addItem(model.getName(), model.getValue());

    this->comboBoxTarget->blockSignals(false);
}

void InspectorNetworkSourceWidget::targetChanged(int index)
{
    this->command->setTarget(this->comboBoxTarget->itemData(index).toString());
}

void InspectorNetworkSourceWidget::sourceChanged(QString source)
{
    this->command->setSource(source);
}

void InspectorNetworkSourceWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}
