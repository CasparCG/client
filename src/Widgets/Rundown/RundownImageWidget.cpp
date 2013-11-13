#include "RundownImageWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "DeviceManager.h"
#include "GpiManager.h"
#include "EventManager.h"
#include "Events/ConnectionStateChangedEvent.h"
#include "Events/Inspector/TargetChangedEvent.h"
#include "Events/Inspector/LabelChangedEvent.h"
#include "Events/Inspector/DeviceChangedEvent.h"

#include <math.h>

#include <QtCore/QObject>
#include <QtCore/QTimer>

RundownImageWidget::RundownImageWidget(const LibraryModel& model, QWidget* parent, const QString& color,
                                       bool active, bool loaded, bool paused, bool playing, bool inGroup,
                                       bool compactView)
    : QWidget(parent),
      active(active), loaded(loaded), paused(paused), playing(playing), inGroup(inGroup),
      compactView(compactView), color(color), model(model)
{
    setupUi(this);

    this->animation = new ActiveAnimation(this->labelActiveColor);

    setThumbnail();
    setColor(this->color);
    setActive(this->active);
    setCompactView(this->compactView);

    this->command.setImageName(this->model.getName());

    this->labelGroupColor->setVisible(this->inGroup);
    this->labelGroupColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_GROUP_COLOR));
    this->labelColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_STILL_COLOR));

    this->labelLabel->setText(this->model.getLabel());
    this->labelChannel->setText(QString("Channel: %1").arg(this->command.getChannel()));
    this->labelVideolayer->setText(QString("Video layer: %1").arg(this->command.getVideolayer()));
    this->labelDelay->setText(QString("Delay: %1").arg(this->command.getDelay()));
    this->labelDevice->setText(QString("Server: %1").arg(this->model.getDeviceName()));

    this->executeTimer.setSingleShot(true);
    QObject::connect(&this->executeTimer, SIGNAL(timeout()), SLOT(executePlay()));

    QObject::connect(&this->command, SIGNAL(channelChanged(int)), this, SLOT(channelChanged(int)));
    QObject::connect(&this->command, SIGNAL(videolayerChanged(int)), this, SLOT(videolayerChanged(int)));
    QObject::connect(&this->command, SIGNAL(delayChanged(int)), this, SLOT(delayChanged(int)));
    QObject::connect(&this->command, SIGNAL(allowGpiChanged(bool)), this, SLOT(allowGpiChanged(bool)));

    QObject::connect(&DeviceManager::getInstance(), SIGNAL(deviceAdded(CasparDevice&)), this, SLOT(deviceAdded(CasparDevice&)));
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL)
        QObject::connect(device.data(), SIGNAL(connectionStateChanged(CasparDevice&)), this, SLOT(deviceConnectionStateChanged(CasparDevice&)));

    QObject::connect(GpiManager::getInstance().getGpiDevice().data(), SIGNAL(connectionStateChanged(bool, GpiDevice*)), this, SLOT(gpiConnectionStateChanged(bool, GpiDevice*)));

    checkEmptyDevice();
    checkGpiConnection();
    checkDeviceConnection();

    qApp->installEventFilter(this);
}

