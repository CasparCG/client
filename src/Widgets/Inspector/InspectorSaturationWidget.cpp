#include "InspectorSaturationWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/PreviewEvent.h"
#include "Models/TweenModel.h"

#include <QtGui/QApplication>

InspectorSaturationWidget::InspectorSaturationWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));

    loadTween();
}

void InspectorSaturationWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<SaturationCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<SaturationCommand*>(event.getCommand());

        this->sliderSaturation->setValue(QString("%1").arg(this->command->getSaturation() * 100).toFloat());
        this->spinBoxSaturation->setValue(QString("%1").arg(this->command->getSaturation() * 100).toFloat());
        this->spinBoxTransitionDuration->setValue(this->command->getTransitionDuration());
        this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
        this->checkBoxDefer->setChecked(this->command->getDefer());
    }

    blockAllSignals(false);
}

void InspectorSaturationWidget::blockAllSignals(bool block)
{
    this->sliderSaturation->blockSignals(block);
    this->spinBoxSaturation->blockSignals(block);
    this->spinBoxTransitionDuration->blockSignals(block);
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

void InspectorSaturationWidget::transitionDurationChanged(int transitionDuration)
{
    this->command->setTransitionDuration(transitionDuration);
}

void InspectorSaturationWidget::tweenChanged(QString tween)
{
    this->command->setTween(tween);
}

void InspectorSaturationWidget::sliderSaturationChanged(int saturation)
{
    this->command->setSaturation(static_cast<float>(saturation) / 100);

    this->spinBoxSaturation->blockSignals(true);
    this->spinBoxSaturation->setValue(saturation);
    this->spinBoxSaturation->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorSaturationWidget::spinBoxSaturationChanged(int saturation)
{
    this->command->setSaturation(static_cast<float>(saturation) / 100);

    this->sliderSaturation->blockSignals(true);
    this->sliderSaturation->setValue(saturation);
    this->sliderSaturation->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorSaturationWidget::deferChanged(int state)
{
    this->command->setDefer((state == Qt::Checked) ? true : false);
}
