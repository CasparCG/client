#include "InspectorMovieWidget.h"
#include "Rundown/AbstractRundownWidget.h"
#include "Rundown/RundownGroupWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Commands/GroupCommand.h"
#include "Models/DirectionModel.h"
#include "Models/TransitionModel.h"
#include "Models/TweenModel.h"

InspectorMovieWidget::InspectorMovieWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL), enableOscInput(false)
{
    setupUi(this);

    this->enableOscInput = (DatabaseManager::getInstance().getConfigurationByName("EnableOscInput").getValue() == "true") ? true : false;

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));

    loadDirection();
    loadTransition();
    loadTween();
}

void InspectorMovieWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<MovieCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<MovieCommand*>(event.getCommand());

        this->comboBoxTransition->setCurrentIndex(this->comboBoxTransition->findText(this->command->getTransition()));
        this->spinBoxTransitionDuration->setValue(this->command->getTransitionDuration());
        this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
        this->comboBoxDirection->setCurrentIndex(this->comboBoxDirection->findText(this->command->getDirection()));
        this->spinBoxSeek->setValue(this->command->getSeek());
        this->spinBoxLength->setValue(this->command->getLength());
        this->checkBoxLoop->setChecked(this->command->getLoop());
        this->checkBoxFreezeOnLoad->setChecked(this->command->getFreezeOnLoad());
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());

        RundownGroupWidget* parent = dynamic_cast<RundownGroupWidget*>(event.getParent());
        AbstractRundownWidget* source = dynamic_cast<AbstractRundownWidget*>(event.getSource());

        // Only show auto play option if we are in a group. OSC needs to be enabled.
        if (this->enableOscInput && source != NULL && parent != NULL && source->isInGroup() && dynamic_cast<GroupCommand*>(parent->getCommand())->getAutoPlay())
        {
            this->labelAutoPlay->setEnabled(true);
            this->checkBoxAutoPlay->setEnabled(true);
            this->checkBoxAutoPlay->setChecked(this->command->getAutoPlay());
        }
        else
        {
            this->labelAutoPlay->setEnabled(false);
            this->checkBoxAutoPlay->setEnabled(false);
            this->checkBoxAutoPlay->setChecked(false);
        }
    }

    blockAllSignals(false);
}

void InspectorMovieWidget::blockAllSignals(bool block)
{
    this->comboBoxTransition->blockSignals(block);
    this->spinBoxTransitionDuration->blockSignals(block);
    this->comboBoxTween->blockSignals(block);
    this->comboBoxDirection->blockSignals(block);
    this->spinBoxSeek->blockSignals(block);
    this->spinBoxLength->blockSignals(block);
    this->checkBoxLoop->blockSignals(block);
    this->checkBoxFreezeOnLoad->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
    this->checkBoxAutoPlay->blockSignals(block);
}

void InspectorMovieWidget::loadDirection()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxDirection->blockSignals(true);

    QList<DirectionModel> models = DatabaseManager::getInstance().getDirection();
    foreach (DirectionModel model, models)
        this->comboBoxDirection->addItem(model.getValue());

    this->comboBoxDirection->blockSignals(false);
}

void InspectorMovieWidget::loadTransition()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTransition->blockSignals(true);

    QList<TransitionModel> models = DatabaseManager::getInstance().getTransition();
    foreach (TransitionModel model, models)
        this->comboBoxTransition->addItem(model.getValue());

    this->comboBoxTransition->blockSignals(false);
}

void InspectorMovieWidget::loadTween()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTween->blockSignals(true);

    QList<TweenModel> models = DatabaseManager::getInstance().getTween();
    foreach (TweenModel model, models)
        this->comboBoxTween->addItem(model.getValue());

    this->comboBoxTween->blockSignals(false);
}

void InspectorMovieWidget::transitionChanged(QString transition)
{
    this->command->setTransition(transition);
}

void InspectorMovieWidget::transitionDurationChanged(int transitionDuration)
{
    this->command->setTransitionDuration(transitionDuration);
}

void InspectorMovieWidget::directionChanged(QString direction)
{
    this->command->setDirection(direction);
}

void InspectorMovieWidget::tweenChanged(QString tween)
{
    this->command->setTween(tween);
}

void InspectorMovieWidget::loopChanged(int state)
{
    this->command->setLoop((state == Qt::Checked) ? true : false);
}

void InspectorMovieWidget::freezeOnLoadChanged(int state)
{
    this->command->setFreezeOnLoad((state == Qt::Checked) ? true : false);
}

void InspectorMovieWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}

void InspectorMovieWidget::seekChanged(int seek)
{
    this->command->setSeek(seek);
}

void InspectorMovieWidget::lengthChanged(int length)
{
    this->command->setLength(length);
}

void InspectorMovieWidget::autoPlayChanged(int state)
{
    this->command->setAutoPlay((state == Qt::Checked) ? true : false);
}
