#include "InspectorSolidColorWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "Events/RundownItemSelectedEvent.h"
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

    loadDirection();
    loadTransition();
    loadTween();

    qApp->installEventFilter(this);
}

bool InspectorSolidColorWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        this->model = rundownItemSelectedEvent->getLibraryModel();

        blockAllSignals(true);

        if (dynamic_cast<SolidColorCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->command = dynamic_cast<SolidColorCommand*>(rundownItemSelectedEvent->getCommand());

            this->comboBoxTransition->setCurrentIndex(this->comboBoxTransition->findText(this->command->getTransition()));
            this->spinBoxDuration->setValue(this->command->getDuration());
            this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
            this->comboBoxDirection->setCurrentIndex(this->comboBoxDirection->findText(this->command->getDirection()));
        }

        blockAllSignals(false);
    }

    return QObject::eventFilter(target, event);
}

void InspectorSolidColorWidget::blockAllSignals(bool block)
{
    this->comboBoxTransition->blockSignals(block);
    this->spinBoxDuration->blockSignals(block);
    this->comboBoxTween->blockSignals(block);
    this->comboBoxDirection->blockSignals(block);
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

void InspectorSolidColorWidget::durationChanged(int duration)
{
    this->command->setDuration(duration);
}

void InspectorSolidColorWidget::directionChanged(QString direction)
{
    this->command->setDirection(direction);
}

void InspectorSolidColorWidget::tweenChanged(QString tween)
{
    this->command->setTween(tween);
}

void InspectorSolidColorWidget::useAutoChanged(int state)
{
    this->command->setUseAuto((state == Qt::Checked) ? true : false);
}

void InspectorSolidColorWidget::resetTransition(QString transition)
{
    this->comboBoxTransition->setCurrentIndex(this->comboBoxTransition->findText(Mixer::DEFAULT_TRANSITION));
    this->command->setTransition(this->comboBoxTransition->currentText());
}

void InspectorSolidColorWidget::resetDuration(QString duration)
{
    this->spinBoxDuration->setValue(Mixer::DEFAULT_DURATION);
    this->command->setDuration(this->spinBoxDuration->value());
}

void InspectorSolidColorWidget::resetDirection(QString direction)
{
    this->comboBoxDirection->setCurrentIndex(this->comboBoxDirection->findText(Mixer::DEFAULT_DIRECTION));
    this->command->setDirection(this->comboBoxDirection->currentText());
}

void InspectorSolidColorWidget::resetTween(QString tween)
{
    this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(Mixer::DEFAULT_TWEEN));
    this->command->setTween(this->comboBoxTween->currentText());
}

void InspectorSolidColorWidget::resetUseAuto(QString useAuto)
{
    this->checkBoxUseAuto->setChecked(false);
    this->command->setUseAuto(this->checkBoxUseAuto->isChecked());
}
