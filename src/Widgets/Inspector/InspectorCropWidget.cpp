#include "InspectorCropWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/PreviewEvent.h"
#include "Models/TweenModel.h"

#include <QtGui/QApplication>

InspectorCropWidget::InspectorCropWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));

    loadTween();
}

void InspectorCropWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<CropCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<CropCommand*>(event.getCommand());

        this->sliderCropLeft->setValue(QString("%1").arg(this->command->getCropLeft() * 100).toFloat());
        this->sliderCropRight->setValue(QString("%1").arg(this->command->getCropRight() * 100).toFloat());
        this->sliderCropTop->setValue(QString("%1").arg(this->command->getCropTop() * 100).toFloat());
        this->sliderCropBottom->setValue(QString("%1").arg(this->command->getCropBottom() * 100).toFloat());
        this->spinBoxCropLeft->setValue(QString("%1").arg(this->command->getCropLeft() * 100).toFloat());
        this->spinBoxCropRight->setValue(QString("%1").arg(this->command->getCropRight() * 100).toFloat());
        this->spinBoxCropTop->setValue(QString("%1").arg(this->command->getCropTop() * 100).toFloat());
        this->spinBoxCropBottom->setValue(QString("%1").arg(this->command->getCropBottom() * 100).toFloat());
        this->spinBoxTransitionDuration->setValue(this->command->getTransitionDuration());
        this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
        this->checkBoxDefer->setChecked(this->command->getDefer());
    }

    blockAllSignals(false);
}

void InspectorCropWidget::blockAllSignals(bool block)
{
    this->sliderCropLeft->blockSignals(block);
    this->sliderCropRight->blockSignals(block);
    this->sliderCropTop->blockSignals(block);
    this->sliderCropBottom->blockSignals(block);
    this->spinBoxCropLeft->blockSignals(block);
    this->spinBoxCropRight->blockSignals(block);
    this->spinBoxCropTop->blockSignals(block);
    this->spinBoxCropBottom->blockSignals(block);
    this->spinBoxTransitionDuration->blockSignals(block);
    this->comboBoxTween->blockSignals(block);
    this->checkBoxDefer->blockSignals(block);
}

void InspectorCropWidget::loadTween()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTween->blockSignals(true);

    QList<TweenModel> models = DatabaseManager::getInstance().getTween();
    foreach (TweenModel model, models)
        this->comboBoxTween->addItem(model.getValue());

    this->comboBoxTween->blockSignals(false);
}

void InspectorCropWidget::sliderCropLeftChanged(int cropLeft)
{
    this->command->setCropLeft(static_cast<float>(cropLeft) / 100);

    this->spinBoxCropLeft->setValue(cropLeft);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorCropWidget::spinBoxCropLeftChanged(int cropLeft)
{
    this->sliderCropLeft->setValue(cropLeft);
}

void InspectorCropWidget::sliderCropRightChanged(int cropRight)
{
    this->command->setCropRight(static_cast<float>(cropRight) / 100);

    this->spinBoxCropRight->setValue(cropRight);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorCropWidget::spinBoxCropRightChanged(int cropRight)
{
    this->sliderCropRight->setValue(cropRight);
}

void InspectorCropWidget::sliderCropTopChanged(int cropTop)
{
    this->command->setCropTop(static_cast<float>(cropTop) / 100);

    this->spinBoxCropTop->setValue(cropTop);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorCropWidget::spinBoxCropTopChanged(int cropTop)
{
    this->sliderCropTop->setValue(cropTop);
}

void InspectorCropWidget::sliderCropBottomChanged(int cropBottom)
{
    this->command->setCropBottom(static_cast<float>(cropBottom) / 100);

    this->spinBoxCropBottom->setValue(cropBottom);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorCropWidget::spinBoxCropBottomChanged(int cropBottom)
{
    this->sliderCropBottom->setValue(cropBottom);
}

void InspectorCropWidget::transitionDurationChanged(int transitionDuration)
{
    this->command->setTransitionDuration(transitionDuration);
}

void InspectorCropWidget::tweenChanged(QString tween)
{
    this->command->setTween(tween);
}

void InspectorCropWidget::deferChanged(int state)
{
    this->command->setDefer((state == Qt::Checked) ? true : false);
}
