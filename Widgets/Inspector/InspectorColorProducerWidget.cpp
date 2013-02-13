#include "InspectorColorProducerWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "Events/RundownItemSelectedEvent.h"
#include "Models/DirectionModel.h"
#include "Models/TransitionModel.h"
#include "Models/TweenModel.h"

#include <QtGui/QColorDialog>

InspectorColorProducerWidget::InspectorColorProducerWidget(QWidget* parent)
    : QWidget(parent),
      preview(false), model(NULL), command(NULL)
{
    setupUi(this);

    loadDirection();
    loadTransition();
    loadTween();

    qApp->installEventFilter(this);
}

bool InspectorColorProducerWidget::eventFilter(QObject* target, QEvent* event)
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

void InspectorColorProducerWidget::loadDirection()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxDirection->blockSignals(true);

    QList<DirectionModel> models = DatabaseManager::getInstance().getDirection();
    foreach (DirectionModel model, models)
        this->comboBoxDirection->addItem(model.getValue());

    this->comboBoxDirection->blockSignals(false);
}

void InspectorColorProducerWidget::loadTransition()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTransition->blockSignals(true);

    QList<TransitionModel> models = DatabaseManager::getInstance().getTransition();
    foreach (TransitionModel model, models)
        this->comboBoxTransition->addItem(model.getValue());

    this->comboBoxTransition->blockSignals(false);
}

void InspectorColorProducerWidget::loadTween()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTween->blockSignals(true);

    QList<TweenModel> models = DatabaseManager::getInstance().getTween();
    foreach (TweenModel model, models)
        this->comboBoxTween->addItem(model.getValue());

    this->comboBoxTween->blockSignals(false);
}

void InspectorColorProducerWidget::colorDialogClicked()
{
    QColor color = QColorDialog::getColor();
}

void InspectorColorProducerWidget::transitionChanged(QString transition)
{
    this->command->setTransition(transition);
}

void InspectorColorProducerWidget::durationChanged(int duration)
{
    this->command->setDuration(duration);
}

void InspectorColorProducerWidget::directionChanged(QString direction)
{
    this->command->setDirection(direction);
}

void InspectorColorProducerWidget::tweenChanged(QString tween)
{
    this->command->setTween(tween);
}

void InspectorColorProducerWidget::useAutoChanged(int state)
{
    this->command->setUseAuto((state == Qt::Checked) ? true : false);
}

void InspectorColorProducerWidget::resetTransition(QString transition)
{
    this->comboBoxTransition->setCurrentIndex(this->comboBoxTransition->findText(Mixer::DEFAULT_TRANSITION));
    this->command->setTransition(this->comboBoxTransition->currentText());
}

void InspectorColorProducerWidget::resetDuration(QString duration)
{
    this->spinBoxDuration->setValue(Mixer::DEFAULT_DURATION);
    this->command->setDuration(this->spinBoxDuration->value());
}

void InspectorColorProducerWidget::resetDirection(QString direction)
{
    this->comboBoxDirection->setCurrentIndex(this->comboBoxDirection->findText(Mixer::DEFAULT_DIRECTION));
    this->command->setDirection(this->comboBoxDirection->currentText());
}

void InspectorColorProducerWidget::resetTween(QString tween)
{
    this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(Mixer::DEFAULT_TWEEN));
    this->command->setTween(this->comboBoxTween->currentText());
}

void InspectorColorProducerWidget::resetUseAuto(QString useAuto)
{
    this->checkBoxUseAuto->setChecked(false);
    this->command->setUseAuto(this->checkBoxUseAuto->isChecked());
}
