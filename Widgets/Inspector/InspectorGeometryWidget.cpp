#include "InspectorGeometryWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "Events/RundownItemPreviewEvent.h"
#include "Events/RundownItemSelectedEvent.h"
#include "Models/TweenModel.h"

#include <QtCore/QDebug>

#include <QtGui/QApplication>

InspectorGeometryWidget::InspectorGeometryWidget(QWidget* parent)
    : QWidget(parent), preview(false), model(NULL), command(NULL), resolutionWidth(0), resolutionHeight(0)
{
    setupUi(this);

    loadTween();

    qApp->installEventFilter(this);
}

bool InspectorGeometryWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        if (dynamic_cast<GeometryCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->preview = false;

            this->model = rundownItemSelectedEvent->getLibraryModel();
            this->command = dynamic_cast<GeometryCommand*>(rundownItemSelectedEvent->getCommand());

            this->resolutionWidth = DatabaseManager::getInstance().getConfigurationByName("ResolutionWidth").getValue().toInt();
            this->resolutionHeight = DatabaseManager::getInstance().getConfigurationByName("ResolutionHeight").getValue().toInt();

            this->sliderPositionX->setMinimum(-this->resolutionWidth);
            this->sliderPositionX->setMaximum(this->resolutionWidth);
            this->sliderPositionY->setMinimum(-this->resolutionHeight);
            this->sliderPositionY->setMaximum(this->resolutionHeight);
            this->spinBoxPositionX->setMinimum(-this->resolutionWidth);
            this->spinBoxPositionX->setMaximum(this->resolutionWidth);
            this->spinBoxPositionY->setMinimum(-this->resolutionHeight);
            this->spinBoxPositionY->setMaximum(this->resolutionHeight);

            this->sliderScaleX->setMinimum(0);
            this->sliderScaleX->setMaximum(this->resolutionWidth * 2);
            this->sliderScaleY->setMinimum(0);
            this->sliderScaleY->setMaximum(this->resolutionHeight * 2);
            this->spinBoxScaleX->setMinimum(0);
            this->spinBoxScaleX->setMaximum(this->resolutionWidth * 2);
            this->spinBoxScaleY->setMinimum(0);
            this->spinBoxScaleY->setMaximum(this->resolutionHeight * 2);

            // This will also set the slider value.
            // TODO: Why QString -> float, see InspectorVolumeWidget.cpp
            this->spinBoxPositionX->setValue(QString("%1").arg(this->command->getPositionX() * this->resolutionWidth).toFloat());
            this->spinBoxPositionY->setValue(QString("%1").arg(this->command->getPositionY() * this->resolutionHeight).toFloat());
            this->spinBoxScaleX->setValue(QString("%1").arg(this->command->getScaleX() * this->resolutionWidth).toFloat());
            this->spinBoxScaleY->setValue(QString("%1").arg(this->command->getScaleY() * this->resolutionHeight).toFloat());

            this->spinBoxDuration->setValue(this->command->getDuration());
            this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
            this->checkBoxDefer->setChecked(this->command->getDefer());

            this->preview = true;
        }
    }

    return QObject::eventFilter(target, event);
}

void InspectorGeometryWidget::loadTween()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTween->blockSignals(true);

    QList<TweenModel> models = DatabaseManager::getInstance().getTween();
    foreach (TweenModel model, models)
        this->comboBoxTween->addItem(model.getValue());

    this->comboBoxTween->blockSignals(false);
}

void InspectorGeometryWidget::sliderPositionXChanged(int positionX)
{
    this->command->setPositionX(static_cast<float>(positionX) / this->resolutionWidth);

    this->spinBoxPositionX->setValue(positionX);

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
}

void InspectorGeometryWidget::spinBoxPositionXChanged(int positionX)
{
    this->sliderPositionX->setValue(positionX);
}

void InspectorGeometryWidget::sliderPositionYChanged(int positionY)
{
    this->command->setPositionY(static_cast<float>(positionY) / this->resolutionHeight);

    this->spinBoxPositionY->setValue(positionY);

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
}

void InspectorGeometryWidget::spinBoxPositionYChanged(int positionY)
{
    this->sliderPositionY->setValue(positionY);
}

void InspectorGeometryWidget::sliderScaleXChanged(int scaleX)
{
    this->command->setScaleX(static_cast<float>(scaleX) / this->resolutionWidth);

    this->spinBoxScaleX->setValue(scaleX);

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
}

void InspectorGeometryWidget::spinBoxScaleXChanged(int scaleX)
{
    this->sliderScaleX->setValue(scaleX);
}

void InspectorGeometryWidget::sliderScaleYChanged(int scaleY)
{
    this->command->setScaleY(static_cast<float>(scaleY) / this->resolutionHeight);

    this->spinBoxScaleY->setValue(scaleY);

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
}

void InspectorGeometryWidget::spinBoxScaleYChanged(int scaleY)
{
    this->sliderScaleY->setValue(scaleY);
}

void InspectorGeometryWidget::durationChanged(int duration)
{
    this->command->setDuration(duration);
}

void InspectorGeometryWidget::tweenChanged(QString tween)
{
    this->command->setTween(tween);
}

void InspectorGeometryWidget::resetPositionX(QString positionX)
{
    this->sliderPositionX->setValue(Mixer::DEFAULT_GEOMETRY_XPOS);
    this->command->setPositionX(this->sliderPositionX->value());

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
}

void InspectorGeometryWidget::resetPositionY(QString positionY)
{
    this->sliderPositionY->setValue(Mixer::DEFAULT_GEOMETRY_YPOS);
    this->command->setPositionY(this->sliderPositionY->value());

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
}

void InspectorGeometryWidget::resetScaleX(QString scaleX)
{
    this->sliderScaleX->setValue(Mixer::DEFAULT_GEOMETRY_XSCALE * this->resolutionWidth);
    this->command->setScaleX(static_cast<float>(this->sliderScaleX->value( ) / this->resolutionWidth));

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
}

void InspectorGeometryWidget::resetScaleY(QString scaleY)
{
    this->sliderScaleY->setValue(Mixer::DEFAULT_GEOMETRY_YSCALE * this->resolutionHeight);
    this->command->setScaleY(static_cast<float>(this->sliderScaleY->value() / this->resolutionHeight));

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
}

void InspectorGeometryWidget::resetDuration(QString duration)
{
    this->spinBoxDuration->setValue(Mixer::DEFAULT_DURATION);
    this->command->setDuration(this->spinBoxDuration->value());
}

void InspectorGeometryWidget::resetTween(QString tween)
{
    this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(Mixer::DEFAULT_TWEEN));
    this->command->setTween(this->comboBoxTween->currentText());
}

void InspectorGeometryWidget::deferChanged(int state)
{
    this->command->setDefer((state == Qt::Checked) ? true : false);
}

void InspectorGeometryWidget::resetDefer(QString defer)
{
    this->checkBoxDefer->setChecked(Mixer::DEFAULT_DEFER);
    this->command->setDefer(this->checkBoxDefer->isChecked());
}