bool RundownImageWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Event::EventType::TargetChanged))
    {
        // This event is not for us.
        if (!this->active)
            return false;

        TargetChangedEvent* targetChangedEvent = dynamic_cast<TargetChangedEvent*>(event);
        this->model.setName(targetChangedEvent->getTarget());
        this->command.setImageName(targetChangedEvent->getTarget());

        setThumbnail();

        return true;
    }
    else if (event->type() == static_cast<QEvent::Type>(Event::EventType::LabelChanged))
    {
        // This event is not for us.
        if (!this->active)
            return false;

        LabelChangedEvent* labelChanged = dynamic_cast<LabelChangedEvent*>(event);
        this->model.setLabel(labelChanged->getLabel());

        this->labelLabel->setText(this->model.getLabel());

        return true;
    }
    else if (event->type() == static_cast<QEvent::Type>(Event::EventType::DeviceChanged))
    {
        // This event is not for us.
        if (!this->active)
            return false;

        // Should we update the device name?
        DeviceChangedEvent* deviceChangedEvent = dynamic_cast<DeviceChangedEvent*>(event);
        if (!deviceChangedEvent->getDeviceName().isEmpty() && deviceChangedEvent->getDeviceName() != this->model.getDeviceName())
        {
            // Disconnect connectionStateChanged() from the old device.
            const QSharedPointer<CasparDevice> oldDevice = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
            if (oldDevice != NULL)
                QObject::disconnect(oldDevice.data(), SIGNAL(connectionStateChanged(CasparDevice&)), this, SLOT(deviceConnectionStateChanged(CasparDevice&)));

            // Update the model with the new device.
            this->model.setDeviceName(deviceChangedEvent->getDeviceName());
            this->labelDevice->setText(QString("Server: %1").arg(this->model.getDeviceName()));

            // Connect connectionStateChanged() to the new device.
            const QSharedPointer<CasparDevice> newDevice = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
            if (newDevice != NULL)
                QObject::connect(newDevice.data(), SIGNAL(connectionStateChanged(CasparDevice&)), this, SLOT(deviceConnectionStateChanged(CasparDevice&)));
        }

        checkEmptyDevice();
        checkDeviceConnection();
    }

    return QObject::eventFilter(target, event);
}

AbstractRundownWidget* RundownImageWidget::clone()
{
    RundownImageWidget* widget = new RundownImageWidget(this->model, this->parentWidget(), this->color,
                                                                        this->active, this->loaded, this->paused, this->playing,
                                                                        this->inGroup, this->compactView);

    ImageCommand* command = dynamic_cast<ImageCommand*>(widget->getCommand());
    command->setChannel(this->command.getChannel());
    command->setVideolayer(this->command.getVideolayer());
    command->setDelay(this->command.getDelay());
    command->setAllowGpi(this->command.getAllowGpi());
    command->setImageName(this->command.getImageName());
    command->setTransition(this->command.getTransition());
    command->setDuration(this->command.getDuration());
    command->setTween(this->command.getTween());
    command->setDirection(this->command.getDirection());
    command->setUseAuto(this->command.getUseAuto());
    command->setTriggerOnNext(this->command.getTriggerOnNext());

    return widget;
}

void RundownImageWidget::setCompactView(bool compactView)
{
    if (compactView)
    {
        this->labelIcon->setFixedSize(Rundown::COMPACT_ICON_WIDTH, Rundown::COMPACT_ICON_HEIGHT);
        this->labelGpiConnected->setFixedSize(Rundown::COMPACT_ICON_WIDTH, Rundown::COMPACT_ICON_HEIGHT);
        this->labelDisconnected->setFixedSize(Rundown::COMPACT_ICON_WIDTH, Rundown::COMPACT_ICON_HEIGHT);
        this->labelThumbnail->setFixedSize(Rundown::COMPACT_THUMBNAIL_WIDTH, Rundown::COMPACT_THUMBNAIL_HEIGHT);
    }
    else
    {
        this->labelIcon->setFixedSize(Rundown::DEFAULT_ICON_WIDTH, Rundown::DEFAULT_ICON_HEIGHT);
        this->labelGpiConnected->setFixedSize(Rundown::DEFAULT_ICON_WIDTH, Rundown::DEFAULT_ICON_HEIGHT);
        this->labelDisconnected->setFixedSize(Rundown::DEFAULT_ICON_WIDTH, Rundown::DEFAULT_ICON_HEIGHT);
        this->labelThumbnail->setFixedSize(Rundown::DEFAULT_THUMBNAIL_WIDTH, Rundown::DEFAULT_THUMBNAIL_HEIGHT);
    }

    this->compactView = compactView;
}

void RundownImageWidget::readProperties(boost::property_tree::wptree& pt)
{
    if (pt.count(L"color") > 0) setColor(QString::fromStdWString(pt.get<std::wstring>(L"color")));
}

void RundownImageWidget::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("color", this->color);
}

bool RundownImageWidget::isGroup() const
{
    return false;
}

bool RundownImageWidget::isInGroup() const
{
    return this->inGroup;
}

AbstractCommand* RundownImageWidget::getCommand()
{
    return &this->command;
}

LibraryModel* RundownImageWidget::getLibraryModel()
{
    return &this->model;
}

