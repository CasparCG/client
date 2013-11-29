#include "RundownAutoWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "DeviceManager.h"
#include "GpiManager.h"
#include "Events/ConnectionStateChangedEvent.h"
#include "Events/Inspector/LabelChangedEvent.h"
#include "Events/Inspector/DeviceChangedEvent.h"

#include <math.h>

#include <QtCore/QObject>
#include <QtCore/QTimer>

RundownAutoWidget::RundownAutoWidget(const LibraryModel& model, QWidget* parent, const QString& color, bool active,
                                     bool inGroup, bool compactView)
    : QWidget(parent),
      active(active), inGroup(inGroup), compactView(compactView), color(color), model(model)
{
    setupUi(this);

    this->animation = new ActiveAnimation(this->labelActiveColor);

    setColor(color);
    setActive(active);
    setCompactView(compactView);

    this->labelGroupColor->setVisible(this->inGroup);
    this->labelGroupColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_GROUP_COLOR));
    this->labelColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_TRICASTER_COLOR));

    this->labelLabel->setText(this->model.getLabel());
    this->labelDelay->setText(QString("Delay: %1").arg(this->command.getDelay()));
    this->labelDevice->setText(QString("Server: %1").arg(this->model.getDeviceName()));

    this->executeTimer.setSingleShot(true);
    QObject::connect(&this->executeTimer, SIGNAL(timeout()), SLOT(executePlay()));

    QObject::connect(&this->command, SIGNAL(channelChanged(int)), this, SLOT(channelChanged(int)));
    QObject::connect(&this->command, SIGNAL(videolayerChanged(int)), this, SLOT(videolayerChanged(int)));
    QObject::connect(&this->command, SIGNAL(delayChanged(int)), this, SLOT(delayChanged(int)));
    QObject::connect(&this->command, SIGNAL(allowGpiChanged(bool)), this, SLOT(allowGpiChanged(bool)));

    /*
    QObject::connect(&DeviceManager::getInstance(), SIGNAL(deviceAdded(CasparDevice&)), this, SLOT(deviceAdded(CasparDevice&)));
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL)
        QObject::connect(device.data(), SIGNAL(connectionStateChanged(CasparDevice&)), this, SLOT(deviceConnectionStateChanged(CasparDevice&)));
    */

    QObject::connect(GpiManager::getInstance().getGpiDevice().data(), SIGNAL(connectionStateChanged(bool, GpiDevice*)), this, SLOT(gpiConnectionStateChanged(bool, GpiDevice*)));

    checkEmptyDevice();
    checkGpiConnection();
    checkDeviceConnection();

    qApp->installEventFilter(this);
}

bool RundownAutoWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Event::EventType::Preview))
    {
        // This event is not for us.
        if (!this->active)
            return false;

        executePlay();

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
        /*
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
        checkDeviceConnection();*/
    }

    return QObject::eventFilter(target, event);
}

AbstractRundownWidget* RundownAutoWidget::clone()
{
    RundownAutoWidget* widget = new RundownAutoWidget(this->model, this->parentWidget(), this->color, this->active,
                                                      this->inGroup, this->compactView);

    AutoCommand* command = dynamic_cast<AutoCommand*>(widget->getCommand());
    command->setDelay(this->command.getDelay());
    command->setAllowGpi(this->command.getAllowGpi());
    command->setStep(this->command.getStep());

    return widget;
}

void RundownAutoWidget::setCompactView(bool compactView)
{
    if (compactView)
    {
        this->labelIcon->setFixedSize(Rundown::COMPACT_ICON_WIDTH, Rundown::COMPACT_ICON_HEIGHT);
        this->labelGpiConnected->setFixedSize(Rundown::COMPACT_ICON_WIDTH, Rundown::COMPACT_ICON_HEIGHT);
        this->labelDisconnected->setFixedSize(Rundown::COMPACT_ICON_WIDTH, Rundown::COMPACT_ICON_HEIGHT);
    }
    else
    {
        this->labelIcon->setFixedSize(Rundown::DEFAULT_ICON_WIDTH, Rundown::DEFAULT_ICON_HEIGHT);
        this->labelGpiConnected->setFixedSize(Rundown::DEFAULT_ICON_WIDTH, Rundown::DEFAULT_ICON_HEIGHT);
        this->labelDisconnected->setFixedSize(Rundown::DEFAULT_ICON_WIDTH, Rundown::DEFAULT_ICON_HEIGHT);
    }

    this->compactView = compactView;
}

