#include "InspectorTakeWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/TriCaster/TriCasterStepModel.h"

#include <QtGui/QApplication>

InspectorTakeWidget::InspectorTakeWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    loadTriCasterStep();

    qApp->installEventFilter(this);
}

bool InspectorTakeWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Event::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        this->model = rundownItemSelectedEvent->getLibraryModel();

        blockAllSignals(true);

        if (dynamic_cast<TakeCommand*>(rundownItemSelectedEvent->getCommand()))
        {  
            this->command = dynamic_cast<TakeCommand*>(rundownItemSelectedEvent->getCommand());

            this->comboBoxStep->setCurrentIndex(this->comboBoxStep->findData(this->command->getStep()));
        }

        checkEmptyStep();

        blockAllSignals(false);
    }

    return QObject::eventFilter(target, event);
}

void InspectorTakeWidget::blockAllSignals(bool block)
{
    this->comboBoxStep->blockSignals(block);
}

void InspectorTakeWidget::loadTriCasterStep()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxStep->blockSignals(true);

    QList<TriCasterStepModel> models = DatabaseManager::getInstance().getTriCasterStep();
    foreach (TriCasterStepModel model, models)
        this->comboBoxStep->addItem(model.getName(), model.getValue());

    this->comboBoxStep->blockSignals(false);
}

void InspectorTakeWidget::checkEmptyStep()
{
    if (this->comboBoxStep->isEnabled() && this->comboBoxStep->currentText() == "")
        this->comboBoxStep->setStyleSheet("border-color: red;");
    else
        this->comboBoxStep->setStyleSheet("");
}

void InspectorTakeWidget::stepChanged(int index)
{
    this->command->setStep(this->comboBoxStep->itemData(index).toString());

    checkEmptyStep();
}
