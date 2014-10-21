#include "InspectorPerspectiveWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/PreviewEvent.h"
#include "Models/TweenModel.h"

#include <QtCore/QDebug>

#include <QtGui/QApplication>

InspectorPerspectiveWidget::InspectorPerspectiveWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL), resolutionWidth(0), resolutionHeight(0)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(deviceChanged(const DeviceChangedEvent&)), this, SLOT(deviceChanged(const DeviceChangedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(channelChanged(const ChannelChangedEvent&)), this, SLOT(channelChanged(const ChannelChangedEvent&)));

    loadTween();
}

void InspectorPerspectiveWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<PerspectiveCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<PerspectiveCommand*>(event.getCommand());

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
        this->checkBoxUseMipmap->setChecked(this->command->getUseMipmap());
    }
    else
        this->command = NULL;

    blockAllSignals(false);
}

void InspectorPerspectiveWidget::deviceChanged(const DeviceChangedEvent& event)
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
        this->checkBoxUseMipmap->setChecked(this->command->getUseMipmap());
    }

    blockAllSignals(false);
}

void InspectorPerspectiveWidget::channelChanged(const ChannelChangedEvent& event)
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
        this->checkBoxUseMipmap->setChecked(this->command->getUseMipmap());
    }

    blockAllSignals(false);
}

void InspectorPerspectiveWidget::blockAllSignals(bool block)
{
    this->sliderUpperLeftX->blockSignals(block);
    this->sliderUpperLeftY->blockSignals(block);
    this->sliderUpperRightX->blockSignals(block);
    this->sliderUpperRightY->blockSignals(block);
    this->sliderLowerRightX->blockSignals(block);
    this->sliderLowerRightY->blockSignals(block);
    this->sliderLowerLeftX->blockSignals(block);
    this->sliderLowerLeftY->blockSignals(block);
    this->spinBoxUpperLeftX->blockSignals(block);
    this->spinBoxUpperLeftY->blockSignals(block);
    this->spinBoxUpperRightX->blockSignals(block);
    this->spinBoxUpperRightY->blockSignals(block);
    this->spinBoxLowerRightX->blockSignals(block);
    this->spinBoxLowerRightY->blockSignals(block);
    this->spinBoxLowerLeftX->blockSignals(block);
    this->spinBoxLowerLeftY->blockSignals(block);
    this->spinBoxTransitionDuration->blockSignals(block);
    this->comboBoxTween->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
    this->checkBoxDefer->blockSignals(block);
    this->checkBoxUseMipmap->blockSignals(block);
}

void InspectorPerspectiveWidget::loadTween()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTween->blockSignals(true);

    QList<TweenModel> models = DatabaseManager::getInstance().getTween();
    foreach (TweenModel model, models)
        this->comboBoxTween->addItem(model.getValue());

    this->comboBoxTween->blockSignals(false);
}

