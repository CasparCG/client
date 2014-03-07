#include "InspectorGeometryWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/PreviewEvent.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Events/Inspector/DeviceChangedEvent.h"
#include "Models/TweenModel.h"

#include <QtCore/QDebug>

#include <QtGui/QApplication>

InspectorGeometryWidget::InspectorGeometryWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL), resolutionWidth(0), resolutionHeight(0)
{
    setupUi(this);

    loadTween();

    qApp->installEventFilter(this);
}

bool InspectorGeometryWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Event::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        this->model = rundownItemSelectedEvent->getLibraryModel();

        blockAllSignals(true);

        if (dynamic_cast<GeometryCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->command = dynamic_cast<GeometryCommand*>(rundownItemSelectedEvent->getCommand());
            const QStringList& channelFormats = DatabaseManager::getInstance().getDeviceByName(this->model->getDeviceName()).getChannelFormats().split(",");
            const FormatModel& formatModel = DatabaseManager::getInstance().getFormat(channelFormats.at(this->command->getChannel() - 1));

            this->resolutionWidth = formatModel.getWidth();
            this->resolutionHeight = formatModel.getHeight();

            setScaleAndPositionValues();

            this->spinBoxDuration->setValue(this->command->getDuration());
            this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
            this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
            this->checkBoxDefer->setChecked(this->command->getDefer());
        }
        else
            this->command = NULL;

        blockAllSignals(false);
    }
    else if (event->type() == static_cast<QEvent::Type>(Event::EventType::DeviceChanged))
    {
        blockAllSignals(true);

        if (this->command != NULL)
        {
            DeviceChangedEvent* deviceChangedEvent = dynamic_cast<DeviceChangedEvent*>(event);
            const QStringList& channelFormats = DatabaseManager::getInstance().getDeviceByName(deviceChangedEvent->getDeviceName()).getChannelFormats().split(",");
            const FormatModel formatModel = DatabaseManager::getInstance().getFormat(channelFormats.at(this->command->getChannel() - 1));

            this->resolutionWidth = formatModel.getWidth();
            this->resolutionHeight = formatModel.getHeight();

            setScaleAndPositionValues();
        }

        blockAllSignals(false);
    }
    else if (event->type() == static_cast<QEvent::Type>(Event::EventType::ChannelChanged))
    {
        blockAllSignals(true);

        if (this->model != NULL && this->command != NULL)
        {
            ChannelChangedEvent* channelChangedEvent = dynamic_cast<ChannelChangedEvent*>(event);
            const QStringList& channelFormats = DatabaseManager::getInstance().getDeviceByName(this->model->getDeviceName()).getChannelFormats().split(",");

            if (channelChangedEvent->getChannel() <= channelFormats.count())
            {
                const FormatModel& formatModel = DatabaseManager::getInstance().getFormat(channelFormats.at(channelChangedEvent->getChannel() - 1));

                this->resolutionWidth = formatModel.getWidth();
                this->resolutionHeight = formatModel.getHeight();

                setScaleAndPositionValues();
            }
        }

        blockAllSignals(false);
    }

    return QObject::eventFilter(target, event);
}

void InspectorGeometryWidget::blockAllSignals(bool block)
{
    this->sliderPositionX->blockSignals(block);
    this->sliderPositionY->blockSignals(block);
    this->sliderScaleX->blockSignals(block);
    this->sliderScaleY->blockSignals(block);
    this->spinBoxPositionX->blockSignals(block);
    this->spinBoxPositionY->blockSignals(block);
    this->spinBoxScaleX->blockSignals(block);
    this->spinBoxScaleY->blockSignals(block);
    this->spinBoxDuration->blockSignals(block);
    this->comboBoxTween->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
    this->checkBoxDefer->blockSignals(block);
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

void InspectorGeometryWidget::setScaleAndPositionValues()
{
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

    this->sliderPositionX->setValue(QString("%1").arg(this->command->getPositionX() * this->resolutionWidth).toFloat());
    this->sliderPositionY->setValue(QString("%1").arg(this->command->getPositionY() * this->resolutionHeight).toFloat());
    this->sliderScaleX->setValue(QString("%1").arg(this->command->getScaleX() * this->resolutionWidth).toFloat());
    this->sliderScaleY->setValue(QString("%1").arg(this->command->getScaleY() * this->resolutionHeight).toFloat());
    this->spinBoxPositionX->setValue(QString("%1").arg(this->command->getPositionX() * this->resolutionWidth).toFloat());
    this->spinBoxPositionY->setValue(QString("%1").arg(this->command->getPositionY() * this->resolutionHeight).toFloat());
    this->spinBoxScaleX->setValue(QString("%1").arg(this->command->getScaleX() * this->resolutionWidth).toFloat());
    this->spinBoxScaleY->setValue(QString("%1").arg(this->command->getScaleY() * this->resolutionHeight).toFloat());
}

void InspectorGeometryWidget::sliderPositionXChanged(int positionX)
{
    this->command->setPositionX(static_cast<float>(positionX) / this->resolutionWidth);

    this->spinBoxPositionX->setValue(positionX);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorGeometryWidget::spinBoxPositionXChanged(int positionX)
{
    this->sliderPositionX->setValue(positionX);
}

void InspectorGeometryWidget::sliderPositionYChanged(int positionY)
{
    this->command->setPositionY(static_cast<float>(positionY) / this->resolutionHeight);

    this->spinBoxPositionY->setValue(positionY);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorGeometryWidget::spinBoxPositionYChanged(int positionY)
{
    this->sliderPositionY->setValue(positionY);
}

void InspectorGeometryWidget::sliderScaleXChanged(int scaleX)
{
    this->command->setScaleX(static_cast<float>(scaleX) / this->resolutionWidth);

    this->spinBoxScaleX->setValue(scaleX);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorGeometryWidget::spinBoxScaleXChanged(int scaleX)
{
    this->sliderScaleX->setValue(scaleX);
}

void InspectorGeometryWidget::sliderScaleYChanged(int scaleY)
{
    this->command->setScaleY(static_cast<float>(scaleY) / this->resolutionHeight);

    this->spinBoxScaleY->setValue(scaleY);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
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

void InspectorGeometryWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}

void InspectorGeometryWidget::deferChanged(int state)
{
    this->command->setDefer((state == Qt::Checked) ? true : false);
}
