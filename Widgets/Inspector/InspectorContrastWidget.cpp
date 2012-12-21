#include "InspectorContrastWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "Events/RundownItemPreviewEvent.h"
#include "Events/RundownItemSelectedEvent.h"
#include "Models/TweenModel.h"

#include <QtGui/QApplication>

InspectorContrastWidget::InspectorContrastWidget(QWidget* parent)
    : QWidget(parent), preview(false), model(NULL), command(NULL)
{
    setupUi(this);

    loadTween();

    qApp->installEventFilter(this);
}

bool InspectorContrastWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        if (dynamic_cast<ContrastCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->preview = false;

            this->model = rundownItemSelectedEvent->getLibraryModel();
            this->command = dynamic_cast<ContrastCommand*>(rundownItemSelectedEvent->getCommand());

            // This will also set the slider value.
            // TODO: Why QString -> float, see InspectorVolumeWidget.cpp
            this->spinBoxContrast->setValue(QString("%1").arg(this->command->getContrast() * 100).toFloat());

            this->spinBoxDuration->setValue(this->command->getDuration());
            this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
            this->checkBoxDefer->setChecked(this->command->getDefer());

            this->preview = true;
        }
    }

    return QObject::eventFilter(target, event);
}

void InspectorContrastWidget::loadTween()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTween->blockSignals(true);

    QList<TweenModel> models = DatabaseManager::getInstance().getTween();
    foreach (TweenModel model, models)
        this->comboBoxTween->addItem(model.getValue());

    this->comboBoxTween->blockSignals(false);
}

void InspectorContrastWidget::durationChanged(int duration)
{
    this->command->setDuration(duration);
}

void InspectorContrastWidget::tweenChanged(QString tween)
{
    this->command->setTween(tween);
}

void InspectorContrastWidget::sliderContrastChanged(int contrast)
{
    this->command->setContrast(static_cast<float>(contrast) / 100);

    this->spinBoxContrast->setValue(contrast);

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
}

void InspectorContrastWidget::spinBoxContrastChanged(int contrast)
{
    this->sliderContrast->setValue(contrast);
}

void InspectorContrastWidget::resetContrast(QString contrast)
{
    this->sliderContrast->setValue(Mixer::DEFAULT_CONTRAST * 100);
    this->command->setContrast(static_cast<float>(this->sliderContrast->value()) / 100);

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
}

void InspectorContrastWidget::resetDuration(QString duration)
{
    this->spinBoxDuration->setValue(Mixer::DEFAULT_DURATION);
    this->command->setDuration(this->spinBoxDuration->value());
}

void InspectorContrastWidget::resetTween(QString tween)
{
    this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(Mixer::DEFAULT_TWEEN));
    this->command->setTween(this->comboBoxTween->currentText());
}

void InspectorContrastWidget::deferChanged(int state)
{
    this->command->setDefer((state == Qt::Checked) ? true : false);
}

void InspectorContrastWidget::resetDefer(QString defer)
{
    this->checkBoxDefer->setChecked(false);
    this->command->setDefer(this->checkBoxDefer->isChecked());
}
