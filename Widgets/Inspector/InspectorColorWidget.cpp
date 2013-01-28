#include "InspectorColorWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "Events/RundownItemSelectedEvent.h"
#include "Models/DirectionModel.h"
#include "Models/TransitionModel.h"
#include "Models/TweenModel.h"

InspectorColorWidget::InspectorColorWidget(QWidget* parent)
    : QWidget(parent),
      preview(false), model(NULL), command(NULL)
{
    setupUi(this);

    loadDirection();
    loadTransition();
    loadTween();

    qApp->installEventFilter(this);
}

bool InspectorColorWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        if (dynamic_cast<ColorCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->preview = false;

            this->model = rundownItemSelectedEvent->getLibraryModel();
            this->command = dynamic_cast<ColorCommand*>(rundownItemSelectedEvent->getCommand());

            this->comboBoxTransition->setCurrentIndex(this->comboBoxTransition->findText(this->command->getTransition()));
            this->spinBoxDuration->setValue(this->command->getDuration());
            this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
            this->comboBoxDirection->setCurrentIndex(this->comboBoxDirection->findText(this->command->getDirection()));

            this->preview = true;
        }
    }

    return QObject::eventFilter(target, event);
}

void InspectorColorWidget::loadDirection()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxDirection->blockSignals(true);

    QList<DirectionModel> models = DatabaseManager::getInstance().getDirection();
    foreach (DirectionModel model, models)
        this->comboBoxDirection->addItem(model.getValue());

    this->comboBoxDirection->blockSignals(false);
}

void InspectorColorWidget::loadTransition()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTransition->blockSignals(true);

    QList<TransitionModel> models = DatabaseManager::getInstance().getTransition();
    foreach (TransitionModel model, models)
        this->comboBoxTransition->addItem(model.getValue());

    this->comboBoxTransition->blockSignals(false);
}

void InspectorColorWidget::loadTween()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTween->blockSignals(true);

    QList<TweenModel> models = DatabaseManager::getInstance().getTween();
    foreach (TweenModel model, models)
        this->comboBoxTween->addItem(model.getValue());

    this->comboBoxTween->blockSignals(false);
}

void InspectorColorWidget::transitionChanged(QString transition)
{
    this->command->setTransition(transition);
}

void InspectorColorWidget::durationChanged(int duration)
{
    this->command->setDuration(duration);
}

void InspectorColorWidget::directionChanged(QString direction)
{
    this->command->setDirection(direction);
}

void InspectorColorWidget::tweenChanged(QString tween)
{
    this->command->setTween(tween);
}

void InspectorColorWidget::resetTransition(QString transition)
{
    this->comboBoxTransition->setCurrentIndex(this->comboBoxTransition->findText(Mixer::DEFAULT_TRANSITION));
    this->command->setTransition(this->comboBoxTransition->currentText());
}

void InspectorColorWidget::resetDuration(QString duration)
{
    this->spinBoxDuration->setValue(Mixer::DEFAULT_DURATION);
    this->command->setDuration(this->spinBoxDuration->value());
}

void InspectorColorWidget::resetDirection(QString direction)
{
    this->comboBoxDirection->setCurrentIndex(this->comboBoxDirection->findText(Mixer::DEFAULT_DIRECTION));
    this->command->setDirection(this->comboBoxDirection->currentText());
}

void InspectorColorWidget::resetTween(QString tween)
{
    this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(Mixer::DEFAULT_TWEEN));
    this->command->setTween(this->comboBoxTween->currentText());
}
