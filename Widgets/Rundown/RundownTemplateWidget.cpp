#include "RundownTemplateWidget.h"

#include "Global.h"

#include "DeviceManager.h"
#include "GpiManager.h"
#include "Events/ConnectionStateChangedEvent.h"
#include "Events/RundownItemChangedEvent.h"

#include <QtCore/QObject>
#include <QtCore/QTimer>

RundownTemplateWidget::RundownTemplateWidget(const LibraryModel& model, QWidget* parent, const QString& color,
                                             bool active, bool loaded, bool inGroup, bool disconnected, bool compactView)
    : QWidget(parent),
    active(active), loaded(loaded), inGroup(inGroup), disconnected(disconnected), compactView(compactView), color(color), model(model)
{
    setupUi(this);

    setActive(active);
    setCompactView(compactView);

    this->command.setTemplateName(this->model.getName());

    this->labelDisconnected->setVisible(this->disconnected);
    this->labelGroupColor->setVisible(this->inGroup);
    this->labelGroupColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_GROUP_COLOR));
    this->labelColor->setStyleSheet(QString("background-color: %1;").arg(color));

    this->labelLabel->setText(this->model.getLabel());
    this->labelChannel->setText(QString("Channel: %1").arg(this->command.getChannel()));
    this->labelVideolayer->setText(QString("Video layer: %1").arg(this->command.getVideolayer()));
    this->labelDelay->setText(QString("Delay: %1").arg(this->command.getDelay()));
    this->labelFlashlayer->setText(QString("Flash layer: %1").arg(this->command.getFlashlayer()));
    this->labelDevice->setText(QString("Device: %1").arg(this->model.getDeviceName()));

    QObject::connect(&this->command, SIGNAL(channelChanged(int)), this, SLOT(channelChanged(int)));
    QObject::connect(&this->command, SIGNAL(videolayerChanged(int)), this, SLOT(videolayerChanged(int)));
    QObject::connect(&this->command, SIGNAL(delayChanged(int)), this, SLOT(delayChanged(int)));
    QObject::connect(&this->command, SIGNAL(flashlayerChanged(int)), this, SLOT(flashlayerChanged(int)));
    QObject::connect(&this->command, SIGNAL(allowGpiChanged(bool)), this, SLOT(allowGpiChanged(bool)));
    QObject::connect(GpiManager::getInstance().getGpiDevice().data(), SIGNAL(connectionStateChanged(bool, GpiDevice*)),
                     this, SLOT(gpiDeviceConnected(bool, GpiDevice*)));

    checkEmptyDevice();
    checkGpiTriggerable();

    qApp->installEventFilter(this);
}

bool RundownTemplateWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::ConnectionStateChanged))
    {
        ConnectionStateChangedEvent* connectionStateChangedEvent = dynamic_cast<ConnectionStateChangedEvent*>(event);
        if (connectionStateChangedEvent->getDeviceName() == this->model.getDeviceName())
        {
            this->disconnected = !connectionStateChangedEvent->getConnected();

            if (connectionStateChangedEvent->getConnected())
                this->labelDisconnected->setVisible(false);
            else
                this->labelDisconnected->setVisible(true);
        }
    }
    else if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemChanged))
    {
        // This event is not for us.
        if (!this->active)
            return false;

        RundownItemChangedEvent* rundownItemChangedEvent = dynamic_cast<RundownItemChangedEvent*>(event);
        this->model.setLabel(rundownItemChangedEvent->getLabel());
        this->model.setDeviceName(rundownItemChangedEvent->getDeviceName());
        this->model.setName(rundownItemChangedEvent->getName());
        this->command.setTemplateName(rundownItemChangedEvent->getName());

        this->labelLabel->setText(this->model.getLabel());
        this->labelDevice->setText(QString("Device: %1").arg(this->model.getDeviceName()));

        checkEmptyDevice();
    }

    return QObject::eventFilter(target, event);
}

