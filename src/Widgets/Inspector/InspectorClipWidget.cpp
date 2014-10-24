#include "InspectorClipWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/PreviewEvent.h"
#include "Models/TweenModel.h"

#include <QtGui/QApplication>

InspectorClipWidget::InspectorClipWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL), resolutionWidth(0), resolutionHeight(0)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(deviceChanged(const DeviceChangedEvent&)), this, SLOT(deviceChanged(const DeviceChangedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(channelChanged(const ChannelChangedEvent&)), this, SLOT(channelChanged(const ChannelChangedEvent&)));

    loadTween();
}

void InspectorClipWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<ClipCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<ClipCommand*>(event.getCommand());

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
        this->checkBoxDefer->setChecked(this->command->getDefer());
    }

    blockAllSignals(false);
}

void InspectorClipWidget::deviceChanged(const DeviceChangedEvent& event)
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
        this->checkBoxDefer->setChecked(this->command->getDefer());
    }

    blockAllSignals(false);
}

void InspectorClipWidget::channelChanged(const ChannelChangedEvent& event)
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
        this->checkBoxDefer->setChecked(this->command->getDefer());
    }

    blockAllSignals(false);
}

void InspectorClipWidget::blockAllSignals(bool block)
{
    this->sliderLeft->blockSignals(block);
    this->sliderWidth->blockSignals(block);
    this->sliderTop->blockSignals(block);
    this->sliderHeight->blockSignals(block);
    this->spinBoxLeft->blockSignals(block);
    this->spinBoxWidth->blockSignals(block);
    this->spinBoxTop->blockSignals(block);
    this->spinBoxHeight->blockSignals(block);
    this->spinBoxTransitionDuration->blockSignals(block);
    this->comboBoxTween->blockSignals(block);
    this->checkBoxDefer->blockSignals(block);
}

void InspectorClipWidget::setPositionValues()
{
    this->sliderLeft->setMaximum(this->resolutionWidth);
    this->sliderWidth->setMaximum(this->resolutionWidth);
    this->sliderTop->setMaximum(this->resolutionHeight);
    this->sliderHeight->setMaximum(this->resolutionHeight);

    this->spinBoxLeft->setMaximum(this->resolutionWidth);
    this->spinBoxWidth->setMaximum(this->resolutionWidth);
    this->spinBoxTop->setMaximum(this->resolutionHeight);
    this->spinBoxHeight->setMaximum(this->resolutionHeight);

    this->sliderLeft->setValue(QString("%1").arg(this->command->getLeft() * this->resolutionWidth).toFloat());
    this->sliderWidth->setValue(QString("%1").arg(this->command->getWidth() * this->resolutionWidth).toFloat());
    this->sliderTop->setValue(QString("%1").arg(this->command->getTop() * this->resolutionHeight).toFloat());
    this->sliderHeight->setValue(QString("%1").arg(this->command->getHeight() * this->resolutionHeight).toFloat());

    this->spinBoxLeft->setValue(QString("%1").arg(this->command->getLeft() * this->resolutionWidth).toFloat());
    this->spinBoxWidth->setValue(QString("%1").arg(this->command->getWidth() * this->resolutionWidth).toFloat());
    this->spinBoxTop->setValue(QString("%1").arg(this->command->getTop() * this->resolutionHeight).toFloat());
    this->spinBoxHeight->setValue(QString("%1").arg(this->command->getHeight() * this->resolutionHeight).toFloat());
}

void InspectorClipWidget::loadTween()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTween->blockSignals(true);

    QList<TweenModel> models = DatabaseManager::getInstance().getTween();
    foreach (TweenModel model, models)
        this->comboBoxTween->addItem(model.getValue());

    this->comboBoxTween->blockSignals(false);
}

void InspectorClipWidget::sliderLeftChanged(int clipLeft)
{
    this->command->setLeft(static_cast<float>(clipLeft) / this->resolutionWidth);

    this->spinBoxLeft->blockSignals(true);
    this->spinBoxLeft->setValue(clipLeft);
    this->spinBoxLeft->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorClipWidget::spinBoxLeftChanged(int clipLeft)
{
    this->command->setLeft(static_cast<float>(clipLeft) / this->resolutionWidth);

    this->sliderLeft->blockSignals(true);
    this->sliderLeft->setValue(clipLeft);
    this->sliderLeft->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorClipWidget::sliderWidthChanged(int clipRight)
{
    this->command->setWidth(static_cast<float>(clipRight) / this->resolutionWidth);

    this->spinBoxWidth->blockSignals(true);
    this->spinBoxWidth->setValue(clipRight);
    this->spinBoxWidth->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorClipWidget::spinBoxWidthChanged(int clipRight)
{
    this->command->setWidth(static_cast<float>(clipRight) / this->resolutionWidth);

    this->sliderWidth->blockSignals(true);
    this->sliderWidth->setValue(clipRight);
    this->sliderWidth->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorClipWidget::sliderTopChanged(int clipTop)
{
    this->command->setTop(static_cast<float>(clipTop) / this->resolutionHeight);

    this->spinBoxTop->blockSignals(true);
    this->spinBoxTop->setValue(clipTop);
    this->spinBoxTop->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorClipWidget::spinBoxTopChanged(int clipTop)
{
    this->command->setTop(static_cast<float>(clipTop) / this->resolutionHeight);

    this->sliderTop->blockSignals(true);
    this->sliderTop->setValue(clipTop);
    this->sliderTop->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorClipWidget::sliderHeightChanged(int clipBottom)
{
    this->command->setHeight(static_cast<float>(clipBottom) / this->resolutionHeight);

    this->spinBoxHeight->blockSignals(true);
    this->spinBoxHeight->setValue(clipBottom);
    this->spinBoxHeight->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorClipWidget::spinBoxHeightChanged(int clipBottom)
{
    this->command->setHeight(static_cast<float>(clipBottom) / this->resolutionHeight);

    this->sliderHeight->blockSignals(true);
    this->sliderHeight->setValue(clipBottom);
    this->sliderHeight->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorClipWidget::transitionDurationChanged(int transitionDuration)
{
    this->command->setTransitionDuration(transitionDuration);
}

void InspectorClipWidget::tweenChanged(QString tween)
{
    this->command->setTween(tween);
}

void InspectorClipWidget::deferChanged(int state)
{
    this->command->setDefer((state == Qt::Checked) ? true : false);
}
