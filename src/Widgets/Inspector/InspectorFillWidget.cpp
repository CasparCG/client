#include "InspectorFillWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/PreviewEvent.h"
#include "Models/TweenModel.h"

#include <QtCore/QDebug>

#include <QtGui/QApplication>

InspectorFillWidget::InspectorFillWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL), resolutionWidth(0), resolutionHeight(0)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(deviceChanged(const DeviceChangedEvent&)), this, SLOT(deviceChanged(const DeviceChangedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(channelChanged(const ChannelChangedEvent&)), this, SLOT(channelChanged(const ChannelChangedEvent&)));

    loadTween();
}

void InspectorFillWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<FillCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<FillCommand*>(event.getCommand());

        const DeviceModel model = DatabaseManager::getInstance().getDeviceByName(this->model->getDeviceName());
        if (!model.getName().isEmpty())
        {
            const QStringList& channelFormats = model.getChannelFormats().split(",");
            const FormatModel& formatModel = DatabaseManager::getInstance().getFormat(channelFormats.at(this->command->getChannel() - 1));

            this->resolutionWidth = formatModel.getWidth();
            this->resolutionHeight = formatModel.getHeight();

            setScaleAndPositionValues();
        }

        this->spinBoxTransitionDuration->setValue(this->command->getTransitionDuration());
        this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
        this->checkBoxDefer->setChecked(this->command->getDefer());
        this->checkBoxUseMipmap->setChecked(this->command->getUseMipmap());
    }
    else
        this->command = NULL;

    blockAllSignals(false);
}

void InspectorFillWidget::deviceChanged(const DeviceChangedEvent& event)
{
    blockAllSignals(true);

    if (this->command != NULL)
    {
        const DeviceModel model = DatabaseManager::getInstance().getDeviceByName(event.getDeviceName());
        if (!model.getName().isEmpty())
        {
            const QStringList& channelFormats = model.getChannelFormats().split(",");
            const FormatModel formatModel = DatabaseManager::getInstance().getFormat(channelFormats.at(this->command->getChannel() - 1));

            this->resolutionWidth = formatModel.getWidth();
            this->resolutionHeight = formatModel.getHeight();

            setScaleAndPositionValues();
        }

        this->spinBoxTransitionDuration->setValue(this->command->getTransitionDuration());
        this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
        this->checkBoxDefer->setChecked(this->command->getDefer());
        this->checkBoxUseMipmap->setChecked(this->command->getUseMipmap());
    }

    blockAllSignals(false);
}

void InspectorFillWidget::channelChanged(const ChannelChangedEvent& event)
{
    blockAllSignals(true);

    if (this->model != NULL && this->command != NULL)
    {
        const DeviceModel model = DatabaseManager::getInstance().getDeviceByName(this->model->getDeviceName());
        if (!model.getName().isEmpty())
        {
            const QStringList& channelFormats = model.getChannelFormats().split(",");
            if (event.getChannel() <= channelFormats.count())
            {
                const FormatModel& formatModel = DatabaseManager::getInstance().getFormat(channelFormats.at(event.getChannel() - 1));

                this->resolutionWidth = formatModel.getWidth();
                this->resolutionHeight = formatModel.getHeight();

                setScaleAndPositionValues();
            }
        }

        this->spinBoxTransitionDuration->setValue(this->command->getTransitionDuration());
        this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
        this->checkBoxDefer->setChecked(this->command->getDefer());
        this->checkBoxUseMipmap->setChecked(this->command->getUseMipmap());
    }

    blockAllSignals(false);
}

void InspectorFillWidget::blockAllSignals(bool block)
{
    this->sliderPositionX->blockSignals(block);
    this->sliderPositionY->blockSignals(block);
    this->sliderScaleX->blockSignals(block);
    this->sliderScaleY->blockSignals(block);
    this->spinBoxPositionX->blockSignals(block);
    this->spinBoxPositionY->blockSignals(block);
    this->spinBoxScaleX->blockSignals(block);
    this->spinBoxScaleY->blockSignals(block);
    this->spinBoxTransitionDuration->blockSignals(block);
    this->comboBoxTween->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
    this->checkBoxDefer->blockSignals(block);
    this->checkBoxUseMipmap->blockSignals(block);
}

void InspectorFillWidget::loadTween()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTween->blockSignals(true);

    QList<TweenModel> models = DatabaseManager::getInstance().getTween();
    foreach (TweenModel model, models)
        this->comboBoxTween->addItem(model.getValue());

    this->comboBoxTween->blockSignals(false);
}

void InspectorFillWidget::setScaleAndPositionValues()
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

void InspectorFillWidget::sliderPositionXChanged(int positionX)
{
    this->command->setPositionX(static_cast<float>(positionX) / this->resolutionWidth);

    this->spinBoxPositionX->blockSignals(true);
    this->spinBoxPositionX->setValue(positionX);
    this->spinBoxPositionX->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorFillWidget::spinBoxPositionXChanged(int positionX)
{
    this->command->setPositionX(static_cast<float>(positionX) / this->resolutionWidth);

    this->sliderPositionX->blockSignals(true);
    this->sliderPositionX->setValue(positionX);
    this->sliderPositionX->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorFillWidget::sliderPositionYChanged(int positionY)
{
    this->command->setPositionY(static_cast<float>(positionY) / this->resolutionHeight);

    this->spinBoxPositionY->blockSignals(true);
    this->spinBoxPositionY->setValue(positionY);
    this->spinBoxPositionY->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorFillWidget::spinBoxPositionYChanged(int positionY)
{
    this->command->setPositionY(static_cast<float>(positionY) / this->resolutionHeight);

    this->sliderPositionY->blockSignals(true);
    this->sliderPositionY->setValue(positionY);
    this->sliderPositionY->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorFillWidget::sliderScaleXChanged(int scaleX)
{
    this->command->setScaleX(static_cast<float>(scaleX) / this->resolutionWidth);

    this->spinBoxScaleX->blockSignals(true);
    this->spinBoxScaleX->setValue(scaleX);
    this->spinBoxScaleX->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorFillWidget::spinBoxScaleXChanged(int scaleX)
{
    this->command->setScaleX(static_cast<float>(scaleX) / this->resolutionWidth);

    this->sliderScaleX->blockSignals(true);
    this->sliderScaleX->setValue(scaleX);
    this->sliderScaleX->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorFillWidget::sliderScaleYChanged(int scaleY)
{
    this->command->setScaleY(static_cast<float>(scaleY) / this->resolutionHeight);

    this->spinBoxScaleY->blockSignals(true);
    this->spinBoxScaleY->setValue(scaleY);
    this->spinBoxScaleY->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorFillWidget::spinBoxScaleYChanged(int scaleY)
{
    this->command->setScaleY(static_cast<float>(scaleY) / this->resolutionHeight);

    this->sliderScaleY->blockSignals(true);
    this->sliderScaleY->setValue(scaleY);
    this->sliderScaleY->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorFillWidget::transitionDurationChanged(int transitionDuration)
{
    this->command->setTransitionDuration(transitionDuration);
}

void InspectorFillWidget::tweenChanged(QString tween)
{
    this->command->setTween(tween);
}

void InspectorFillWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}

void InspectorFillWidget::deferChanged(int state)
{
    this->command->setDefer((state == Qt::Checked) ? true : false);
}

void InspectorFillWidget::useMipmapChanged(int state)
{
    this->command->setUseMipmap((state == Qt::Checked) ? true : false);
}
