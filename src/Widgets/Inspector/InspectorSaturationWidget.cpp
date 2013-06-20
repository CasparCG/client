#include "InspectorSaturationWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/RundownItemSelectedEvent.h"
#include "Models/TweenModel.h"

#include <QtGui/QApplication>

InspectorSaturationWidget::InspectorSaturationWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    loadTween();

    qApp->installEventFilter(this);
}

bool InspectorSaturationWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Event::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        this->model = rundownItemSelectedEvent->getLibraryModel();

        blockAllSignals(true);

        if (dynamic_cast<SaturationCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->command = dynamic_cast<SaturationCommand*>(rundownItemSelectedEvent->getCommand());

            this->sliderSaturation->setValue(QString("%1").arg(this->command->getSaturation() * 100).toFloat());
            this->spinBoxSaturation->setValue(QString("%1").arg(this->command->getSaturation() * 100).toFloat());
            this->spinBoxDuration->setValue(this->command->getDuration());
            this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
            this->checkBoxDefer->setChecked(this->command->getDefer());
        }

        blockAllSignals(false);
    }

    return QObject::eventFilter(target, event);
}

void InspectorSaturationWidget::blockAllSignals(bool block)
{
    this->spinBoxSaturation->blockSignals(block);
    this->spinBoxDuration->blockSignals(block);
    this->comboBoxTween->blockSignals(block);
    this->checkBoxDefer->blockSignals(block);
}

void InspectorSaturationWidget::loadTween()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTween->blockSignals(true);

    QList<TweenModel> models = DatabaseManager::getInstance().getTween();
    foreach (TweenModel model, models)
        this->comboBoxTween->addItem(model.getValue());

    this->comboBoxTween->blockSignals(false);
}

void InspectorSaturationWidget::durationChanged(int duration)
{
    this->command->setDuration(duration);
}

void InspectorSaturationWidget::tweenChanged(QString tween)
{
    this->command->setTween(tween);
}

void InspectorSaturationWidget::sliderSaturationChanged(int saturation)
{
    this->command->setSaturation(static_cast<float>(saturation) / 100);

    this->spinBoxSaturation->setValue(saturation);

    EventManager::getInstance().firePreviewEvent();
}

void InspectorSaturationWidget::spinBoxSaturationChanged(int saturation)
{
    this->sliderSaturation->setValue(saturation);
}

void InspectorSaturationWidget::resetSaturation(QString saturation)
{
    this->sliderSaturation->setValue(Mixer::DEFAULT_SATURATION * 100);
    this->command->setSaturation(static_cast<float>(this->sliderSaturation->value()) / 100);

    EventManager::getInstance().firePreviewEvent();
}

void InspectorSaturationWidget::resetDuration(QString duration)
{
    this->spinBoxDuration->setValue(Mixer::DEFAULT_DURATION);
    this->command->setDuration(this->spinBoxDuration->value());
}

void InspectorSaturationWidget::resetTween(QString tween)
{
    this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(Mixer::DEFAULT_TWEEN));
    this->command->setTween(this->comboBoxTween->currentText());
}

void InspectorSaturationWidget::deferChanged(int state)
{
    this->command->setDefer((state == Qt::Checked) ? true : false);
}

void InspectorSaturationWidget::resetDefer(QString defer)
{
    this->checkBoxDefer->setChecked(Mixer::DEFAULT_DEFER);
    this->command->setDefer(this->checkBoxDefer->isChecked());
}
