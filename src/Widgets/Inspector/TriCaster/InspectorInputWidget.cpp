#include "InspectorInputWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/TriCaster/TriCasterSwitcherModel.h"
#include "Models/TriCaster/TriCasterInputModel.h"

#include <QtGui/QApplication>

InspectorInputWidget::InspectorInputWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    loadTriCasterSwitcher();
    loadTriCasterInput();

    qApp->installEventFilter(this);
}

bool InspectorInputWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Event::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        this->model = rundownItemSelectedEvent->getLibraryModel();

        blockAllSignals(true);

        if (dynamic_cast<InputCommand*>(rundownItemSelectedEvent->getCommand()))
        {  
            this->command = dynamic_cast<InputCommand*>(rundownItemSelectedEvent->getCommand());

            this->comboBoxSwitcher->setCurrentIndex(this->comboBoxSwitcher->findData(this->command->getSwitcher()));
            this->comboBoxInput->setCurrentIndex(this->comboBoxInput->findData(this->command->getInput()));
        }

        blockAllSignals(false);
    }

    return QObject::eventFilter(target, event);
}

void InspectorInputWidget::blockAllSignals(bool block)
{
    this->comboBoxSwitcher->blockSignals(block);
    this->comboBoxInput->blockSignals(block);
}

void InspectorInputWidget::loadTriCasterSwitcher()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxSwitcher->blockSignals(true);

    QList<TriCasterSwitcherModel> models = DatabaseManager::getInstance().getTriCasterSwitcher();
    foreach (TriCasterSwitcherModel model, models)
        this->comboBoxSwitcher->addItem(model.getName(), model.getValue());

    this->comboBoxSwitcher->blockSignals(false);
}

void InspectorInputWidget::loadTriCasterInput()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxInput->blockSignals(true);

    QList<TriCasterInputModel> models = DatabaseManager::getInstance().getTriCasterInput();
    foreach (TriCasterInputModel model, models)
        this->comboBoxInput->addItem(model.getName(), model.getValue());

    this->comboBoxInput->blockSignals(false);
}

void InspectorInputWidget::switcherChanged(int index)
{
    this->command->setSwitcher(this->comboBoxSwitcher->itemData(index).toString());
}

void InspectorInputWidget::inputChanged(int index)
{
    this->command->setInput(this->comboBoxInput->itemData(index).toString());
}