IRundownWidget* RundownTemplateWidget::clone()
{
    RundownTemplateWidget* widget = new RundownTemplateWidget(this->model, this->parentWidget(), this->color,
                                                              this->active, this->loaded, this->inGroup,
                                                              this->disconnected, this->compactView);

    TemplateCommand* command = dynamic_cast<TemplateCommand*>(widget->getCommand());
    command->setChannel(this->command.getChannel());
    command->setVideolayer(this->command.getVideolayer());
    command->setDelay(this->command.getDelay());
    command->setAllowGpi(this->command.getAllowGpi());
    command->setFlashlayer(this->command.getFlashlayer());
    command->setInvoke(this->command.getInvoke());
    command->setTemplateName(this->command.getTemplateName());
    command->setTemplateDataModels(this->command.getTemplateDataModels());
    command->setUseStoredData(this->command.getUseStoredData());

    return widget;
}

void RundownTemplateWidget::setCompactView(bool compactView)
{
    if (compactView)
    {
        this->labelThumbnail->setFixedSize(Define::COMPACT_VIEW_WIDTH, Define::COMPACT_VIEW_HEIGHT);
        this->labelGpiConnected->setFixedSize(Define::COMPACT_VIEW_WIDTH, Define::COMPACT_VIEW_HEIGHT);
        this->labelDisconnected->setFixedSize(Define::COMPACT_VIEW_WIDTH, Define::COMPACT_VIEW_HEIGHT);
    }
    else
    {
        this->labelThumbnail->setFixedSize(Define::DEFAULT_VIEW_WIDTH, Define::DEFAULT_VIEW_HEIGHT);
        this->labelGpiConnected->setFixedSize(Define::DEFAULT_VIEW_WIDTH, Define::DEFAULT_VIEW_HEIGHT);
        this->labelDisconnected->setFixedSize(Define::DEFAULT_VIEW_WIDTH, Define::DEFAULT_VIEW_HEIGHT);
    }

    this->compactView = compactView;
}

void RundownTemplateWidget::readProperties(boost::property_tree::wptree& pt)
{
    setColor(QString::fromStdWString(pt.get<std::wstring>(L"color")));
}

void RundownTemplateWidget::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("color", this->color);
}

bool RundownTemplateWidget::isGroup() const
{
    return false;
}

ICommand* RundownTemplateWidget::getCommand()
{
    return &this->command;
}

LibraryModel* RundownTemplateWidget::getLibraryModel()
{
    return &this->model;
}

void RundownTemplateWidget::setActive(bool active)
{
    this->active = active;

    if (this->active)
        this->labelActiveColor->setStyleSheet("background-color: red;");
    else
        this->labelActiveColor->setStyleSheet("");
}

void RundownTemplateWidget::setInGroup(bool inGroup)
{
    this->inGroup = inGroup;
    this->labelGroupColor->setVisible(inGroup);

    if (this->inGroup)
    {
        this->labelChannel->setGeometry(this->labelChannel->geometry().x() + Define::GROUP_XPOS_OFFSET,
                                        this->labelChannel->geometry().y(),
                                        this->labelChannel->geometry().width(),
                                        this->labelChannel->geometry().height());
        this->labelVideolayer->setGeometry(this->labelVideolayer->geometry().x() + Define::GROUP_XPOS_OFFSET,
                                           this->labelVideolayer->geometry().y(),
                                           this->labelVideolayer->geometry().width(),
                                           this->labelVideolayer->geometry().height());
        this->labelDelay->setGeometry(this->labelDelay->geometry().x() + Define::GROUP_XPOS_OFFSET,
                                      this->labelDelay->geometry().y(),
                                      this->labelDelay->geometry().width(),
                                      this->labelDelay->geometry().height());
        this->labelFlashlayer->setGeometry(this->labelFlashlayer->geometry().x() + Define::GROUP_XPOS_OFFSET,
                                           this->labelFlashlayer->geometry().y(),
                                           this->labelFlashlayer->geometry().width(),
                                           this->labelFlashlayer->geometry().height());
        this->labelDevice->setGeometry(this->labelDevice->geometry().x() + Define::GROUP_XPOS_OFFSET,
                                       this->labelDevice->geometry().y(),
                                       this->labelDevice->geometry().width(),
                                       this->labelDevice->geometry().height());
    }
}

