#include "InspectorCropWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/RundownItemSelectedEvent.h"
#include "Models/TweenModel.h"

#include <QtGui/QApplication>

InspectorCropWidget::InspectorCropWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    loadTween();

    qApp->installEventFilter(this);
}

bool InspectorCropWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Event::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        this->model = rundownItemSelectedEvent->getLibraryModel();

        blockAllSignals(true);

        if (dynamic_cast<CropCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->command = dynamic_cast<CropCommand*>(rundownItemSelectedEvent->getCommand());

            this->sliderCropLeft->setValue(QString("%1").arg(this->command->getCropLeft() * 100).toFloat());
            this->sliderCropRight->setValue(QString("%1").arg(this->command->getCropRight() * 100).toFloat());
            this->sliderCropTop->setValue(QString("%1").arg(this->command->getCropTop() * 100).toFloat());
            this->sliderCropBottom->setValue(QString("%1").arg(this->command->getCropBottom() * 100).toFloat());
            this->spinBoxCropLeft->setValue(QString("%1").arg(this->command->getCropLeft() * 100).toFloat());
            this->spinBoxCropRight->setValue(QString("%1").arg(this->command->getCropRight() * 100).toFloat());
            this->spinBoxCropTop->setValue(QString("%1").arg(this->command->getCropTop() * 100).toFloat());
            this->spinBoxCropBottom->setValue(QString("%1").arg(this->command->getCropBottom() * 100).toFloat());
            this->spinBoxDuration->setValue(this->command->getDuration());
            this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
            this->checkBoxDefer->setChecked(this->command->getDefer());
        }

        blockAllSignals(false);
    }

    return QObject::eventFilter(target, event);
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
    this->spinBoxDuration->blockSignals(block);
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

    EventManager::getInstance().firePreviewEvent();
}

void InspectorCropWidget::spinBoxCropLeftChanged(int cropLeft)
{
    this->sliderCropLeft->setValue(cropLeft);
}

void InspectorCropWidget::sliderCropRightChanged(int cropRight)
{
    this->command->setCropRight(static_cast<float>(cropRight) / 100);

    this->spinBoxCropRight->setValue(cropRight);

    EventManager::getInstance().firePreviewEvent();
}

void InspectorCropWidget::spinBoxCropRightChanged(int cropRight)
{
    this->sliderCropRight->setValue(cropRight);
}

void InspectorCropWidget::sliderCropTopChanged(int cropTop)
{
    this->command->setCropTop(static_cast<float>(cropTop) / 100);

    this->spinBoxCropTop->setValue(cropTop);

    EventManager::getInstance().firePreviewEvent();
}

void InspectorCropWidget::spinBoxCropTopChanged(int cropTop)
{
    this->sliderCropTop->setValue(cropTop);
}

void InspectorCropWidget::sliderCropBottomChanged(int cropBottom)
{
    this->command->setCropBottom(static_cast<float>(cropBottom) / 100);

    this->spinBoxCropBottom->setValue(cropBottom);

    EventManager::getInstance().firePreviewEvent();
}

void InspectorCropWidget::spinBoxCropBottomChanged(int cropBottom)
{
    this->sliderCropBottom->setValue(cropBottom);
}

void InspectorCropWidget::durationChanged(int duration)
{
    this->command->setDuration(duration);
}

void InspectorCropWidget::tweenChanged(QString tween)
{
    this->command->setTween(tween);
}

void InspectorCropWidget::resetCropLeft(QString value)
{
    this->sliderCropLeft->setValue(Mixer::DEFAULT_CROP_LEFT);
    this->command->setCropLeft(static_cast<float>(this->sliderCropLeft->value()) / 100);

    EventManager::getInstance().firePreviewEvent();
}

void InspectorCropWidget::resetCropRight(QString value)
{
    this->sliderCropRight->setValue(Mixer::DEFAULT_CROP_RIGHT);
    this->command->setCropRight(static_cast<float>(this->sliderCropRight->value()) / 100);

    EventManager::getInstance().firePreviewEvent();
}

void InspectorCropWidget::resetCropTop(QString value)
{
    this->sliderCropTop->setValue(Mixer::DEFAULT_CROP_TOP * 100);
    this->command->setCropTop(static_cast<float>(this->sliderCropTop->value()) / 100);

    EventManager::getInstance().firePreviewEvent();
}

void InspectorCropWidget::resetCropBottom(QString value)
{
    this->sliderCropBottom->setValue(Mixer::DEFAULT_CROP_BOTTOM * 100);
    this->command->setCropBottom(static_cast<float>(this->sliderCropBottom->value()) / 100);

    EventManager::getInstance().firePreviewEvent();
}

void InspectorCropWidget::resetDuration(QString value)
{
    this->spinBoxDuration->setValue(Mixer::DEFAULT_DURATION);
    this->command->setDuration(this->spinBoxDuration->value());
}

void InspectorCropWidget::resetTween(QString value)
{
    this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(Mixer::DEFAULT_TWEEN));
    this->command->setTween(this->comboBoxTween->currentText());
}

void InspectorCropWidget::deferChanged(int state)
{
    this->command->setDefer((state == Qt::Checked) ? true : false);
}

void InspectorCropWidget::resetDefer(QString defer)
{
    this->checkBoxDefer->setChecked(Mixer::DEFAULT_DEFER);
    this->command->setDefer(this->checkBoxDefer->isChecked());
}
