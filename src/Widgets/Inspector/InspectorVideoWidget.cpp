#include "InspectorVideoWidget.h"
#include "Rundown/AbstractRundownWidget.h"
#include "Rundown/RundownGroupWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "Commands/GroupCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/DirectionModel.h"
#include "Models/TransitionModel.h"
#include "Models/TweenModel.h"

InspectorVideoWidget::InspectorVideoWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL), disableVideoProgress(false)
{
    setupUi(this);

    this->disableVideoProgress = (DatabaseManager::getInstance().getConfigurationByName("DisableVideoProgress").getValue() == "true") ? true : false;

    loadDirection();
    loadTransition();
    loadTween();

    qApp->installEventFilter(this);
}

bool InspectorVideoWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Event::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        this->model = rundownItemSelectedEvent->getLibraryModel();

        blockAllSignals(true);

        if (dynamic_cast<VideoCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->command = dynamic_cast<VideoCommand*>(rundownItemSelectedEvent->getCommand());

            this->comboBoxTransition->setCurrentIndex(this->comboBoxTransition->findText(this->command->getTransition()));
            this->spinBoxDuration->setValue(this->command->getDuration());
            this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
            this->comboBoxDirection->setCurrentIndex(this->comboBoxDirection->findText(this->command->getDirection()));
            this->spinBoxSeek->setValue(this->command->getSeek());
            this->spinBoxLength->setValue(this->command->getLength());
            this->checkBoxLoop->setChecked(this->command->getLoop());
            this->checkBoxFreezeOnLoad->setChecked(this->command->getFreezeOnLoad());
            this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());

            RundownGroupWidget* parent = dynamic_cast<RundownGroupWidget*>(rundownItemSelectedEvent->getParent());
            AbstractRundownWidget* source = dynamic_cast<AbstractRundownWidget*>(rundownItemSelectedEvent->getSource());

            // Only show auto play option if we are in a group. OSC needs to be enabled.
            if (!this->disableVideoProgress && source != NULL && parent != NULL && source->isInGroup() && dynamic_cast<GroupCommand*>(parent->getCommand())->getAutoPlay())
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

    return QObject::eventFilter(target, event);
}

void InspectorVideoWidget::blockAllSignals(bool block)
{
    this->comboBoxTransition->blockSignals(block);
    this->spinBoxDuration->blockSignals(block);
    this->comboBoxTween->blockSignals(block);
    this->comboBoxDirection->blockSignals(block);
    this->spinBoxSeek->blockSignals(block);
    this->spinBoxLength->blockSignals(block);
    this->checkBoxLoop->blockSignals(block);
    this->checkBoxFreezeOnLoad->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
    this->checkBoxAutoPlay->blockSignals(block);
}

void InspectorVideoWidget::loadDirection()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxDirection->blockSignals(true);

    QList<DirectionModel> models = DatabaseManager::getInstance().getDirection();
    foreach (DirectionModel model, models)
        this->comboBoxDirection->addItem(model.getValue());

    this->comboBoxDirection->blockSignals(false);
}

void InspectorVideoWidget::loadTransition()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTransition->blockSignals(true);

    QList<TransitionModel> models = DatabaseManager::getInstance().getTransition();
    foreach (TransitionModel model, models)
        this->comboBoxTransition->addItem(model.getValue());

    this->comboBoxTransition->blockSignals(false);
}

void InspectorVideoWidget::loadTween()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTween->blockSignals(true);

    QList<TweenModel> models = DatabaseManager::getInstance().getTween();
    foreach (TweenModel model, models)
        this->comboBoxTween->addItem(model.getValue());

    this->comboBoxTween->blockSignals(false);
}

void InspectorVideoWidget::transitionChanged(QString transition)
{
    this->command->setTransition(transition);
}

void InspectorVideoWidget::durationChanged(int duration)
{
    this->command->setDuration(duration);
}

void InspectorVideoWidget::directionChanged(QString direction)
{
    this->command->setDirection(direction);
}

void InspectorVideoWidget::tweenChanged(QString tween)
{
    this->command->setTween(tween);
}

void InspectorVideoWidget::loopChanged(int state)
{
    this->command->setLoop((state == Qt::Checked) ? true : false);
}

void InspectorVideoWidget::freezeOnLoadChanged(int state)
{
    this->command->setFreezeOnLoad((state == Qt::Checked) ? true : false);
}

void InspectorVideoWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}

void InspectorVideoWidget::seekChanged(int seek)
{
    this->command->setSeek(seek);
}

void InspectorVideoWidget::lengthChanged(int length)
{
    this->command->setLength(length);
}

void InspectorVideoWidget::resetTransition(QString transition)
{
    this->comboBoxTransition->setCurrentIndex(this->comboBoxTransition->findText(Mixer::DEFAULT_TRANSITION));
    this->command->setTransition(this->comboBoxTransition->currentText());
}

void InspectorVideoWidget::resetDuration(QString duration)
{
    this->spinBoxDuration->setValue(Mixer::DEFAULT_DURATION);
    this->command->setDuration(this->spinBoxDuration->value());
}

void InspectorVideoWidget::resetDirection(QString direction)
{
    this->comboBoxDirection->setCurrentIndex(this->comboBoxDirection->findText(Mixer::DEFAULT_DIRECTION));
    this->command->setDirection(this->comboBoxDirection->currentText());
}

void InspectorVideoWidget::resetTween(QString tween)
{
    this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(Mixer::DEFAULT_TWEEN));
    this->command->setTween(this->comboBoxTween->currentText());
}

void InspectorVideoWidget::resetSeek(QString seek)
{
    this->spinBoxSeek->setValue(Video::DEFAULT_SEEK);
    this->command->setSeek(this->spinBoxSeek->value());
}

void InspectorVideoWidget::resetLength(QString length)
{
    this->spinBoxLength->setValue(Video::DEFAULT_LENGTH);
    this->command->setLength(this->spinBoxLength->value());
}

void InspectorVideoWidget::resetLoop(QString loop)
{
    this->checkBoxLoop->setChecked(Video::DEFAULT_LOOP);
    this->command->setLoop(this->checkBoxLoop->isChecked());
}

void InspectorVideoWidget::resetFreezeOnLoad(QString freezeOnLoad)
{
    this->checkBoxFreezeOnLoad->setChecked(Video::DEFAULT_FREEZE_ON_LOAD);
    this->command->setFreezeOnLoad(this->checkBoxFreezeOnLoad->isChecked());
}

void InspectorVideoWidget::resetTriggerOnNext(QString triggerOnNext)
{
    this->checkBoxTriggerOnNext->setChecked(Video::DEFAULT_TRIGGER_ON_NEXT);
    this->command->setTriggerOnNext(this->checkBoxTriggerOnNext->isChecked());
}

void InspectorVideoWidget::autoPlayChanged(int state)
{
    this->command->setAutoPlay((state == Qt::Checked) ? true : false);
}