void RundownTemplateWidget::setColor(const QString& color)
{
    this->color = color;
    this->labelColor->setStyleSheet(QString("background-color: %1;").arg(color));
}

void RundownTemplateWidget::checkEmptyDevice()
{
    if (this->labelDevice->text() == "Device: ")
        this->labelDevice->setStyleSheet("color: black;");
    else
        this->labelDevice->setStyleSheet("");
}

bool RundownTemplateWidget::executeCommand(enum Playout::PlayoutType::Type type)
{
    if (type == Playout::PlayoutType::Stop)
        QTimer::singleShot(0, this, SLOT(executeStop()));
    else if (type == Playout::PlayoutType::Play)
        QTimer::singleShot(this->command.getDelay(), this, SLOT(executePlay()));
    else if (type == Playout::PlayoutType::Load)
        QTimer::singleShot(0, this, SLOT(executeLoad()));
    else if (type == Playout::PlayoutType::Next)
        QTimer::singleShot(0, this, SLOT(executeNext()));
    else if (type == Playout::PlayoutType::Update)
        QTimer::singleShot(0, this, SLOT(executeUpdate()));
    else if (type == Playout::PlayoutType::Invoke)
        QTimer::singleShot(0, this, SLOT(executeInvoke()));
    else if (type == Playout::PlayoutType::Clear)
        QTimer::singleShot(0, this, SLOT(executeClear()));
    else if (type == Playout::PlayoutType::ClearVideolayer)
        QTimer::singleShot(0, this, SLOT(executeClearVideolayer()));
    else if (type == Playout::PlayoutType::ClearChannel)
        QTimer::singleShot(0, this, SLOT(executeClearChannel()));

    return true;
}

void RundownTemplateWidget::executeStop()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getConnectionByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
        device->stopTemplate(this->command.getChannel(), this->command.getVideolayer(), this->command.getFlashlayer());

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getConnectionByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
            deviceShadow->stopTemplate(this->command.getChannel(), this->command.getVideolayer(), this->command.getFlashlayer());
    }

    this->loaded = false;
}

void RundownTemplateWidget::executePlay()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getConnectionByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
    {
        if (this->loaded)
        {
            device->playTemplate(this->command.getChannel(), this->command.getVideolayer(), this->command.getFlashlayer());
        }
        else
        {
            if (this->command.getTemplateData().isEmpty())
                device->playTemplate(this->command.getChannel(), this->command.getVideolayer(), this->command.getFlashlayer(),
                                     this->command.getTemplateName());
            else
                device->playTemplate(this->command.getChannel(), this->command.getVideolayer(), this->command.getFlashlayer(),
                                     this->command.getTemplateName(), this->command.getTemplateData());
        }
    }

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice>  deviceShadow = DeviceManager::getInstance().getConnectionByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
        {
            if (this->loaded)
            {
                deviceShadow->playTemplate(this->command.getChannel(), this->command.getVideolayer(), this->command.getFlashlayer());
            }
            else
            {
                if (this->command.getTemplateData().isEmpty())
                    deviceShadow->playTemplate(this->command.getChannel(), this->command.getVideolayer(), this->command.getFlashlayer(),
                                               this->command.getTemplateName());
                else
                    deviceShadow->playTemplate(this->command.getChannel(), this->command.getVideolayer(), this->command.getFlashlayer(),
                                               this->command.getTemplateName(), this->command.getTemplateData());
            }
        }
    }

    this->loaded = false;
}

void RundownTemplateWidget::executeLoad()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getConnectionByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
    {
        if (this->command.getTemplateData().isEmpty())
            device->addTemplate(this->command.getChannel(), this->command.getVideolayer(), this->command.getFlashlayer(),
                                this->command.getTemplateName(), false);
        else
            device->addTemplate(this->command.getChannel(), this->command.getVideolayer(), this->command.getFlashlayer(),
                                this->command.getTemplateName(), false, this->command.getTemplateData());
    }

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice>  deviceShadow = DeviceManager::getInstance().getConnectionByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
        {
            if (this->command.getTemplateData().isEmpty())
                deviceShadow->addTemplate(this->command.getChannel(), this->command.getVideolayer(), this->command.getFlashlayer(),
                                          this->command.getTemplateName(), false);
            else
                deviceShadow->addTemplate(this->command.getChannel(), this->command.getVideolayer(), this->command.getFlashlayer(),
                                          this->command.getTemplateName(), false, this->command.getTemplateData());
        }
    }

    this->loaded = true;
}

