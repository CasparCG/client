#include "InspectorCropWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/PreviewEvent.h"
#include "Models/TweenModel.h"

#include <QtWidgets/QApplication>

InspectorCropWidget::InspectorCropWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL), resolutionWidth(0), resolutionHeight(0)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(deviceChanged(const DeviceChangedEvent&)), this, SLOT(deviceChanged(const DeviceChangedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(channelChanged(const ChannelChangedEvent&)), this, SLOT(channelChanged(const ChannelChangedEvent&)));

    loadTween();
}

void InspectorCropWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->command = nullptr;
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<CropCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<CropCommand*>(event.getCommand());

        const DeviceModel model = DatabaseManager::getInstance().getDeviceByName(this->model->getDeviceName());
        if (!model.getName().isEmpty())
        {
            const QStringList& channelFormats = DatabaseManager::getInstance().getDeviceByName(model.getName()).getChannelFormats().split(",");
            const FormatModel& formatModel = DatabaseManager::getInstance().getFormat(channelFormats.at(this->command->channel.get() - 1));

            this->resolutionWidth = formatModel.getWidth();
            this->resolutionHeight = formatModel.getHeight();

            setPositionValues();
        }

        this->spinBoxTransitionDuration->setValue(this->command->getTransitionDuration());
        this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
        this->checkBoxDefer->setChecked(this->command->getDefer());
    }

    blockAllSignals(false);
}

void InspectorCropWidget::deviceChanged(const DeviceChangedEvent& event)
{
    blockAllSignals(true);

    if (this->command != NULL)
    {
        const DeviceModel model = DatabaseManager::getInstance().getDeviceByName(event.getDeviceName());
        if (!model.getName().isEmpty())
        {
            const QStringList& channelFormats = DatabaseManager::getInstance().getDeviceByName(model.getName()).getChannelFormats().split(",");
            if (this->command->channel.get() > channelFormats.count())
                return;

            const FormatModel formatModel = DatabaseManager::getInstance().getFormat(channelFormats.at(this->command->channel.get() - 1));

            this->resolutionWidth = formatModel.getWidth();
            this->resolutionHeight = formatModel.getHeight();

            setPositionValues();
        }

        this->spinBoxTransitionDuration->setValue(this->command->getTransitionDuration());
        this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
        this->checkBoxDefer->setChecked(this->command->getDefer());
    }

    blockAllSignals(false);
}

void InspectorCropWidget::channelChanged(const ChannelChangedEvent& event)
{
    blockAllSignals(true);

    if (this->model != NULL && this->command != NULL)
    {
        const DeviceModel model = DatabaseManager::getInstance().getDeviceByName(this->model->getDeviceName());
        if (!model.getName().isEmpty())
        {
            const QStringList& channelFormats = DatabaseManager::getInstance().getDeviceByName(model.getName()).getChannelFormats().split(",");
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
        this->checkBoxDefer->setChecked(this->command->getDefer());
    }

    blockAllSignals(false);
}

void InspectorCropWidget::blockAllSignals(bool block)
{
    this->sliderLeft->blockSignals(block);
    this->sliderTop->blockSignals(block);
    this->sliderRight->blockSignals(block);
    this->sliderBottom->blockSignals(block);

    this->sliderLeft->blockSignals(block);
    this->spinBoxTop->blockSignals(block);
    this->spinBoxRight->blockSignals(block);
    this->spinBoxBottom->blockSignals(block);

    this->spinBoxTransitionDuration->blockSignals(block);
    this->comboBoxTween->blockSignals(block);
    this->checkBoxDefer->blockSignals(block);
}

void InspectorCropWidget::loadTween()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTween->blockSignals(true);

    QList<TweenModel> models = DatabaseManager::getInstance().getTween();
    foreach (TweenModel model, models)
        this->comboBoxTween->addItem(model.getValue());

    this->comboBoxTween->blockSignals(false);
}

