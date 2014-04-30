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
        this->spinBoxTransitionDuration->setValue(this->command->getTransitionDuration());
        this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
        this->checkBoxDefer->setChecked(this->command->getDefer());
    }

    blockAllSignals(false);
}

void InspectorBrightnessWidget::blockAllSignals(bool block)
{
    this->sliderBrightness->blockSignals(block);
    this->spinBoxBrightness->blockSignals(block);
    this->spinBoxTransitionDuration->blockSignals(block);
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

void InspectorBrightnessWidget::transitionDurationChanged(int transitionDuration)
{
    this->command->setTransitionDuration(transitionDuration);
}

void InspectorBrightnessWidget::tweenChanged(QString tween)
{
    this->command->setTween(tween);
}

void InspectorBrightnessWidget::sliderBrightnessChanged(int brightness)
{
    this->command->setBrightness(static_cast<float>(brightness) / 100);

    this->spinBoxBrightness->blockSignals(true);
    this->spinBoxBrightness->setValue(brightness);
    this->spinBoxBrightness->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorBrightnessWidget::spinBoxBrightnessChanged(int brightness)
{
    this->command->setBrightness(static_cast<float>(brightness) / 100);

    this->sliderBrightness->blockSignals(true);
    this->sliderBrightness->setValue(brightness);
    this->sliderBrightness->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorBrightnessWidget::deferChanged(int state)
{
    this->command->setDefer((state == Qt::Checked) ? true : false);
}
