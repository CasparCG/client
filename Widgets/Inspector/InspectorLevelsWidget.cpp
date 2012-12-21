#include "InspectorLevelsWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "Events/RundownItemPreviewEvent.h"
#include "Events/RundownItemSelectedEvent.h"
#include "Models/TweenModel.h"

#include <QtGui/QApplication>

InspectorLevelsWidget::InspectorLevelsWidget(QWidget* parent)
    : QWidget(parent), preview(false), model(NULL), command(NULL)
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
        if (dynamic_cast<LevelsCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->preview = false;

            this->model = rundownItemSelectedEvent->getLibraryModel();
            this->command = dynamic_cast<LevelsCommand*>(rundownItemSelectedEvent->getCommand());

            // This will also set the slider value.
            // TODO: Why QString -> float, see InspectorVolumeWidget.cpp
            this->spinBoxMinIn->setValue(QString("%1").arg(this->command->getMinIn() * 100).toFloat());
            this->spinBoxMaxIn->setValue(QString("%1").arg(this->command->getMaxIn() * 100).toFloat());
            this->spinBoxMinOut->setValue(QString("%1").arg(this->command->getMinOut() * 100).toFloat());
            this->spinBoxMaxOut->setValue(QString("%1").arg(this->command->getMaxOut() * 100).toFloat());
            this->spinBoxGamma->setValue(QString("%1").arg(this->command->getGamma() * 100).toFloat());

            this->spinBoxDuration->setValue(this->command->getDuration());
            this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
            this->checkBoxDefer->setChecked(this->command->getDefer());

            this->preview = true;
        }
    }

    return QObject::eventFilter(target, event);
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

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
}

void InspectorLevelsWidget::spinBoxMinInChanged(int minIn)
{
    this->sliderMinIn->setValue(minIn);
}

void InspectorLevelsWidget::sliderMaxInChanged(int maxIn)
{
    this->command->setMaxIn(static_cast<float>(maxIn) / 100);

    this->spinBoxMaxIn->setValue(maxIn);

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
}

void InspectorLevelsWidget::spinBoxMaxInChanged(int maxIn)
{
    this->sliderMaxIn->setValue(maxIn);
}

void InspectorLevelsWidget::sliderMinOutChanged(int minOut)
{
    this->command->setMinOut(static_cast<float>(minOut) / 100);

    this->spinBoxMinOut->setValue(minOut);

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
}

void InspectorLevelsWidget::spinBoxMinOutChanged(int minOut)
{
    this->sliderMinOut->setValue(minOut);
}

void InspectorLevelsWidget::sliderMaxOutChanged(int maxOut)
{
    this->command->setMaxOut(static_cast<float>(maxOut) / 100);

    this->spinBoxMaxOut->setValue(maxOut);

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
}

void InspectorLevelsWidget::spinBoxMaxOutChanged(int maxOut)
{
    this->sliderMaxOut->setValue(maxOut);
}

void InspectorLevelsWidget::sliderGammaChanged(int gamma)
{
    this->command->setGamma(static_cast<float>(gamma) / 100);

    this->spinBoxGamma->setValue(gamma);

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
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

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
}

void InspectorLevelsWidget::resetMaxIn(QString maxIn)
{
    this->sliderMaxIn->setValue(Mixer::DEFAULT_LEVELS_MAX_IN * 100);
    this->command->setMaxIn(static_cast<float>(this->sliderMaxIn->value()) / 100);

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
}

void InspectorLevelsWidget::resetMinOut(QString minOut)
{
    this->sliderMinOut->setValue(Mixer::DEFAULT_LEVELS_MIN_OUT);
    this->command->setMinOut(static_cast<float>(this->sliderMinOut->value()) / 100);

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
}

void InspectorLevelsWidget::resetMaxOut(QString maxOut)
{
    this->sliderMaxOut->setValue(Mixer::DEFAULT_LEVELS_MAX_OUT * 100);
    this->command->setMaxOut(static_cast<float>(this->sliderMaxOut->value()) / 100);

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
}

void InspectorLevelsWidget::resetGamma(QString gamma)
{
    this->sliderGamma->setValue(Mixer::DEFAULT_LEVELS_GAMMA * 100);
    this->command->setGamma(static_cast<float>(this->sliderGamma->value()) / 100);

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
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
    this->checkBoxDefer->setChecked(false);
    this->command->setDefer(this->checkBoxDefer->isChecked());
}
