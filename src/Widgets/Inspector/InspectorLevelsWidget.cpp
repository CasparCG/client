#include "InspectorLevelsWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/PreviewEvent.h"
#include "Models/TweenModel.h"

#include <QtGui/QApplication>

InspectorLevelsWidget::InspectorLevelsWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));

    loadTween();
}

void InspectorLevelsWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<LevelsCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<LevelsCommand*>(event.getCommand());

        this->sliderMinIn->setValue(QString("%1").arg(this->command->getMinIn() * 100).toFloat());
        this->sliderMaxIn->setValue(QString("%1").arg(this->command->getMaxIn() * 100).toFloat());
        this->sliderMinOut->setValue(QString("%1").arg(this->command->getMinOut() * 100).toFloat());
        this->sliderMaxOut->setValue(QString("%1").arg(this->command->getMaxOut() * 100).toFloat());
        this->spinBoxMinIn->setValue(QString("%1").arg(this->command->getMinIn() * 100).toFloat());
        this->spinBoxMaxIn->setValue(QString("%1").arg(this->command->getMaxIn() * 100).toFloat());
        this->spinBoxMinOut->setValue(QString("%1").arg(this->command->getMinOut() * 100).toFloat());
        this->spinBoxMaxOut->setValue(QString("%1").arg(this->command->getMaxOut() * 100).toFloat());
        this->sliderGamma->setValue(QString("%1").arg(this->command->getGamma() * 100).toFloat());
        this->spinBoxGamma->setValue(QString("%1").arg(this->command->getGamma() * 100).toFloat());
        this->spinBoxTransitionDuration->setValue(this->command->getTransitionDuration());
        this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
        this->checkBoxDefer->setChecked(this->command->getDefer());
    }

    blockAllSignals(false);
}

void InspectorLevelsWidget::blockAllSignals(bool block)
{
    this->sliderMinIn->blockSignals(block);
    this->sliderMaxIn->blockSignals(block);
    this->sliderMinOut->blockSignals(block);
    this->sliderMaxOut->blockSignals(block);
    this->sliderGamma->blockSignals(block);
    this->spinBoxMinIn->blockSignals(block);
    this->spinBoxMaxIn->blockSignals(block);
    this->spinBoxMinOut->blockSignals(block);
    this->spinBoxMaxOut->blockSignals(block);
    this->spinBoxGamma->blockSignals(block);
    this->spinBoxTransitionDuration->blockSignals(block);
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

    this->spinBoxMinIn->blockSignals(true);
    this->spinBoxMinIn->setValue(minIn);
    this->spinBoxMinIn->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorLevelsWidget::spinBoxMinInChanged(int minIn)
{
    this->command->setMinIn(static_cast<float>(minIn) / 100);

    this->sliderMinIn->blockSignals(true);
    this->sliderMinIn->setValue(minIn);
    this->sliderMinIn->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorLevelsWidget::sliderMaxInChanged(int maxIn)
{
    this->command->setMaxIn(static_cast<float>(maxIn) / 100);

    this->spinBoxMaxIn->blockSignals(true);
    this->spinBoxMaxIn->setValue(maxIn);
    this->spinBoxMaxIn->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorLevelsWidget::spinBoxMaxInChanged(int maxIn)
{
    this->command->setMaxIn(static_cast<float>(maxIn) / 100);

    this->sliderMaxIn->blockSignals(true);
    this->sliderMaxIn->setValue(maxIn);
    this->sliderMaxIn->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorLevelsWidget::sliderMinOutChanged(int minOut)
{
    this->command->setMinOut(static_cast<float>(minOut) / 100);

    this->spinBoxMinOut->blockSignals(true);
    this->spinBoxMinOut->setValue(minOut);
    this->spinBoxMinOut->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorLevelsWidget::spinBoxMinOutChanged(int minOut)
{
    this->command->setMinOut(static_cast<float>(minOut) / 100);

    this->sliderMinOut->blockSignals(true);
    this->sliderMinOut->setValue(minOut);
    this->sliderMinOut->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorLevelsWidget::sliderMaxOutChanged(int maxOut)
{
    this->command->setMaxOut(static_cast<float>(maxOut) / 100);

    this->spinBoxMaxOut->blockSignals(true);
    this->spinBoxMaxOut->setValue(maxOut);
    this->spinBoxMaxOut->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorLevelsWidget::spinBoxMaxOutChanged(int maxOut)
{
    this->command->setMaxOut(static_cast<float>(maxOut) / 100);

    this->sliderMaxOut->blockSignals(true);
    this->sliderMaxOut->setValue(maxOut);
    this->sliderMaxOut->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorLevelsWidget::sliderGammaChanged(int gamma)
{
    this->command->setGamma(static_cast<float>(gamma) / 100);

    this->spinBoxGamma->blockSignals(true);
    this->spinBoxGamma->setValue(gamma);
    this->spinBoxGamma->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorLevelsWidget::spinBoxGammaChanged(int gamma)
{
    this->command->setGamma(static_cast<float>(gamma) / 100);

    this->sliderGamma->blockSignals(true);
    this->sliderGamma->setValue(gamma);
    this->sliderGamma->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorLevelsWidget::transitionDurationChanged(int transitionDuration)
{
    this->command->setTransitionDuration(transitionDuration);
}

void InspectorLevelsWidget::tweenChanged(QString tween)
{
    this->command->setTween(tween);
}

void InspectorLevelsWidget::deferChanged(int state)
{
    this->command->setDefer((state == Qt::Checked) ? true : false);
}
