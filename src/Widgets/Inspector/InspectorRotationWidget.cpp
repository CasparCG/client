#include "InspectorRotationWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/PreviewEvent.h"
#include "Models/TweenModel.h"

#include <QtGui/QApplication>

InspectorRotationWidget::InspectorRotationWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));

    loadTween();
}

void InspectorRotationWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<RotationCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<RotationCommand*>(event.getCommand());

        this->sliderRotation->setValue(QString("%1").arg(this->command->getRotation() * 100).toFloat());
        this->doubleSpinBoxRotation->setValue(this->command->getRotation());
        this->spinBoxTransitionDuration->setValue(this->command->getTransitionDuration());
        this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
        this->checkBoxDefer->setChecked(this->command->getDefer());
    }

    blockAllSignals(false);
}

void InspectorRotationWidget::blockAllSignals(bool block)
{
    this->sliderRotation->blockSignals(block);
    this->doubleSpinBoxRotation->blockSignals(block);
    this->spinBoxTransitionDuration->blockSignals(block);
    this->comboBoxTween->blockSignals(block);
    this->checkBoxDefer->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
}

void InspectorRotationWidget::loadTween()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTween->blockSignals(true);

    QList<TweenModel> models = DatabaseManager::getInstance().getTween();
    foreach (TweenModel model, models)
        this->comboBoxTween->addItem(model.getValue());

    this->comboBoxTween->blockSignals(false);
}

void InspectorRotationWidget::transitionDurationChanged(int transitionDuration)
{
    this->command->setTransitionDuration(transitionDuration);
}

void InspectorRotationWidget::tweenChanged(QString tween)
{
    this->command->setTween(tween);
}

void InspectorRotationWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}

void InspectorRotationWidget::sliderRotationChanged(int rotation)
{
    this->command->setRotation(static_cast<float>(rotation) / 100);

    this->doubleSpinBoxRotation->blockSignals(true);
    this->doubleSpinBoxRotation->setValue(static_cast<float>(rotation) / 100);
    this->doubleSpinBoxRotation->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorRotationWidget::doubleSpinBoxRotationChanged(double rotation)
{
    this->command->setRotation(static_cast<float>(rotation));

    this->sliderRotation->blockSignals(true);
    this->sliderRotation->setValue(rotation * 100);
    this->sliderRotation->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorRotationWidget::deferChanged(int state)
{
    this->command->setDefer((state == Qt::Checked) ? true : false);
}
