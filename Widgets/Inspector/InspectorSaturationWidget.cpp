#include "InspectorSaturationWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "Events/RundownItemPreviewEvent.h"
#include "Events/RundownItemSelectedEvent.h"
#include "Models/TweenModel.h"

#include <QtGui/QApplication>

InspectorSaturationWidget::InspectorSaturationWidget(QWidget* parent)
    : QWidget(parent), preview(false), model(NULL), command(NULL)
{
    setupUi(this);

    loadTween();

    qApp->installEventFilter(this);
}

bool InspectorSaturationWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        if (dynamic_cast<SaturationCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->preview = false;

            this->model = rundownItemSelectedEvent->getLibraryModel();
            this->command = dynamic_cast<SaturationCommand*>(rundownItemSelectedEvent->getCommand());

            // This will also set the slider value.
            // TODO: Why QString -> float, see InspectorVolumeWidget.cpp
            this->spinBoxSaturation->setValue(QString("%1").arg(this->command->getSaturation() * 100).toFloat());

            this->spinBoxDuration->setValue(this->command->getDuration());
            this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
            this->checkBoxDefer->setChecked(this->command->getDefer());

            this->preview = true;
        }
    }

    return QObject::eventFilter(target, event);
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

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
}

void InspectorSaturationWidget::spinBoxSaturationChanged(int saturation)
{
    this->sliderSaturation->setValue(saturation);
}

void InspectorSaturationWidget::resetSaturation(QString saturation)
{
    this->sliderSaturation->setValue(Mixer::DEFAULT_SATURATION * 100);
    this->command->setSaturation(static_cast<float>(this->sliderSaturation->value()) / 100);

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
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
    this->checkBoxDefer->setChecked(false);
    this->command->setDefer(this->checkBoxDefer->isChecked());
}