void RundownImageWidget::setThumbnail()
{
    QString data = DatabaseManager::getInstance().getThumbnailById(this->model.getThumbnailId()).getData();
    if (data.isEmpty())
        data = DatabaseManager::getInstance().getThumbnailByNameAndDeviceName(this->model.getName(), this->model.getDeviceName()).getData();

    QImage image;
    image.loadFromData(QByteArray::fromBase64(data.toAscii()), "PNG");
    this->labelThumbnail->setPixmap(QPixmap::fromImage(image));

    bool displayThumbnailTooltip = (DatabaseManager::getInstance().getConfigurationByName("ShowThumbnailTooltip").getValue() == "true") ? true : false;
    if (displayThumbnailTooltip)
        this->labelThumbnail->setToolTip(QString("<img src=\"data:image/png;base64,%1 \"/>").arg(data));
}

void RundownImageWidget::setActive(bool active)
{
    this->active = active;

    this->animation->stop();

    if (this->active)
        this->labelActiveColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_ACTIVE_COLOR));
    else
        this->labelActiveColor->setStyleSheet("");
}

void RundownImageWidget::setInGroup(bool inGroup)
{
    this->inGroup = inGroup;
    this->labelGroupColor->setVisible(this->inGroup);
}

void RundownImageWidget::setColor(const QString& color)
{
    this->color = color;
    this->setStyleSheet(QString("#frameItem, #frameStatus { background-color: rgba(%1); }").arg(color));
}

void RundownImageWidget::checkEmptyDevice()
{
    if (this->labelDevice->text() == "Device: ")
        this->labelDevice->setStyleSheet("color: black;");
    else
        this->labelDevice->setStyleSheet("");
}

bool RundownImageWidget::executeCommand(enum Playout::PlayoutType::Type type)
{
    if (type == Playout::PlayoutType::Stop)
        QTimer::singleShot(0, this, SLOT(executeStop()));
    else if (type == Playout::PlayoutType::Play && !this->command.getTriggerOnNext())
    {
        if (!this->model.getDeviceName().isEmpty()) // The user need to select a device.
        {
            const QStringList& channelFormats = DatabaseManager::getInstance().getDeviceByName(this->model.getDeviceName()).getChannelFormats().split(",");
            double framesPerSecond = DatabaseManager::getInstance().getFormat(channelFormats[this->command.getChannel() - 1]).getFramesPerSecond().toDouble();

            this->executeTimer.setInterval(floor(this->command.getDelay() * (1000 / framesPerSecond)));
            this->executeTimer.start();
        }
    }
    else if (type == Playout::PlayoutType::Next && this->command.getTriggerOnNext())
        QTimer::singleShot(0, this, SLOT(executePlay()));
    else if (type == Playout::PlayoutType::Pause)
        QTimer::singleShot(0, this, SLOT(executePause()));
    else if (type == Playout::PlayoutType::Load)
        QTimer::singleShot(0, this, SLOT(executeLoad()));
    else if (type == Playout::PlayoutType::Clear)
        QTimer::singleShot(0, this, SLOT(executeClearVideolayer()));
    else if (type == Playout::PlayoutType::ClearVideolayer)
        QTimer::singleShot(0, this, SLOT(executeClearVideolayer()));
    else if (type == Playout::PlayoutType::ClearChannel)
        QTimer::singleShot(0, this, SLOT(executeClearChannel()));

    if (this->active)
        this->animation->start(1);

    return true;
}

void RundownImageWidget::executeStop()
{
    this->executeTimer.stop();

    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
        device->stopImage(this->command.getChannel(), this->command.getVideolayer());

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
            deviceShadow->stopImage(this->command.getChannel(), this->command.getVideolayer());
    }

    this->paused = false;
    this->loaded = false;
    this->playing = false;
}

void RundownImageWidget::executePlay()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
    {
        if (this->loaded)
            device->playImage(this->command.getChannel(), this->command.getVideolayer());
        else
            device->playImage(this->command.getChannel(), this->command.getVideolayer(), this->command.getImageName(),
                              this->command.getTransition(), this->command.getDuration(), this->command.getTween(),
                              this->command.getDirection(), this->command.getUseAuto());
    }

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice>  deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
        {
            if (this->loaded)
                deviceShadow->playImage(this->command.getChannel(), this->command.getVideolayer());
            else
                deviceShadow->playImage(this->command.getChannel(), this->command.getVideolayer(), this->command.getImageName(),
                                        this->command.getTransition(), this->command.getDuration(), this->command.getTween(),
                                        this->command.getDirection(), this->command.getUseAuto());
        }
    }

    this->paused = false;
    this->loaded = false;
    this->playing = true;
}

