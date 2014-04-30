#include "InspectorOpacityWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/PreviewEvent.h"
#include "Models/TweenModel.h"

#include <QtGui/QApplication>

InspectorOpacityWidget::InspectorOpacityWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));

    loadTween();
}

void InspectorOpacityWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<OpacityCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<OpacityCommand*>(event.getCommand());

        this->sliderOpacity->setValue(QString("%1").arg(this->command->getOpacity() * 100).toFloat());
        this->spinBoxOpacity->setValue(QString("%1").arg(this->command->getOpacity() * 100).toFloat());
        this->spinBoxTransitionDuration->setValue(this->command->getTransitionDuration());
        this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
        this->checkBoxDefer->setChecked(this->command->getDefer());
    }

    blockAllSignals(false);
}

void InspectorOpacityWidget::blockAllSignals(bool block)
{
    this->sliderOpacity->blockSignals(block);
    this->spinBoxOpacity->blockSignals(block);
    this->spinBoxTransitionDuration->blockSignals(block);
    this->comboBoxTween->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
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

void InspectorOpacityWidget::transitionDurationChanged(int transitionDuration)
{
    this->command->setTransitionDuration(transitionDuration);
}

void InspectorOpacityWidget::tweenChanged(QString tween)
{
    this->command->setTween(tween);
}

void InspectorOpacityWidget::sliderOpacityChanged(int opacity)
{
    this->command->setOpacity(static_cast<float>(opacity) / 100);

    this->spinBoxOpacity->blockSignals(true);
    this->spinBoxOpacity->setValue(opacity);
    this->spinBoxOpacity->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorOpacityWidget::spinBoxOpacityChanged(int opacity)
{
    this->command->setOpacity(static_cast<float>(opacity) / 100);

    this->sliderOpacity->blockSignals(true);
    this->sliderOpacity->setValue(opacity);
    this->sliderOpacity->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorOpacityWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}

void InspectorOpacityWidget::deferChanged(int state)
{
    this->command->setDefer((state == Qt::Checked) ? true : false);
}
