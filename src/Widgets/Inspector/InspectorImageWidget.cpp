#include "InspectorImageWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "Events/RundownItemSelectedEvent.h"
#include "Models/DirectionModel.h"
#include "Models/TransitionModel.h"
#include "Models/TweenModel.h"

InspectorImageWidget::InspectorImageWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    loadDirection();
    loadTransition();
    loadTween();

    qApp->installEventFilter(this);
}

bool InspectorImageWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Event::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        this->model = rundownItemSelectedEvent->getLibraryModel();

        blockAllSignals(true);

        if (dynamic_cast<ImageCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->command = dynamic_cast<ImageCommand*>(rundownItemSelectedEvent->getCommand());

            this->comboBoxTransition->setCurrentIndex(this->comboBoxTransition->findText(this->command->getTransition()));
            this->spinBoxDuration->setValue(this->command->getDuration());
            this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
            this->comboBoxDirection->setCurrentIndex(this->comboBoxDirection->findText(this->command->getDirection()));
            this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
            this->checkBoxUseAuto->setChecked(this->command->getUseAuto());
        }

        blockAllSignals(false);
    }

    return QObject::eventFilter(target, event);
}

void InspectorImageWidget::blockAllSignals(bool block)
{
    this->comboBoxTransition->blockSignals(block);
    this->spinBoxDuration->blockSignals(block);
    this->comboBoxTween->blockSignals(block);
    this->comboBoxDirection->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
    this->checkBoxUseAuto->blockSignals(block);
}

void InspectorImageWidget::loadDirection()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxDirection->blockSignals(true);

    QList<DirectionModel> models = DatabaseManager::getInstance().getDirection();
    foreach (DirectionModel model, models)
        this->comboBoxDirection->addItem(model.getValue());

    this->comboBoxDirection->blockSignals(false);
}

void InspectorImageWidget::loadTransition()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTransition->blockSignals(true);

    QList<TransitionModel> models = DatabaseManager::getInstance().getTransition();
    foreach (TransitionModel model, models)
        this->comboBoxTransition->addItem(model.getValue());

    this->comboBoxTransition->blockSignals(false);
}

void InspectorImageWidget::loadTween()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTween->blockSignals(true);

    QList<TweenModel> models = DatabaseManager::getInstance().getTween();
    foreach (TweenModel model, models)
        this->comboBoxTween->addItem(model.getValue());

    this->comboBoxTween->blockSignals(false);
}

void InspectorImageWidget::transitionChanged(QString transition)
{
    this->command->setTransition(transition);
}

void InspectorImageWidget::durationChanged(int duration)
{
    this->command->setDuration(duration);
}

void InspectorImageWidget::directionChanged(QString direction)
{
    this->command->setDirection(direction);
}

void InspectorImageWidget::tweenChanged(QString tween)
{
    this->command->setTween(tween);
}

void InspectorImageWidget::useAutoChanged(int state)
{
    this->command->setUseAuto((state == Qt::Checked) ? true : false);
}

void InspectorImageWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}

void InspectorImageWidget::resetTransition(QString transition)
{
    this->comboBoxTransition->setCurrentIndex(this->comboBoxTransition->findText(Mixer::DEFAULT_TRANSITION));
    this->command->setTransition(this->comboBoxTransition->currentText());
}

void InspectorImageWidget::resetDuration(QString duration)
{
    this->spinBoxDuration->setValue(Mixer::DEFAULT_DURATION);
    this->command->setDuration(this->spinBoxDuration->value());
}

void InspectorImageWidget::resetDirection(QString direction)
{
    this->comboBoxDirection->setCurrentIndex(this->comboBoxDirection->findText(Mixer::DEFAULT_DIRECTION));
    this->command->setDirection(this->comboBoxDirection->currentText());
}

void InspectorImageWidget::resetTween(QString tween)
{
    this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(Mixer::DEFAULT_TWEEN));
    this->command->setTween(this->comboBoxTween->currentText());
}

void InspectorImageWidget::resetUseAuto(QString useAuto)
{
    this->checkBoxUseAuto->setChecked(Image::DEFAULT_USE_AUTO);
    this->command->setUseAuto(this->checkBoxUseAuto->isChecked());
}

void InspectorImageWidget::resetTriggerOnNext(QString triggerOnNext)
{
    this->checkBoxTriggerOnNext->setChecked(Image::DEFAULT_TRIGGER_ON_NEXT);
    this->command->setTriggerOnNext(this->checkBoxTriggerOnNext->isChecked());
}
