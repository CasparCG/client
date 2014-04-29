#include "InspectorSolidColorWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Models/DirectionModel.h"
#include "Models/TransitionModel.h"
#include "Models/TweenModel.h"

#include <QtCore/QDebug>

#include <QtGui/QColorDialog>

InspectorSolidColorWidget::InspectorSolidColorWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));

    loadDirection();
    loadTransition();
    loadTween();
}

void InspectorSolidColorWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<SolidColorCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<SolidColorCommand*>(event.getCommand());

        this->lineEditColor->setText(this->command->getColor());
        this->comboBoxTransition->setCurrentIndex(this->comboBoxTransition->findText(this->command->getTransition()));
        this->spinBoxTransitionDuration->setValue(this->command->getTransitionDuration());
        this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
        this->comboBoxDirection->setCurrentIndex(this->comboBoxDirection->findText(this->command->getDirection()));
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
    }

    blockAllSignals(false);
}

void InspectorSolidColorWidget::blockAllSignals(bool block)
{
    this->lineEditColor->blockSignals(block);
    this->comboBoxTransition->blockSignals(block);
    this->spinBoxTransitionDuration->blockSignals(block);
    this->comboBoxTween->blockSignals(block);
    this->comboBoxDirection->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
}

void InspectorSolidColorWidget::loadDirection()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxDirection->blockSignals(true);

    QList<DirectionModel> models = DatabaseManager::getInstance().getDirection();
    foreach (DirectionModel model, models)
        this->comboBoxDirection->addItem(model.getValue());

    this->comboBoxDirection->blockSignals(false);
}

void InspectorSolidColorWidget::loadTransition()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTransition->blockSignals(true);

    QList<TransitionModel> models = DatabaseManager::getInstance().getTransition();
    foreach (TransitionModel model, models)
        this->comboBoxTransition->addItem(model.getValue());

    this->comboBoxTransition->blockSignals(false);
}

void InspectorSolidColorWidget::loadTween()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTween->blockSignals(true);

    QList<TweenModel> models = DatabaseManager::getInstance().getTween();
    foreach (TweenModel model, models)
        this->comboBoxTween->addItem(model.getValue());

    this->comboBoxTween->blockSignals(false);
}

void InspectorSolidColorWidget::colorDialogClicked()
{
    QColorDialog dialog;
    dialog.setOption(QColorDialog::ShowAlphaChannel);

    if (!this->lineEditColor->text().isEmpty())
    {
        QString hexColor = this->lineEditColor->text().remove('#');
        if (hexColor.length() == 8)
        {
            int alpha = hexColor.mid(0, 2).toInt(0, 16);
            int red = hexColor.mid(2, 2).toInt(0, 16);
            int green = hexColor.mid(4, 2).toInt(0, 16);
            int blue = hexColor.mid(6, 2).toInt(0, 16);

            QColor color;
            color.setAlpha(alpha);
            color.setRed(red);
            color.setGreen(green);
            color.setBlue(blue);

            dialog.setCurrentColor(color);
        }
    }

    if (dialog.exec() == QDialog::Accepted)
    {
        QString color;
        color.sprintf("#%02x%02x%02x%02x", dialog.selectedColor().alpha(), dialog.selectedColor().red(),
                      dialog.selectedColor().green(), dialog.selectedColor().blue());

        this->lineEditColor->setText(color);
    }
}

void InspectorSolidColorWidget::transitionChanged(QString transition)
{
    this->command->setTransition(transition);
}

void InspectorSolidColorWidget::transitionDurationChanged(int transitionDuration)
{
    this->command->setTransitionDuration(transitionDuration);
}

void InspectorSolidColorWidget::directionChanged(QString direction)
{
    this->command->setDirection(direction);
}

void InspectorSolidColorWidget::tweenChanged(QString tween)
{
    this->command->setTween(tween);
}

void InspectorSolidColorWidget::colorChanged(QString color)
{
    this->command->setColor(color);
}

void InspectorSolidColorWidget::useAutoChanged(int state)
{
    this->command->setUseAuto((state == Qt::Checked) ? true : false);
}

void InspectorSolidColorWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}
