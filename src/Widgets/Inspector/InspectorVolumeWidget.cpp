#include "InspectorVolumeWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/PreviewEvent.h"
#include "Models/TweenModel.h"

#include <QtCore/QDebug>

#include <QtGui/QApplication>

InspectorVolumeWidget::InspectorVolumeWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));

    loadTween();
}

void InspectorVolumeWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<VolumeCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<VolumeCommand*>(event.getCommand());

        this->sliderVolume->setValue(QString("%1").arg(this->command->getVolume() * 100).toFloat());
        this->spinBoxVolume->setValue(QString("%1").arg(this->command->getVolume() * 100).toFloat());
        this->spinBoxTransitionDuration->setValue(this->command->getTransitionDuration());
        this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
        this->checkBoxDefer->setChecked(this->command->getDefer());
    }

    blockAllSignals(false);
}

void InspectorVolumeWidget::blockAllSignals(bool block)
{
    this->sliderVolume->blockSignals(block);
    this->spinBoxVolume->blockSignals(block);
    this->spinBoxTransitionDuration->blockSignals(block);
    this->comboBoxTween->blockSignals(block);
    this->checkBoxDefer->blockSignals(block);
}

void InspectorVolumeWidget::loadTween()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTween->blockSignals(true);

    QList<TweenModel> models = DatabaseManager::getInstance().getTween();
    foreach (TweenModel model, models)
        this->comboBoxTween->addItem(model.getValue());

    this->comboBoxTween->blockSignals(false);
}

void InspectorVolumeWidget::transitionDurationChanged(int transitionDuration)
{
    this->command->setTransitionDuration(transitionDuration);
}

void InspectorVolumeWidget::tweenChanged(QString tween)
{
    this->command->setTween(tween);
}

void InspectorVolumeWidget::sliderVolumeChanged(int volume)
{
    this->command->setVolume(static_cast<float>(volume) / 100);

    this->spinBoxVolume->blockSignals(true);
    this->spinBoxVolume->setValue(volume);
    this->spinBoxVolume->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorVolumeWidget::spinBoxVolumeChanged(int volume)
{
    this->command->setVolume(static_cast<float>(volume) / 100);

    this->sliderVolume->blockSignals(true);
    this->sliderVolume->setValue(volume);
    this->sliderVolume->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorVolumeWidget::deferChanged(int state)
{
    this->command->setDefer((state == Qt::Checked) ? true : false);
}
