#include "InspectorLevelsWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/RundownItemSelectedEvent.h"
#include "Models/TweenModel.h"

#include <QtGui/QApplication>

InspectorLevelsWidget::InspectorLevelsWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    loadTween();

    qApp->installEventFilter(this);
}

bool InspectorLevelsWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        this->model = rundownItemSelectedEvent->getLibraryModel();

        blockAllSignals(true);

        if (dynamic_cast<LevelsCommand*>(rundownItemSelectedEvent->getCommand()))
        { 
            this->command = dynamic_cast<LevelsCommand*>(rundownItemSelectedEvent->getCommand());

            this->sliderMinIn->setValue(QString("%1").arg(this->command->getMinIn() * 100).toFloat());
            this->sliderMaxIn->setValue(QString("%1").arg(this->command->getMaxIn() * 100).toFloat());
            this->sliderMinOut->setValue(QString("%1").arg(this->command->getMinOut() * 100).toFloat());
            this->sliderMaxOut->setValue(QString("%1").arg(this->command->getMaxOut() * 100).toFloat());
            this->spinBoxMinIn->setValue(QString("%1").arg(this->command->getMinIn() * 100).toFloat());
            this->spinBoxMaxIn->setValue(QString("%1").arg(this->command->getMaxIn() * 100).toFloat());
            this->spinBoxMinOut->setValue(QString("%1").arg(this->command->getMinOut() * 100).toFloat());
            this->spinBoxMaxOut->setValue(QString("%1").arg(this->command->getMaxOut() * 100).toFloat());
            this->spinBoxGamma->setValue(QString("%1").arg(this->command->getGamma() * 100).toFloat());
            this->spinBoxDuration->setValue(this->command->getDuration());
            this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
            this->checkBoxDefer->setChecked(this->command->getDefer());
        }

        blockAllSignals(false);
    }

    return QObject::eventFilter(target, event);
}

void InspectorLevelsWidget::blockAllSignals(bool block)
{
    this->spinBoxMinIn->blockSignals(block);
    this->spinBoxMaxIn->blockSignals(block);
    this->spinBoxMinOut->blockSignals(block);
    this->spinBoxMaxOut->blockSignals(block);
    this->spinBoxGamma->blockSignals(block);
    this->spinBoxDuration->blockSignals(block);
    this->comboBoxTween->blockSignals(block);
    this->checkBoxDefer->blockSignals(block);
}

void InspectorLevelsWidget::loadTween()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTween->blockSignals(true);

    QList<TweenModel> models = DatabaseManager::getInstance().getTween();
    foreach (TweenModel model, models)
        this->comboBoxTween->addItem(model.getValue());

    this->comboBoxTween->blockSignals(false);
}

void InspectorLevelsWidget::sliderMinInChanged(int minIn)
{
    this->command->setMinIn(static_cast<float>(minIn) / 100);

    this->spinBoxMinIn->setValue(minIn);

    EventManager::getInstance().firePreviewEvent();
}

void InspectorLevelsWidget::spinBoxMinInChanged(int minIn)
{
    this->sliderMinIn->setValue(minIn);
}

void InspectorLevelsWidget::sliderMaxInChanged(int maxIn)
{
    this->command->setMaxIn(static_cast<float>(maxIn) / 100);

    this->spinBoxMaxIn->setValue(maxIn);

    EventManager::getInstance().firePreviewEvent();
}

void InspectorLevelsWidget::spinBoxMaxInChanged(int maxIn)
{
    this->sliderMaxIn->setValue(maxIn);
}

void InspectorLevelsWidget::sliderMinOutChanged(int minOut)
{
    this->command->setMinOut(static_cast<float>(minOut) / 100);

    this->spinBoxMinOut->setValue(minOut);

    EventManager::getInstance().firePreviewEvent();
}

void InspectorLevelsWidget::spinBoxMinOutChanged(int minOut)
{
    this->sliderMinOut->setValue(minOut);
}

void InspectorLevelsWidget::sliderMaxOutChanged(int maxOut)
{
    this->command->setMaxOut(static_cast<float>(maxOut) / 100);

    this->spinBoxMaxOut->setValue(maxOut);

    EventManager::getInstance().firePreviewEvent();
}

void InspectorLevelsWidget::spinBoxMaxOutChanged(int maxOut)
{
    this->sliderMaxOut->setValue(maxOut);
}

void InspectorLevelsWidget::sliderGammaChanged(int gamma)
{
    this->command->setGamma(static_cast<float>(gamma) / 100);

    this->spinBoxGamma->setValue(gamma);

    EventManager::getInstance().firePreviewEvent();
}

void InspectorLevelsWidget::spinBoxGammaChanged(int gamma)
{
    this->sliderGamma->setValue(gamma);
}

void InspectorLevelsWidget::durationChanged(int duration)
{
    this->command->setDuration(duration);
}

void InspectorLevelsWidget::tweenChanged(QString tween)
{
    this->command->setTween(tween);
}

void InspectorLevelsWidget::resetMinIn(QString minIn)
{
    this->sliderMinIn->setValue(Mixer::DEFAULT_LEVELS_MIN_IN);
    this->command->setMinIn(static_cast<float>(this->sliderMinIn->value()) / 100);

    EventManager::getInstance().firePreviewEvent();
}

void InspectorLevelsWidget::resetMaxIn(QString maxIn)
{
    this->sliderMaxIn->setValue(Mixer::DEFAULT_LEVELS_MAX_IN * 100);
    this->command->setMaxIn(static_cast<float>(this->sliderMaxIn->value()) / 100);

    EventManager::getInstance().firePreviewEvent();
}

void InspectorLevelsWidget::resetMinOut(QString minOut)
{
    this->sliderMinOut->setValue(Mixer::DEFAULT_LEVELS_MIN_OUT);
    this->command->setMinOut(static_cast<float>(this->sliderMinOut->value()) / 100);

    EventManager::getInstance().firePreviewEvent();
}

void InspectorLevelsWidget::resetMaxOut(QString maxOut)
{
    this->sliderMaxOut->setValue(Mixer::DEFAULT_LEVELS_MAX_OUT * 100);
    this->command->setMaxOut(static_cast<float>(this->sliderMaxOut->value()) / 100);

    EventManager::getInstance().firePreviewEvent();
}

void InspectorLevelsWidget::resetGamma(QString gamma)
{
    this->sliderGamma->setValue(Mixer::DEFAULT_LEVELS_GAMMA * 100);
    this->command->setGamma(static_cast<float>(this->sliderGamma->value()) / 100);

    EventManager::getInstance().firePreviewEvent();
}

void InspectorLevelsWidget::resetDuration(QString duration)
{
    this->spinBoxDuration->setValue(Mixer::DEFAULT_DURATION);
    this->command->setDuration(this->spinBoxDuration->value());
}

void InspectorLevelsWidget::resetTween(QString tween)
{
    this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(Mixer::DEFAULT_TWEEN));
    this->command->setTween(this->comboBoxTween->currentText());
}

void InspectorLevelsWidget::deferChanged(int state)
{
    this->command->setDefer((state == Qt::Checked) ? true : false);
}

void InspectorLevelsWidget::resetDefer(QString defer)
{
    this->checkBoxDefer->setChecked(Mixer::DEFAULT_DEFER);
    this->command->setDefer(this->checkBoxDefer->isChecked());
}
