#include "InspectorDeckLinkInputWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "Events/RundownItemSelectedEvent.h"
#include "Models/FormatModel.h"

InspectorDeckLinkInputWidget::InspectorDeckLinkInputWidget(QWidget* parent)
    : QWidget(parent), preview(false), model(NULL), command(NULL)
{
    setupUi(this);

    loadDirection();
    loadFormat();
    loadTransition();
    loadTween();

    qApp->installEventFilter(this);
}

bool InspectorDeckLinkInputWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        if (dynamic_cast<DeckLinkInputCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->preview = false;

            this->model = rundownItemSelectedEvent->getLibraryModel();
            this->command = dynamic_cast<DeckLinkInputCommand*>(rundownItemSelectedEvent->getCommand());

            this->spinBoxDevice->setValue(this->command->getDevice());
            this->comboBoxFormat->setCurrentIndex(this->comboBoxFormat->findText(this->command->getFormat()));
            this->comboBoxTransition->setCurrentIndex(this->comboBoxTransition->findText(this->command->getTransition()));
            this->spinBoxDuration->setValue(this->command->getDuration());
            this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
            this->comboBoxDirection->setCurrentIndex(this->comboBoxDirection->findText(this->command->getDirection()));

            this->preview = true;
        }
    }

    return QObject::eventFilter(target, event);
}

void InspectorDeckLinkInputWidget::loadDirection()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxDirection->blockSignals(true);

    QList<DirectionModel> models = DatabaseManager::getInstance().getDirection();
    foreach (DirectionModel model, models)
        this->comboBoxDirection->addItem(model.getValue());

    this->comboBoxDirection->blockSignals(false);
}

void InspectorDeckLinkInputWidget::loadFormat()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxFormat->blockSignals(true);

    QList<FormatModel> models = DatabaseManager::getInstance().getFormat();
    foreach (FormatModel model, models)
        this->comboBoxFormat->addItem(model.getValue());

    this->comboBoxFormat->blockSignals(false);
}

void InspectorDeckLinkInputWidget::loadTransition()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTransition->blockSignals(true);

    QList<TransitionModel> models = DatabaseManager::getInstance().getTransition();
    foreach (TransitionModel model, models)
        this->comboBoxTransition->addItem(model.getValue());

    this->comboBoxTransition->blockSignals(false);
}

void InspectorDeckLinkInputWidget::loadTween()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTween->blockSignals(true);

    QList<TweenModel> models = DatabaseManager::getInstance().getTween();
    foreach (TweenModel model, models)
        this->comboBoxTween->addItem(model.getValue());

    this->comboBoxTween->blockSignals(false);
}

void InspectorDeckLinkInputWidget::deviceChanged(int device)
{
    this->command->setDevice(device);
}

void InspectorDeckLinkInputWidget::formatChanged(QString format)
{
    this->command->setFormat(format);
}

void InspectorDeckLinkInputWidget::transitionChanged(QString transition)
{
    this->command->setTransition(transition);
}

void InspectorDeckLinkInputWidget::durationChanged(int duration)
{
    this->command->setDuration(duration);
}

void InspectorDeckLinkInputWidget::directionChanged(QString direction)
{
    this->command->setDirection(direction);
}

void InspectorDeckLinkInputWidget::tweenChanged(QString tween)
{
    this->command->setTween(tween);
}

void InspectorDeckLinkInputWidget::resetTransition(QString transition)
{
    this->comboBoxTransition->setCurrentIndex(this->comboBoxTransition->findText("CUT"));
    this->command->setTransition(this->comboBoxTransition->currentText());
}

void InspectorDeckLinkInputWidget::resetDuration(QString duration)
{
    this->spinBoxDuration->setValue(Mixer::DEFAULT_DURATION);
    this->command->setDuration(this->spinBoxDuration->value());
}

void InspectorDeckLinkInputWidget::resetDirection(QString direction)
{
    this->comboBoxDirection->setCurrentIndex(this->comboBoxDirection->findText("RIGHT"));
    this->command->setDirection(this->comboBoxDirection->currentText());
}

void InspectorDeckLinkInputWidget::resetTween(QString tween)
{
    this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText("Linear"));
    this->command->setTween(this->comboBoxTween->currentText());
}

void InspectorDeckLinkInputWidget::resetDevice(QString)
{
    this->spinBoxDevice->setValue(DeckLinkInput::DEFAULT_DEVICE);
    this->command->setDevice(this->spinBoxDevice->value());
}

void InspectorDeckLinkInputWidget::resetFormat(QString)
{
    this->comboBoxFormat->setCurrentIndex(this->comboBoxFormat->findText(DeckLinkInput::DEFAULT_FORMAT));
    this->command->setFormat(this->comboBoxFormat->currentText());
}
