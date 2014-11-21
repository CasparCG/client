#include "InspectorHtmlWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Models/DirectionModel.h"
#include "Models/TransitionModel.h"
#include "Models/TweenModel.h"

InspectorHtmlWidget::InspectorHtmlWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));

    loadDirection();
    loadTransition();
    loadTween();
}

void InspectorHtmlWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<HtmlCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<HtmlCommand*>(event.getCommand());

        this->lineEditUrl->setText(this->command->getUrl());
        this->comboBoxTransition->setCurrentIndex(this->comboBoxTransition->findText(this->command->getTransition()));
        this->spinBoxTransitionDuration->setValue(this->command->getTransitionDuration());
        this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
        this->comboBoxDirection->setCurrentIndex(this->comboBoxDirection->findText(this->command->getDirection()));
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
        this->checkBoxUseAuto->setChecked(this->command->getUseAuto());
    }

    checkEmptyUrl();

    blockAllSignals(false);
}

void InspectorHtmlWidget::blockAllSignals(bool block)
{
    this->lineEditUrl->blockSignals(block);
    this->comboBoxTransition->blockSignals(block);
    this->spinBoxTransitionDuration->blockSignals(block);
    this->comboBoxTween->blockSignals(block);
    this->comboBoxDirection->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
    this->checkBoxUseAuto->blockSignals(block);
}

void InspectorHtmlWidget::loadDirection()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxDirection->blockSignals(true);

    QList<DirectionModel> models = DatabaseManager::getInstance().getDirection();
    foreach (DirectionModel model, models)
        this->comboBoxDirection->addItem(model.getValue());

    this->comboBoxDirection->blockSignals(false);
}

void InspectorHtmlWidget::loadTransition()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTransition->blockSignals(true);

    QList<TransitionModel> models = DatabaseManager::getInstance().getTransition();
    foreach (TransitionModel model, models)
        this->comboBoxTransition->addItem(model.getValue());

    this->comboBoxTransition->blockSignals(false);
}

void InspectorHtmlWidget::loadTween()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTween->blockSignals(true);

    QList<TweenModel> models = DatabaseManager::getInstance().getTween();
    foreach (TweenModel model, models)
        this->comboBoxTween->addItem(model.getValue());

    this->comboBoxTween->blockSignals(false);
}

void InspectorHtmlWidget::checkEmptyUrl()
{
    if (this->lineEditUrl->text().isEmpty())
        this->lineEditUrl->setStyleSheet("border-color: firebrick;");
    else
        this->lineEditUrl->setStyleSheet("");
}

void InspectorHtmlWidget::transitionChanged(QString transition)
{
    this->command->setTransition(transition);
}

void InspectorHtmlWidget::transitionDurationChanged(int transitionDuration)
{
    this->command->setTransitionDuration(transitionDuration);
}

void InspectorHtmlWidget::directionChanged(QString direction)
{
    this->command->setDirection(direction);
}

void InspectorHtmlWidget::tweenChanged(QString tween)
{
    this->command->setTween(tween);
}

void InspectorHtmlWidget::useAutoChanged(int state)
{
    this->command->setUseAuto((state == Qt::Checked) ? true : false);
}

void InspectorHtmlWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}

void InspectorHtmlWidget::urlChanged(QString url)
{
    this->command->setUrl(url);

    checkEmptyUrl();
}