void RundownImageWidget::executePause()
{
    if (!this->playing)
        return;

    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
    {
        if (this->paused)
            device->playImage(this->command.getChannel(), this->command.getVideolayer());
        else
            device->pauseImage(this->command.getChannel(), this->command.getVideolayer());
    }

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
        {
            if (this->paused)
                deviceShadow->playImage(this->command.getChannel(), this->command.getVideolayer());
            else
                deviceShadow->pauseImage(this->command.getChannel(), this->command.getVideolayer());
        }
    }

    this->paused = !this->paused;
}

void RundownImageWidget::executeLoad()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
    {
        device->loadImage(this->command.getChannel(), this->command.getVideolayer(), this->command.getImageName(),
                          this->command.getTransition(), this->command.getDuration(), this->command.getTween(),
                          this->command.getDirection(), this->command.getUseAuto());
    }

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice>  deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
        {
            deviceShadow->loadImage(this->command.getChannel(), this->command.getVideolayer(), this->command.getImageName(),
                                    this->command.getTransition(), this->command.getDuration(), this->command.getTween(),
                                    this->command.getDirection(), this->command.getUseAuto());
        }
    }

    this->loaded = true;
    this->paused = false;
    this->playing = false;
}

void RundownImageWidget::executeClearVideolayer()
{
    this->executeTimer.stop();

    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
        device->clearVideolayer(this->command.getChannel(), this->command.getVideolayer());

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
            deviceShadow->clearVideolayer(this->command.getChannel(), this->command.getVideolayer());
    }

    this->paused = false;
    this->loaded = false;
    this->playing = false;
}

void RundownImageWidget::executeClearChannel()
{
    this->executeTimer.stop();

    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
    {
        device->clearChannel(this->command.getChannel());
        device->clearMixerChannel(this->command.getChannel());
    }

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
        {
            deviceShadow->clearChannel(this->command.getChannel());
            deviceShadow->clearMixerChannel(this->command.getChannel());
        }
    }

    this->paused = false;
    this->loaded = false;
    this->playing = false;
}

void RundownImageWidget::channelChanged(int channel)
{
    this->labelChannel->setText(QString("Channel: %1").arg(channel));
}

void RundownImageWidget::videolayerChanged(int videolayer)
{
    this->labelVideolayer->setText(QString("Video layer: %1").arg(videolayer));
}

void RundownImageWidget::delayChanged(int delay)
{
    this->labelDelay->setText(QString("Delay: %1").arg(delay));
}

void RundownImageWidget::checkGpiConnection()
{
    this->labelGpiConnected->setVisible(this->command.getAllowGpi());

    if (GpiManager::getInstance().getGpiDevice()->isConnected())
        this->labelGpiConnected->setPixmap(QPixmap(":/Graphics/Images/GpiConnected.png"));
    else
        this->labelGpiConnected->setPixmap(QPixmap(":/Graphics/Images/GpiDisconnected.png"));
}

void RundownImageWidget::checkDeviceConnection()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device == NULL)
        this->labelDisconnected->setVisible(true);
    else
        this->labelDisconnected->setVisible(!device->isConnected());
}

void RundownImageWidget::allowGpiChanged(bool allowGpi)
{
    checkGpiConnection();
}

void RundownImageWidget::gpiConnectionStateChanged(bool connected, GpiDevice* device)
{
    checkGpiConnection();
}

void RundownImageWidget::deviceConnectionStateChanged(CasparDevice& device)
{
    checkDeviceConnection();
}

void RundownImageWidget::deviceAdded(CasparDevice& device)
{
    if (DeviceManager::getInstance().getDeviceModelByAddress(device.getAddress()).getName() == this->model.getDeviceName())
        QObject::connect(&device, SIGNAL(connectionStateChanged(CasparDevice&)), this, SLOT(deviceConnectionStateChanged(CasparDevice&)));

    checkDeviceConnection();
}
