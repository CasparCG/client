#include "InspectorAnchorWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/PreviewEvent.h"
#include "Models/TweenModel.h"

#include <QtCore/QDebug>

#include <QtGui/QApplication>

InspectorAnchorWidget::InspectorAnchorWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL), resolutionWidth(0), resolutionHeight(0)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(deviceChanged(const DeviceChangedEvent&)), this, SLOT(deviceChanged(const DeviceChangedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(channelChanged(const ChannelChangedEvent&)), this, SLOT(channelChanged(const ChannelChangedEvent&)));

    loadTween();
}

void InspectorAnchorWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<AnchorCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<AnchorCommand*>(event.getCommand());

        const DeviceModel model = DatabaseManager::getInstance().getDeviceByName(this->model->getDeviceName());
        if (!model.getName().isEmpty())
        {
            const QStringList& channelFormats = model.getChannelFormats().split(",");
            const FormatModel& formatModel = DatabaseManager::getInstance().getFormat(channelFormats.at(this->command->getChannel() - 1));

            this->resolutionWidth = formatModel.getWidth();
            this->resolutionHeight = formatModel.getHeight();

            setPositionValues();
        }

        this->spinBoxTransitionDuration->setValue(this->command->getTransitionDuration());
        this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
        this->checkBoxDefer->setChecked(this->command->getDefer());
    }
    else
        this->command = NULL;

    blockAllSignals(false);
}

void InspectorAnchorWidget::deviceChanged(const DeviceChangedEvent& event)
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

            setPositionValues();
        }

        this->spinBoxTransitionDuration->setValue(this->command->getTransitionDuration());
        this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
        this->checkBoxDefer->setChecked(this->command->getDefer());
    }

    blockAllSignals(false);
}

void InspectorAnchorWidget::channelChanged(const ChannelChangedEvent& event)
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

                setPositionValues();
            }
        }

        this->spinBoxTransitionDuration->setValue(this->command->getTransitionDuration());
        this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
        this->checkBoxDefer->setChecked(this->command->getDefer());
    }

    blockAllSignals(false);
}

void InspectorAnchorWidget::blockAllSignals(bool block)
{
    this->sliderPositionX->blockSignals(block);
    this->sliderPositionY->blockSignals(block);
    this->spinBoxPositionX->blockSignals(block);
    this->spinBoxPositionY->blockSignals(block);
    this->spinBoxTransitionDuration->blockSignals(block);
    this->comboBoxTween->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
    this->checkBoxDefer->blockSignals(block);
}

void InspectorAnchorWidget::loadTween()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTween->blockSignals(true);

    QList<TweenModel> models = DatabaseManager::getInstance().getTween();
    foreach (TweenModel model, models)
        this->comboBoxTween->addItem(model.getValue());

    this->comboBoxTween->blockSignals(false);
}

void InspectorAnchorWidget::setPositionValues()
{
    this->sliderPositionX->setMinimum(-this->resolutionWidth * 2);
    this->sliderPositionX->setMaximum(this->resolutionWidth * 2);
    this->sliderPositionY->setMinimum(-this->resolutionHeight * 2);
    this->sliderPositionY->setMaximum(this->resolutionHeight * 2);
    this->spinBoxPositionX->setMinimum(-this->resolutionWidth * 2);
    this->spinBoxPositionX->setMaximum(this->resolutionWidth * 2);
    this->spinBoxPositionY->setMinimum(-this->resolutionHeight * 2);
    this->spinBoxPositionY->setMaximum(this->resolutionHeight * 2);

    this->sliderPositionX->setValue(QString("%1").arg(this->command->getPositionX() * this->resolutionWidth).toFloat());
    this->sliderPositionY->setValue(QString("%1").arg(this->command->getPositionY() * this->resolutionHeight).toFloat());
    this->spinBoxPositionX->setValue(QString("%1").arg(this->command->getPositionX() * this->resolutionWidth).toFloat());
    this->spinBoxPositionY->setValue(QString("%1").arg(this->command->getPositionY() * this->resolutionHeight).toFloat());
}

void InspectorAnchorWidget::sliderPositionXChanged(int positionX)
{
    this->command->setPositionX(static_cast<float>(positionX) / this->resolutionWidth);

    this->spinBoxPositionX->blockSignals(true);
    this->spinBoxPositionX->setValue(positionX);
    this->spinBoxPositionX->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorAnchorWidget::spinBoxPositionXChanged(int positionX)
{
    this->command->setPositionX(static_cast<float>(positionX) / this->resolutionWidth);

    this->sliderPositionX->blockSignals(true);
    this->sliderPositionX->setValue(positionX);
    this->sliderPositionX->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorAnchorWidget::sliderPositionYChanged(int positionY)
{
    this->command->setPositionY(static_cast<float>(positionY) / this->resolutionHeight);

    this->spinBoxPositionY->blockSignals(true);
    this->spinBoxPositionY->setValue(positionY);
    this->spinBoxPositionY->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorAnchorWidget::spinBoxPositionYChanged(int positionY)
{
    this->command->setPositionY(static_cast<float>(positionY) / this->resolutionHeight);

    this->sliderPositionY->blockSignals(true);
    this->sliderPositionY->setValue(positionY);
    this->sliderPositionY->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorAnchorWidget::transitionDurationChanged(int transitionDuration)
{
    this->command->setTransitionDuration(transitionDuration);
}

void InspectorAnchorWidget::tweenChanged(QString tween)
{
    this->command->setTween(tween);
}

void InspectorAnchorWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}

void InspectorAnchorWidget::deferChanged(int state)
{
    this->command->setDefer((state == Qt::Checked) ? true : false);
}