void InspectorPerspectiveWidget::setPositionValues()
{
    this->sliderUpperLeftX->setMinimum(-this->resolutionWidth * 2);
    this->sliderUpperLeftX->setMaximum(this->resolutionWidth * 2);
    this->sliderUpperLeftY->setMinimum(-this->resolutionHeight * 2);
    this->sliderUpperLeftY->setMaximum(this->resolutionHeight * 2);
    this->sliderUpperRightX->setMinimum(-this->resolutionWidth * 2);
    this->sliderUpperRightX->setMaximum(this->resolutionWidth * 2);
    this->sliderUpperRightY->setMinimum(-this->resolutionHeight * 2);
    this->sliderUpperRightY->setMaximum(this->resolutionHeight * 2);
    this->sliderLowerRightX->setMinimum(-this->resolutionWidth * 2);
    this->sliderLowerRightX->setMaximum(this->resolutionWidth * 2);
    this->sliderLowerRightY->setMinimum(-this->resolutionHeight * 2);
    this->sliderLowerRightY->setMaximum(this->resolutionHeight * 2);
    this->sliderLowerLeftX->setMinimum(-this->resolutionWidth * 2);
    this->sliderLowerLeftX->setMaximum(this->resolutionWidth * 2);
    this->sliderLowerLeftY->setMinimum(-this->resolutionHeight * 2);
    this->sliderLowerLeftY->setMaximum(this->resolutionHeight * 2);

    this->spinBoxUpperLeftX->setMinimum(-this->resolutionWidth * 2);
    this->spinBoxUpperLeftX->setMaximum(this->resolutionWidth * 2);
    this->spinBoxUpperLeftY->setMinimum(-this->resolutionHeight * 2);
    this->spinBoxUpperLeftY->setMaximum(this->resolutionHeight * 2);
    this->spinBoxUpperRightX->setMinimum(-this->resolutionWidth * 2);
    this->spinBoxUpperRightX->setMaximum(this->resolutionWidth * 2);
    this->spinBoxUpperRightY->setMinimum(-this->resolutionHeight * 2);
    this->spinBoxUpperRightY->setMaximum(this->resolutionHeight * 2);
    this->spinBoxLowerRightX->setMinimum(-this->resolutionWidth * 2);
    this->spinBoxLowerRightX->setMaximum(this->resolutionWidth * 2);
    this->spinBoxLowerRightY->setMinimum(-this->resolutionHeight * 2);
    this->spinBoxLowerRightY->setMaximum(this->resolutionHeight * 2);
    this->spinBoxLowerLeftX->setMinimum(-this->resolutionWidth * 2);
    this->spinBoxLowerLeftX->setMaximum(this->resolutionWidth * 2);
    this->spinBoxLowerLeftY->setMinimum(-this->resolutionHeight * 2);
    this->spinBoxLowerLeftY->setMaximum(this->resolutionHeight * 2);

    this->sliderUpperLeftX->setValue(QString("%1").arg(this->command->getUpperLeftX() * this->resolutionWidth).toFloat());
    this->sliderUpperLeftY->setValue(QString("%1").arg(this->command->getUpperLeftY() * this->resolutionHeight).toFloat());
    this->sliderUpperRightX->setValue(QString("%1").arg(this->command->getUpperRightX() * this->resolutionWidth).toFloat());
    this->sliderUpperRightY->setValue(QString("%1").arg(this->command->getUpperRightY() * this->resolutionHeight).toFloat());
    this->sliderLowerRightX->setValue(QString("%1").arg(this->command->getLowerRightX() * this->resolutionWidth).toFloat());
    this->sliderLowerRightY->setValue(QString("%1").arg(this->command->getLowerRightY() * this->resolutionHeight).toFloat());
    this->sliderLowerLeftX->setValue(QString("%1").arg(this->command->getLowerLeftX() * this->resolutionWidth).toFloat());
    this->sliderLowerLeftY->setValue(QString("%1").arg(this->command->getLowerLeftY() * this->resolutionHeight).toFloat());

    this->spinBoxUpperLeftX->setValue(QString("%1").arg(this->command->getUpperLeftX() * this->resolutionWidth).toFloat());
    this->spinBoxUpperLeftY->setValue(QString("%1").arg(this->command->getUpperLeftY() * this->resolutionHeight).toFloat());
    this->spinBoxUpperRightX->setValue(QString("%1").arg(this->command->getUpperRightX() * this->resolutionWidth).toFloat());
    this->spinBoxUpperRightY->setValue(QString("%1").arg(this->command->getUpperRightY() * this->resolutionHeight).toFloat());
    this->spinBoxLowerRightX->setValue(QString("%1").arg(this->command->getLowerRightX() * this->resolutionWidth).toFloat());
    this->spinBoxLowerRightY->setValue(QString("%1").arg(this->command->getLowerRightY() * this->resolutionHeight).toFloat());
    this->spinBoxLowerLeftX->setValue(QString("%1").arg(this->command->getLowerLeftX() * this->resolutionWidth).toFloat());
    this->spinBoxLowerLeftY->setValue(QString("%1").arg(this->command->getLowerLeftY() * this->resolutionHeight).toFloat());
}