void RundownTemplateWidget::executeNext()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getConnectionByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
        device->nextTemplate(this->command.getChannel(), this->command.getVideolayer(), this->command.getFlashlayer());

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getConnectionByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
            deviceShadow->nextTemplate(this->command.getChannel(), this->command.getVideolayer(), this->command.getFlashlayer());
    }
}

void RundownTemplateWidget::executeUpdate()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getConnectionByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
    {
        if (this->command.getTemplateData().isEmpty())
            return;

        device->updateTemplate(this->command.getChannel(), this->command.getVideolayer(),
                               this->command.getFlashlayer(), this->command.getTemplateData());
    }

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getConnectionByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
        {
            if (this->command.getTemplateData().isEmpty())
                return;

            deviceShadow->updateTemplate(this->command.getChannel(), this->command.getVideolayer(),
                                         this->command.getFlashlayer(), this->command.getTemplateData());
        }
    }
}

void RundownTemplateWidget::executeInvoke()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getConnectionByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
        device->invokeTemplate(this->command.getChannel(), this->command.getVideolayer(),
                               this->command.getFlashlayer(), this->command.getInvoke());

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getConnectionByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
            deviceShadow->invokeTemplate(this->command.getChannel(), this->command.getVideolayer(),
                                         this->command.getFlashlayer(), this->command.getInvoke());
    }
}

void RundownTemplateWidget::executeClear()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getConnectionByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
        device->removeTemplate(this->command.getChannel(), this->command.getVideolayer(), this->command.getFlashlayer());

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getConnectionByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
            deviceShadow->removeTemplate(this->command.getChannel(), this->command.getVideolayer(), this->command.getFlashlayer());
    }

    this->loaded = false;
}

void RundownTemplateWidget::executeClearVideolayer()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getConnectionByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
        device->clearVideolayer(this->command.getChannel(), this->command.getVideolayer());

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getConnectionByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
            deviceShadow->clearVideolayer(this->command.getChannel(), this->command.getVideolayer());
    }

    this->loaded = false;
}

void RundownTemplateWidget::executeClearChannel()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getConnectionByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
    {
        device->clearChannel(this->command.getChannel());
        device->clearMixerChannel(this->command.getChannel());
    }

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getConnectionByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
        {
            deviceShadow->clearChannel(this->command.getChannel());
            deviceShadow->clearMixerChannel(this->command.getChannel());
        }
    }

    this->loaded = false;
}

void RundownTemplateWidget::channelChanged(int channel)
{
    this->labelChannel->setText(QString("Channel: %1").arg(channel));
}

void RundownTemplateWidget::videolayerChanged(int videolayer)
{
    this->labelVideolayer->setText(QString("Video layer: %1").arg(videolayer));
}

void RundownTemplateWidget::delayChanged(int delay)
{
    this->labelDelay->setText(QString("Delay: %1").arg(delay));
}

void RundownTemplateWidget::flashlayerChanged(int flashlayer)
{
    this->labelFlashlayer->setText(QString("Flash layer: %1").arg(flashlayer));
}

void RundownTemplateWidget::checkGpiTriggerable()
{
    labelGpiConnected->setVisible(this->command.getAllowGpi());

    if (GpiManager::getInstance().getGpiDevice()->isConnected())
        labelGpiConnected->setPixmap(QPixmap(":/Graphics/Images/GpiConnected.png"));
    else
        labelGpiConnected->setPixmap(QPixmap(":/Graphics/Images/GpiDisconnected.png"));
}

void RundownTemplateWidget::allowGpiChanged(bool allowGpi)
{
    checkGpiTriggerable();
}

void RundownTemplateWidget::gpiDeviceConnected(bool connected, GpiDevice* device)
{
    checkGpiTriggerable();
}
