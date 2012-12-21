#include "InspectorCropWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "Events/RundownItemPreviewEvent.h"
#include "Events/RundownItemSelectedEvent.h"
#include "Models/TweenModel.h"

#include <QtGui/QApplication>

InspectorCropWidget::InspectorCropWidget(QWidget* parent)
    : QWidget(parent), preview(false), model(NULL), command(NULL)
{
    setupUi(this);

    loadTween();

    qApp->installEventFilter(this);
}

bool InspectorCropWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        if (dynamic_cast<CropCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->preview = false;

            this->model = rundownItemSelectedEvent->getLibraryModel();
            this->command = dynamic_cast<CropCommand*>(rundownItemSelectedEvent->getCommand());

            // This will also set the slider value.
            // TODO: Why QString -> float, see InspectorVolumeWidget.cpp
            this->spinBoxCropLeft->setValue(QString("%1").arg(this->command->getCropLeft() * 100).toFloat());
            this->spinBoxCropRight->setValue(QString("%1").arg(this->command->getCropRight() * 100).toFloat());
            this->spinBoxCropTop->setValue(QString("%1").arg(this->command->getCropTop() * 100).toFloat());
            this->spinBoxCropBottom->setValue(QString("%1").arg(this->command->getCropBottom() * 100).toFloat());

            this->spinBoxDuration->setValue(this->command->getDuration());
            this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
            this->checkBoxDefer->setChecked(this->command->getDefer());

            this->preview = true;
        }
    }

    return QObject::eventFilter(target, event);
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

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
}

void InspectorCropWidget::spinBoxCropLeftChanged(int cropLeft)
{
    this->sliderCropLeft->setValue(cropLeft);
}

void InspectorCropWidget::sliderCropRightChanged(int cropRight)
{
    this->command->setCropRight(static_cast<float>(cropRight) / 100);

    this->spinBoxCropRight->setValue(cropRight);

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
}

void InspectorCropWidget::spinBoxCropRightChanged(int cropRight)
{
    this->sliderCropRight->setValue(cropRight);
}

void InspectorCropWidget::sliderCropTopChanged(int cropTop)
{
    this->command->setCropTop(static_cast<float>(cropTop) / 100);

    this->spinBoxCropTop->setValue(cropTop);

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
}

void InspectorCropWidget::spinBoxCropTopChanged(int cropTop)
{
    this->sliderCropTop->setValue(cropTop);
}

void InspectorCropWidget::sliderCropBottomChanged(int cropBottom)
{
    this->command->setCropBottom(static_cast<float>(cropBottom) / 100);

    this->spinBoxCropBottom->setValue(cropBottom);

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
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

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
}

void InspectorCropWidget::resetCropRight(QString value)
{
    this->sliderCropRight->setValue(Mixer::DEFAULT_CROP_RIGHT);
    this->command->setCropRight(static_cast<float>(this->sliderCropRight->value()) / 100);

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
}

void InspectorCropWidget::resetCropTop(QString value)
{
    this->sliderCropTop->setValue(Mixer::DEFAULT_CROP_TOP * 100);
    this->command->setCropTop(static_cast<float>(this->sliderCropTop->value()) / 100);

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
}

void InspectorCropWidget::resetCropBottom(QString value)
{
    this->sliderCropBottom->setValue(Mixer::DEFAULT_CROP_BOTTOM * 100);
    this->command->setCropBottom(static_cast<float>(this->sliderCropBottom->value()) / 100);

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
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
    this->checkBoxDefer->setChecked(false);
    this->command->setDefer(this->checkBoxDefer->isChecked());
}
