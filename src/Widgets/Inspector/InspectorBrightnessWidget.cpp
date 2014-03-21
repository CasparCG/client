#include "InspectorBrightnessWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/PreviewEvent.h"
#include "Models/TweenModel.h"

#include <QtGui/QApplication>

InspectorBrightnessWidget::InspectorBrightnessWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));

    loadTween();
}

void InspectorBrightnessWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<BrightnessCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<BrightnessCommand*>(event.getCommand());

        this->sliderBrightness->setValue(QString("%1").arg(this->command->getBrightness() * 100).toFloat());
        this->spinBoxBrightness->setValue(QString("%1").arg(this->command->getBrightness() * 100).toFloat());
        this->spinBoxDuration->setValue(this->command->getDuration());
        this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
        this->checkBoxDefer->setChecked(this->command->getDefer());
    }

    blockAllSignals(false);
}

void InspectorBrightnessWidget::blockAllSignals(bool block)
{
    this->sliderBrightness->blockSignals(block);
    this->spinBoxBrightness->blockSignals(block);
    this->spinBoxDuration->blockSignals(block);
    this->comboBoxTween->blockSignals(block);
    this->checkBoxDefer->blockSignals(block);
}

void InspectorBrightnessWidget::loadTween()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTween->blockSignals(true);

    QList<TweenModel> models = DatabaseManager::getInstance().getTween();
    foreach (TweenModel model, models)
        this->comboBoxTween->addItem(model.getValue());

    this->comboBoxTween->blockSignals(false);
}

void InspectorBrightnessWidget::durationChanged(int duration)
{
    this->command->setDuration(duration);
}

void InspectorBrightnessWidget::tweenChanged(QString tween)
{
    this->command->setTween(tween);
}

void InspectorBrightnessWidget::sliderBrightnessChanged(int brightness)
{
    this->command->setBrightness(static_cast<float>(brightness) / 100);

    this->spinBoxBrightness->setValue(brightness);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorBrightnessWidget::spinBoxBrightnessChanged(int brightness)
{
    this->sliderBrightness->setValue(brightness);
}

void InspectorBrightnessWidget::resetBrightness(QString brightness)
{
    this->sliderBrightness->setValue(Mixer::DEFAULT_BRIGHTNESS * 100);
    this->command->setBrightness(static_cast<float>(this->sliderBrightness->value()) / 100);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorBrightnessWidget::resetDuration(QString duration)
{
    this->spinBoxDuration->setValue(Mixer::DEFAULT_DURATION);
    this->command->setDuration(this->spinBoxDuration->value());
}

void InspectorBrightnessWidget::resetTween(QString tween)
{
    this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(Mixer::DEFAULT_TWEEN));
    this->command->setTween(this->comboBoxTween->currentText());
}

void InspectorBrightnessWidget::deferChanged(int state)
{
    this->command->setDefer((state == Qt::Checked) ? true : false);
}

void InspectorBrightnessWidget::resetDefer(QString defer)
{
    this->checkBoxDefer->setChecked(Mixer::DEFAULT_DEFER);
    this->command->setDefer(this->checkBoxDefer->isChecked());
}
