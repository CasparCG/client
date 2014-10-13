#include "RundownPlayoutCommandWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "DeviceManager.h"
#include "GpiManager.h"
#include "EventManager.h"
#include "Events/ConnectionStateChangedEvent.h"

#include <math.h>

#include <QtCore/QObject>
#include <QtCore/QTimer>

#include <QtGui/QGraphicsOpacityEffect>

RundownPlayoutCommandWidget::RundownPlayoutCommandWidget(const LibraryModel& model, QWidget* parent, const QString& color, bool active,
                                                         bool inGroup, bool compactView)
    : QWidget(parent),
      active(active), inGroup(inGroup), compactView(compactView), color(color), model(model), playControlSubscription(NULL),
      playNowControlSubscription(NULL)
{
    setupUi(this);

    this->animation = new ActiveAnimation(this->labelActiveColor);

    this->delayType = DatabaseManager::getInstance().getConfigurationByName("DelayType").getValue();
    this->markUsedItems = (DatabaseManager::getInstance().getConfigurationByName("MarkUsedItems").getValue() == "true") ? true : false;

    setColor(this->color);
    setActive(this->active);
    setCompactView(this->compactView);

    this->labelGroupColor->setVisible(this->inGroup);
    this->labelGroupColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_GROUP_COLOR));
    this->labelColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_MIXER_COLOR));

    this->labelLabel->setText(this->model.getLabel());
    this->labelDelay->setText(QString("Delay: %1").arg(this->command.getDelay()));

    this->executeTimer.setSingleShot(true);
    QObject::connect(&this->executeTimer, SIGNAL(timeout()), SLOT(executePlay()));

    QObject::connect(&this->command, SIGNAL(delayChanged(int)), this, SLOT(delayChanged(int)));
    QObject::connect(&this->command, SIGNAL(allowGpiChanged(bool)), this, SLOT(allowGpiChanged(bool)));
    QObject::connect(&this->command, SIGNAL(remoteTriggerIdChanged(const QString&)), this, SLOT(remoteTriggerIdChanged(const QString&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(labelChanged(const LabelChangedEvent&)), this, SLOT(labelChanged(const LabelChangedEvent&)));

    QObject::connect(GpiManager::getInstance().getGpiDevice().data(), SIGNAL(connectionStateChanged(bool, GpiDevice*)), this, SLOT(gpiConnectionStateChanged(bool, GpiDevice*)));

    checkGpiConnection();
}

void RundownPlayoutCommandWidget::labelChanged(const LabelChangedEvent& event)
{
    // This event is not for us.
    if (!this->active)
        return;

    this->model.setLabel(event.getLabel());

    this->labelLabel->setText(this->model.getLabel());
}

AbstractRundownWidget* RundownPlayoutCommandWidget::clone()
{
    RundownPlayoutCommandWidget* widget = new RundownPlayoutCommandWidget(this->model, this->parentWidget(), this->color, this->active,
                                                                          this->inGroup, this->compactView);

    PlayoutCommand* command = dynamic_cast<PlayoutCommand*>(widget->getCommand());
    command->setChannel(this->command.getChannel());
    command->setVideolayer(this->command.getVideolayer());
    command->setDelay(this->command.getDelay());
    command->setDuration(this->command.getDuration());
    command->setAllowGpi(this->command.getAllowGpi());
    command->setAllowRemoteTriggering(this->command.getAllowRemoteTriggering());
    command->setRemoteTriggerId(this->command.getRemoteTriggerId());
    command->setPlayoutCommand(this->command.getPlayoutCommand());

    return widget;
}

void RundownPlayoutCommandWidget::setCompactView(bool compactView)
{
    if (compactView)
    {
        this->labelIcon->setFixedSize(Rundown::COMPACT_ICON_WIDTH, Rundown::COMPACT_ICON_HEIGHT);
        this->labelGpiConnected->setFixedSize(Rundown::COMPACT_ICON_WIDTH, Rundown::COMPACT_ICON_HEIGHT);
    }
    else
    {
        this->labelIcon->setFixedSize(Rundown::DEFAULT_ICON_WIDTH, Rundown::DEFAULT_ICON_HEIGHT);
        this->labelGpiConnected->setFixedSize(Rundown::DEFAULT_ICON_WIDTH, Rundown::DEFAULT_ICON_HEIGHT);
    }

    this->compactView = compactView;
}

void RundownPlayoutCommandWidget::readProperties(boost::property_tree::wptree& pt)
{
    if (pt.count(L"color") > 0) setColor(QString::fromStdWString(pt.get<std::wstring>(L"color")));
}

void RundownPlayoutCommandWidget::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("color", this->color);
}