void RundownAutoWidget::readProperties(boost::property_tree::wptree& pt)
{
    if (pt.count(L"color") > 0) setColor(QString::fromStdWString(pt.get<std::wstring>(L"color")));
}

void RundownAutoWidget::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("color", this->color);
}

bool RundownAutoWidget::isGroup() const
{
    return false;
}

bool RundownAutoWidget::isInGroup() const
{
    return this->inGroup;
}

AbstractCommand* RundownAutoWidget::getCommand()
{
    return &this->command;
}

LibraryModel* RundownAutoWidget::getLibraryModel()
{
    return &this->model;
}

void RundownAutoWidget::setActive(bool active)
{
    this->active = active;

    this->animation->stop();

    if (this->active)
        this->labelActiveColor->setStyleSheet("background-color: lime;");
    else
        this->labelActiveColor->setStyleSheet("");
}

void RundownAutoWidget::setInGroup(bool inGroup)
{
    this->inGroup = inGroup;
    this->labelGroupColor->setVisible(this->inGroup);
}

void RundownAutoWidget::setColor(const QString& color)
{
    this->color = color;
    this->setStyleSheet(QString("#frameItem, #frameStatus { background-color: rgba(%1); }").arg(color));
}

void RundownAutoWidget::checkEmptyDevice()
{
    if (this->labelDevice->text() == "Device: ")
        this->labelDevice->setStyleSheet("color: black;");
    else
        this->labelDevice->setStyleSheet("");
}

bool RundownAutoWidget::executeCommand(enum Playout::PlayoutType::Type type)
{
    if (type == Playout::PlayoutType::Play || type == Playout::PlayoutType::Update)
    {       
        //if (!this->model.getDeviceName().isEmpty()) // The user need to select a device.
        //{
            QTimer::singleShot(this->command.getDelay(), this, SLOT(executePlay()));
        //}
    }

    if (this->active)
        this->animation->start(1);

    return true;
}

void RundownAutoWidget::executePlay()
{
    DeviceManager::getInstance().getTriCasterDevice()->triggerAuto(this->command.getStep());
}

void RundownAutoWidget::delayChanged(int delay)
{
    this->labelDelay->setText(QString("Delay: %1").arg(delay));
}

void RundownAutoWidget::checkGpiConnection()
{
    this->labelGpiConnected->setVisible(this->command.getAllowGpi());

    if (GpiManager::getInstance().getGpiDevice()->isConnected())
        this->labelGpiConnected->setPixmap(QPixmap(":/Graphics/Images/GpiConnected.png"));
    else
        this->labelGpiConnected->setPixmap(QPixmap(":/Graphics/Images/GpiDisconnected.png"));
}

void RundownAutoWidget::checkDeviceConnection()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device == NULL)
        this->labelDisconnected->setVisible(true);
    else
        this->labelDisconnected->setVisible(!device->isConnected());
}

void RundownAutoWidget::allowGpiChanged(bool allowGpi)
{
    checkGpiConnection();
}

void RundownAutoWidget::gpiConnectionStateChanged(bool connected, GpiDevice* device)
{
    checkGpiConnection();
}
/*
void RundownTriCasterWidget::deviceConnectionStateChanged(CasparDevice& device)
{
    checkDeviceConnection();
}

void RundownTriCasterWidget::deviceAdded(CasparDevice& device)
{
    if (DeviceManager::getInstance().getDeviceModelByAddress(device.getAddress()).getName() == this->model.getDeviceName())
        QObject::connect(&device, SIGNAL(connectionStateChanged(CasparDevice&)), this, SLOT(deviceConnectionStateChanged(CasparDevice&)));

    checkDeviceConnection();
}
*/
