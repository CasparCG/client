#include "InspectorStillWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Models/DirectionModel.h"
#include "Models/TransitionModel.h"
#include "Models/TweenModel.h"

InspectorStillWidget::InspectorStillWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));

    loadDirection();
    loadTransition();
    loadTween();
}

void InspectorStillWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<StillCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<StillCommand*>(event.getCommand());

        this->comboBoxTransition->setCurrentIndex(this->comboBoxTransition->findText(this->command->getTransition()));
        this->spinBoxTransitionDuration->setValue(this->command->getTransitionDuration());
        this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
        this->comboBoxDirection->setCurrentIndex(this->comboBoxDirection->findText(this->command->getDirection()));
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
        this->checkBoxUseAuto->setChecked(this->command->getUseAuto());
    }

    blockAllSignals(false);
}

void InspectorStillWidget::blockAllSignals(bool block)
{
    this->comboBoxTransition->blockSignals(block);
    this->spinBoxTransitionDuration->blockSignals(block);
    this->comboBoxTween->blockSignals(block);
    this->comboBoxDirection->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
    this->checkBoxUseAuto->blockSignals(block);
}

void InspectorStillWidget::loadDirection()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxDirection->blockSignals(true);

    QList<DirectionModel> models = DatabaseManager::getInstance().getDirection();
    foreach (DirectionModel model, models)
        this->comboBoxDirection->addItem(model.getValue());

    this->comboBoxDirection->blockSignals(false);
}

void InspectorStillWidget::loadTransition()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTransition->blockSignals(true);

    QList<TransitionModel> models = DatabaseManager::getInstance().getTransition();
    foreach (TransitionModel model, models)
        this->comboBoxTransition->addItem(model.getValue());

    this->comboBoxTransition->blockSignals(false);
}

void InspectorStillWidget::loadTween()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTween->blockSignals(true);

    QList<TweenModel> models = DatabaseManager::getInstance().getTween();
    foreach (TweenModel model, models)
        this->comboBoxTween->addItem(model.getValue());

    this->comboBoxTween->blockSignals(false);
}

void InspectorStillWidget::transitionChanged(QString transition)
{
    this->command->setTransition(transition);
}

void InspectorStillWidget::transitionDurationChanged(int transitionDuration)
{
    this->command->setTransitionDuration(transitionDuration);
}

void InspectorStillWidget::directionChanged(QString direction)
{
    this->command->setDirection(direction);
}

void InspectorStillWidget::tweenChanged(QString tween)
{
    this->command->setTween(tween);
}

void InspectorStillWidget::useAutoChanged(int state)
{
    this->command->setUseAuto((state == Qt::Checked) ? true : false);
}

void InspectorStillWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}