bool RundownPlayoutCommandWidget::isGroup() const
{
    return false;
}

bool RundownPlayoutCommandWidget::isInGroup() const
{
    return this->inGroup;
}

AbstractCommand* RundownPlayoutCommandWidget::getCommand()
{
    return &this->command;
}

LibraryModel* RundownPlayoutCommandWidget::getLibraryModel()
{
    return &this->model;
}

void RundownPlayoutCommandWidget::setActive(bool active)
{
    if (this->active == active)
        return;

    this->active = active;

    this->animation->stop();

    if (this->active)
        this->labelActiveColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_ACTIVE_COLOR));
    else
        this->labelActiveColor->setStyleSheet("");
}

void RundownPlayoutCommandWidget::setInGroup(bool inGroup)
{
    this->inGroup = inGroup;
    this->labelGroupColor->setVisible(this->inGroup);
}

QString RundownPlayoutCommandWidget::getColor() const
{
    return this->color;
}

void RundownPlayoutCommandWidget::setColor(const QString& color)
{
    this->color = color;
    this->setStyleSheet(QString("#frameItem, #frameStatus { background-color: %1; }").arg(color));
}

void RundownPlayoutCommandWidget::clearDelayedCommands()
{
    this->executeTimer.stop();
}

void RundownPlayoutCommandWidget::setUsed(bool used)
{
    if (used)
    {
        if (this->graphicsEffect() == NULL)
        {
            QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(this);
            effect->setOpacity(0.25);

            this->setGraphicsEffect(effect);
        }
    }
    else
        this->setGraphicsEffect(NULL);
}

bool RundownPlayoutCommandWidget::executeCommand(Playout::PlayoutType::Type type)
{
    if (type == Playout::PlayoutType::Play)
    {
        if (this->command.getDelay() < 0)
            return true;

        this->executeTimer.setInterval(this->command.getDelay());
        this->executeTimer.start();
    }
    else if (type == Playout::PlayoutType::PlayNow)
        executePlay();

    if (this->active)
        this->animation->start(1);

    return true;
}

void RundownPlayoutCommandWidget::executePlay()
{
    if (this->command.getPlayoutCommand() == Playout::toString(Playout::PlayoutType::Stop))
        EventManager::getInstance().fireExecutePlayoutCommandEvent(ExecutePlayoutCommandEvent(QEvent::KeyPress, Qt::Key_F1, Qt::NoModifier));
    else if (this->command.getPlayoutCommand() == Playout::toString(Playout::PlayoutType::Play))
        EventManager::getInstance().fireExecutePlayoutCommandEvent(ExecutePlayoutCommandEvent(QEvent::KeyPress, Qt::Key_F2, Qt::NoModifier));
    else if (this->command.getPlayoutCommand() == Playout::toString(Playout::PlayoutType::PlayNow))
        EventManager::getInstance().fireExecutePlayoutCommandEvent(ExecutePlayoutCommandEvent(QEvent::KeyPress, Qt::Key_F2, Qt::ShiftModifier));
    else if (this->command.getPlayoutCommand() == Playout::toString(Playout::PlayoutType::Load))
        EventManager::getInstance().fireExecutePlayoutCommandEvent(ExecutePlayoutCommandEvent(QEvent::KeyPress, Qt::Key_F3, Qt::NoModifier));
    else if (this->command.getPlayoutCommand() == Playout::toString(Playout::PlayoutType::PauseResume))
        EventManager::getInstance().fireExecutePlayoutCommandEvent(ExecutePlayoutCommandEvent(QEvent::KeyPress, Qt::Key_F4, Qt::NoModifier));
    else if (this->command.getPlayoutCommand() == Playout::toString(Playout::PlayoutType::Next))
        EventManager::getInstance().fireExecutePlayoutCommandEvent(ExecutePlayoutCommandEvent(QEvent::KeyPress, Qt::Key_F5, Qt::NoModifier));
    else if (this->command.getPlayoutCommand() == Playout::toString(Playout::PlayoutType::Update))
        EventManager::getInstance().fireExecutePlayoutCommandEvent(ExecutePlayoutCommandEvent(QEvent::KeyPress, Qt::Key_F6, Qt::NoModifier));
    else if (this->command.getPlayoutCommand() == Playout::toString(Playout::PlayoutType::Invoke))
        EventManager::getInstance().fireExecutePlayoutCommandEvent(ExecutePlayoutCommandEvent(QEvent::KeyPress, Qt::Key_F7, Qt::NoModifier));
    else if (this->command.getPlayoutCommand() == Playout::toString(Playout::PlayoutType::Preview))
        EventManager::getInstance().fireExecutePlayoutCommandEvent(ExecutePlayoutCommandEvent(QEvent::KeyPress, Qt::Key_F8, Qt::NoModifier));
    else if (this->command.getPlayoutCommand() == Playout::toString(Playout::PlayoutType::Clear))
        EventManager::getInstance().fireExecutePlayoutCommandEvent(ExecutePlayoutCommandEvent(QEvent::KeyPress, Qt::Key_F10, Qt::NoModifier));
    else if (this->command.getPlayoutCommand() == Playout::toString(Playout::PlayoutType::ClearVideoLayer))
        EventManager::getInstance().fireExecutePlayoutCommandEvent(ExecutePlayoutCommandEvent(QEvent::KeyPress, Qt::Key_F11, Qt::NoModifier));
    else if (this->command.getPlayoutCommand() == Playout::toString(Playout::PlayoutType::ClearChannel))
        EventManager::getInstance().fireExecutePlayoutCommandEvent(ExecutePlayoutCommandEvent(QEvent::KeyPress, Qt::Key_F12, Qt::NoModifier));

    if (this->markUsedItems)
        setUsed(true);
}

