#include "InspectorOpacityWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/RundownItemSelectedEvent.h"
#include "Models/TweenModel.h"

#include <QtGui/QApplication>

InspectorOpacityWidget::InspectorOpacityWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    loadTween();

    qApp->installEventFilter(this);
}

bool InspectorOpacityWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        this->model = rundownItemSelectedEvent->getLibraryModel();

        blockAllSignals(true);

        if (dynamic_cast<OpacityCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->command = dynamic_cast<OpacityCommand*>(rundownItemSelectedEvent->getCommand());

            this->sliderOpacity->setValue(QString("%1").arg(this->command->getOpacity() * 100).toFloat());
            this->spinBoxOpacity->setValue(QString("%1").arg(this->command->getOpacity() * 100).toFloat());
            this->spinBoxDuration->setValue(this->command->getDuration());
            this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
            this->checkBoxDefer->setChecked(this->command->getDefer());
        }

        blockAllSignals(false);
    }

    return QObject::eventFilter(target, event);
}

void InspectorOpacityWidget::blockAllSignals(bool block)
{
    this->spinBoxOpacity->blockSignals(block);
    this->spinBoxDuration->blockSignals(block);
    this->comboBoxTween->blockSignals(block);
    this->checkBoxDefer->blockSignals(block);
}

void InspectorOpacityWidget::loadTween()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTween->blockSignals(true);

    QList<TweenModel> models = DatabaseManager::getInstance().getTween();
    foreach (TweenModel model, models)
        this->comboBoxTween->addItem(model.getValue());

    this->comboBoxTween->blockSignals(false);
}

void InspectorOpacityWidget::durationChanged(int duration)
{
    this->command->setDuration(duration);
}

void InspectorOpacityWidget::tweenChanged(QString tween)
{
    this->command->setTween(tween);
}

void InspectorOpacityWidget::sliderOpacityChanged(int opacity)
{
    this->command->setOpacity(static_cast<float>(opacity) / 100);

    this->spinBoxOpacity->setValue(opacity);

    EventManager::getInstance().firePreviewEvent();
}

void InspectorOpacityWidget::spinBoxOpacityChanged(int opacity)
{
    this->sliderOpacity->setValue(opacity);
}

void InspectorOpacityWidget::resetOpacity(QString opacity)
{
    this->sliderOpacity->setValue(Mixer::DEFAULT_OPACITY * 100);
    this->command->setOpacity(static_cast<float>(this->sliderOpacity->value()) / 100);

    EventManager::getInstance().firePreviewEvent();
}

void InspectorOpacityWidget::resetDuration(QString duration)
{
    this->spinBoxDuration->setValue(Mixer::DEFAULT_DURATION);
    this->command->setDuration(this->spinBoxDuration->value());
}

void InspectorOpacityWidget::resetTween(QString tween)
{
    this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(Mixer::DEFAULT_TWEEN));
    this->command->setTween(this->comboBoxTween->currentText());
}

void InspectorOpacityWidget::deferChanged(int state)
{
    this->command->setDefer((state == Qt::Checked) ? true : false);
}

void InspectorOpacityWidget::resetDefer(QString defer)
{
    this->checkBoxDefer->setChecked(Mixer::DEFAULT_DEFER);
    this->command->setDefer(this->checkBoxDefer->isChecked());
}