void InspectorPerspectiveWidget::sliderUpperLeftXChanged(int upperLeftX)
{
    this->command->setUpperLeftX(static_cast<float>(upperLeftX) / this->resolutionWidth);

    this->spinBoxUpperLeftX->blockSignals(true);
    this->spinBoxUpperLeftX->setValue(upperLeftX);
    this->spinBoxUpperLeftX->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorPerspectiveWidget::spinBoxUpperLeftXChanged(int upperLeftX)
{
    this->command->setUpperLeftX(static_cast<float>(upperLeftX) / this->resolutionWidth);

    this->sliderUpperLeftX->blockSignals(true);
    this->sliderUpperLeftX->setValue(upperLeftX);
    this->sliderUpperLeftX->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorPerspectiveWidget::sliderUpperLeftYChanged(int upperLeftY)
{
    this->command->setUpperLeftY(static_cast<float>(upperLeftY) / this->resolutionHeight);

    this->spinBoxUpperLeftY->blockSignals(true);
    this->spinBoxUpperLeftY->setValue(upperLeftY);
    this->spinBoxUpperLeftY->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorPerspectiveWidget::spinBoxUpperLeftYChanged(int upperLeftY)
{
    this->command->setUpperLeftY(static_cast<float>(upperLeftY) / this->resolutionHeight);

    this->sliderUpperLeftY->blockSignals(true);
    this->sliderUpperLeftY->setValue(upperLeftY);
    this->sliderUpperLeftY->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorPerspectiveWidget::sliderUpperRightXChanged(int upperRightX)
{
    this->command->setUpperRightX(static_cast<float>(upperRightX) / this->resolutionWidth);

    this->spinBoxUpperRightX->blockSignals(true);
    this->spinBoxUpperRightX->setValue(upperRightX);
    this->spinBoxUpperRightX->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorPerspectiveWidget::spinBoxUpperRightXChanged(int upperRightX)
{
    this->command->setUpperRightX(static_cast<float>(upperRightX) / this->resolutionWidth);

    this->sliderUpperRightX->blockSignals(true);
    this->sliderUpperRightX->setValue(upperRightX);
    this->sliderUpperRightX->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorPerspectiveWidget::sliderUpperRightYChanged(int upperRightY)
{
    this->command->setUpperRightY(static_cast<float>(upperRightY) / this->resolutionHeight);

    this->spinBoxUpperRightY->blockSignals(true);
    this->spinBoxUpperRightY->setValue(upperRightY);
    this->spinBoxUpperRightY->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorPerspectiveWidget::spinBoxUpperRightYChanged(int upperRightY)
{
    this->command->setUpperRightY(static_cast<float>(upperRightY) / this->resolutionHeight);

    this->sliderUpperRightY->blockSignals(true);
    this->sliderUpperRightY->setValue(upperRightY);
    this->sliderUpperRightY->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorPerspectiveWidget::sliderLowerRightXChanged(int lowerRightX)
{
    this->command->setLowerRightX(static_cast<float>(lowerRightX) / this->resolutionWidth);

    this->spinBoxLowerRightX->blockSignals(true);
    this->spinBoxLowerRightX->setValue(lowerRightX);
    this->spinBoxLowerRightX->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorPerspectiveWidget::spinBoxLowerRightXChanged(int lowerRightX)
{
    this->command->setLowerRightX(static_cast<float>(lowerRightX) / this->resolutionWidth);

    this->sliderLowerRightX->blockSignals(true);
    this->sliderLowerRightX->setValue(lowerRightX);
    this->sliderLowerRightX->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorPerspectiveWidget::sliderLowerRightYChanged(int lowerRightY)
{
    this->command->setLowerRightY(static_cast<float>(lowerRightY) / this->resolutionHeight);

    this->spinBoxLowerRightY->blockSignals(true);
    this->spinBoxLowerRightY->setValue(lowerRightY);
    this->spinBoxLowerRightY->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorPerspectiveWidget::spinBoxLowerRightYChanged(int lowerRightY)
{
    this->command->setLowerRightY(static_cast<float>(lowerRightY) / this->resolutionHeight);

    this->sliderLowerRightY->blockSignals(true);
    this->sliderLowerRightY->setValue(lowerRightY);
    this->sliderLowerRightY->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorPerspectiveWidget::sliderLowerLeftXChanged(int lowerLeftX)
{
    this->command->setLowerLeftX(static_cast<float>(lowerLeftX) / this->resolutionWidth);

    this->spinBoxLowerLeftX->blockSignals(true);
    this->spinBoxLowerLeftX->setValue(lowerLeftX);
    this->spinBoxLowerLeftX->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorPerspectiveWidget::spinBoxLowerLeftXChanged(int lowerLeftX)
{
    this->command->setLowerLeftX(static_cast<float>(lowerLeftX) / this->resolutionWidth);

    this->sliderLowerLeftX->blockSignals(true);
    this->sliderLowerLeftX->setValue(lowerLeftX);
    this->sliderLowerLeftX->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorPerspectiveWidget::sliderLowerLeftYChanged(int lowerLeftY)
{
    this->command->setLowerLeftY(static_cast<float>(lowerLeftY) / this->resolutionHeight);

    this->spinBoxLowerLeftY->blockSignals(true);
    this->spinBoxLowerLeftY->setValue(lowerLeftY);
    this->spinBoxLowerLeftY->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorPerspectiveWidget::spinBoxLowerLeftYChanged(int lowerLeftY)
{
    this->command->setLowerLeftY(static_cast<float>(lowerLeftY) / this->resolutionHeight);

    this->sliderLowerLeftY->blockSignals(true);
    this->sliderLowerLeftY->setValue(lowerLeftY);
    this->sliderLowerLeftY->blockSignals(false);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorPerspectiveWidget::transitionDurationChanged(int transitionDuration)
{
    this->command->setTransitionDuration(transitionDuration);
}

void InspectorPerspectiveWidget::tweenChanged(QString tween)
{
    this->command->setTween(tween);
}

void InspectorPerspectiveWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}

void InspectorPerspectiveWidget::deferChanged(int state)
{
    this->command->setDefer((state == Qt::Checked) ? true : false);
}

void InspectorPerspectiveWidget::useMipmapChanged(int state)
{
    this->command->setUseMipmap((state == Qt::Checked) ? true : false);
}