void RundownPlayoutCommandWidget::delayChanged(int delay)
{
    this->labelDelay->setText(QString("Delay: %1").arg(delay));
}

void RundownPlayoutCommandWidget::checkGpiConnection()
{
    this->labelGpiConnected->setVisible(this->command.getAllowGpi());

    if (GpiManager::getInstance().getGpiDevice()->isConnected())
        this->labelGpiConnected->setPixmap(QPixmap(":/Graphics/Images/GpiConnected.png"));
    else
        this->labelGpiConnected->setPixmap(QPixmap(":/Graphics/Images/GpiDisconnected.png"));
}

void RundownPlayoutCommandWidget::configureOscSubscriptions()
{
    if (!this->command.getAllowRemoteTriggering())
        return;

    if (this->playControlSubscription != NULL)
        this->playControlSubscription->disconnect(); // Disconnect all events.

    if (this->playNowControlSubscription != NULL)
        this->playNowControlSubscription->disconnect(); // Disconnect all events.

    QString playControlFilter = Osc::DEFAULT_PLAY_CONTROL_FILTER;
    playControlFilter.replace("#UID#", this->command.getRemoteTriggerId());
    this->playControlSubscription = new OscSubscription(playControlFilter, this);
    QObject::connect(this->playControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(playControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString playNowControlFilter = Osc::DEFAULT_PLAYNOW_CONTROL_FILTER;
    playNowControlFilter.replace("#UID#", this->command.getRemoteTriggerId());
    this->playNowControlSubscription = new OscSubscription(playNowControlFilter, this);
    QObject::connect(this->playNowControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(playNowControlSubscriptionReceived(const QString&, const QList<QVariant>&)));
}

void RundownPlayoutCommandWidget::allowGpiChanged(bool allowGpi)
{
    checkGpiConnection();
}

void RundownPlayoutCommandWidget::gpiConnectionStateChanged(bool connected, GpiDevice* device)
{
    checkGpiConnection();
}

void RundownPlayoutCommandWidget::remoteTriggerIdChanged(const QString& remoteTriggerId)
{
    configureOscSubscriptions();

    this->labelRemoteTriggerId->setText(QString("UID: %1").arg(remoteTriggerId));
}

void RundownPlayoutCommandWidget::playControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0] == 1)
        executeCommand(Playout::PlayoutType::Play);
}

void RundownPlayoutCommandWidget::playNowControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0] == 1)
        executeCommand(Playout::PlayoutType::PlayNow);
}