void InspectorCropWidget::setPositionValues()
{
    this->sliderLeft->setMaximum(this->resolutionWidth);
    this->sliderTop->setMaximum(this->resolutionHeight);
    this->sliderRight->setMaximum(this->resolutionWidth);
    this->sliderBottom->setMaximum(this->resolutionHeight);

    this->spinBoxLeft->setMaximum(this->resolutionWidth);
    this->spinBoxTop->setMaximum(this->resolutionHeight);
    this->spinBoxRight->setMaximum(this->resolutionWidth);
    this->spinBoxBottom->setMaximum(this->resolutionHeight);

    this->sliderLeft->setValue(QString("%1").arg(this->command->getLeft() * this->resolutionWidth).toFloat());
    this->sliderTop->setValue(QString("%1").arg(this->command->getTop() * this->resolutionHeight).toFloat());
    this->sliderRight->setValue(QString("%1").arg(this->command->getRight() * this->resolutionWidth).toFloat());
    this->sliderBottom->setValue(QString("%1").arg(this->command->getBottom() * this->resolutionHeight).toFloat());

    this->spinBoxLeft->setValue(QString("%1").arg(this->command->getLeft() * this->resolutionWidth).toFloat());
    this->spinBoxTop->setValue(QString("%1").arg(this->command->getTop() * this->resolutionHeight).toFloat());
    this->spinBoxRight->setValue(QString("%1").arg(this->command->getRight() * this->resolutionWidth).toFloat());
    this->spinBoxBottom->setValue(QString("%1").arg(this->command->getBottom() * this->resolutionHeight).toFloat());
}

void InspectorCropWidget::sliderLeftChanged(int left)
{
    this->command->setLeft(static_cast<float>(left) / this->resolutionWidth);

    this->spinBoxLeft->blockSignals(true);
    this->spinBoxLeft->setValue(left);
    this->spinBoxLeft->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorCropWidget::sliderTopChanged(int top)
{
    this->command->setTop(static_cast<float>(top) / this->resolutionHeight);

    this->spinBoxTop->blockSignals(true);
    this->spinBoxTop->setValue(top);
    this->spinBoxTop->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorCropWidget::sliderRightChanged(int right)
{
    this->command->setRight(static_cast<float>(right) / this->resolutionWidth);

    this->spinBoxRight->blockSignals(true);
    this->spinBoxRight->setValue(right);
    this->spinBoxRight->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorCropWidget::sliderBottomChanged(int bottom)
{
    this->command->setBottom(static_cast<float>(bottom) / this->resolutionHeight);

    this->spinBoxBottom->blockSignals(true);
    this->spinBoxBottom->setValue(bottom);
    this->spinBoxBottom->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorCropWidget::spinBoxLeftChanged(int left)
{
    this->command->setLeft(static_cast<float>(left) / this->resolutionWidth);

    this->sliderLeft->blockSignals(true);
    this->sliderLeft->setValue(left);
    this->sliderLeft->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorCropWidget::spinBoxTopChanged(int top)
{
    this->command->setTop(static_cast<float>(top) / this->resolutionHeight);

    this->sliderTop->blockSignals(true);
    this->sliderTop->setValue(top);
    this->sliderTop->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorCropWidget::spinBoxRightChanged(int right)
{
    this->command->setRight(static_cast<float>(right) / this->resolutionWidth);

    this->sliderRight->blockSignals(true);
    this->sliderRight->setValue(right);
    this->sliderRight->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorCropWidget::spinBoxBottomChanged(int bottom)
{
    this->command->setBottom(static_cast<float>(bottom) / this->resolutionHeight);

    this->sliderBottom->blockSignals(true);
    this->sliderBottom->setValue(bottom);
    this->sliderBottom->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorCropWidget::transitionDurationChanged(int transitionDuration)
{
    this->command->setTransitionDuration(transitionDuration);
}

void InspectorCropWidget::tweenChanged(QString tween)
{
    this->command->setTween(tween);
}

void InspectorCropWidget::deferChanged(int state)
{
    this->command->setDefer((state == Qt::Checked) ? true : false);
}
