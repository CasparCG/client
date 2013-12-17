#include "RundownGroupWidget.h"

#include "Global.h"
#include "GpiManager.h"

#include "Events/Inspector/LabelChangedEvent.h"

#include <QtCore/QObject>

#include <QtGui/QApplication>
#include <QtGui/QTreeWidget>


RundownGroupWidget::RundownGroupWidget(const LibraryModel& model, QWidget* parent, const QString& color, bool active, bool compactView)
    : QWidget(parent),
      active(active), compactView(compactView), color(color), model(model), stopControlSubscription(NULL),
      playControlSubscription(NULL), loadControlSubscription(NULL), pauseControlSubscription(NULL), nextControlSubscription(NULL),
      updateControlSubscription(NULL), invokeControlSubscription(NULL), clearControlSubscription(NULL), clearVideolayerControlSubscription(NULL),
      clearChannelControlSubscription(NULL)
{
    setupUi(this);

    this->animation = new ActiveAnimation(this->labelActiveColor);

    setColor(this->color);
    setActive(this->active);
    setCompactView(this->compactView);

    this->labelAutoStep->setVisible(false);
    this->labelAutoPlay->setVisible(false);

    this->labelGroupColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_GROUP_COLOR));
    this->labelColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_GROUP_COLOR));

    this->labelLabel->setText(this->model.getLabel());

    QObject::connect(&this->command, SIGNAL(notesChanged(const QString&)), this, SLOT(notesChanged(const QString&)));
    QObject::connect(&this->command, SIGNAL(allowGpiChanged(bool)), this, SLOT(allowGpiChanged(bool)));
    QObject::connect(&this->command, SIGNAL(autoStepChanged(bool)), this, SLOT(autoStepChanged(bool)));
    QObject::connect(&this->command, SIGNAL(autoPlayChanged(bool)), this, SLOT(autoPlayChanged(bool)));

    QObject::connect(GpiManager::getInstance().getGpiDevice().data(), SIGNAL(connectionStateChanged(bool, GpiDevice*)), this, SLOT(gpiConnectionStateChanged(bool, GpiDevice*)));

    checkGpiConnection();

    configureOscSubscriptions();

    qApp->installEventFilter(this);
}

bool RundownGroupWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Event::EventType::LabelChanged))
    {
        // This event is not for us.
        if (!this->active)
            return false;

        LabelChangedEvent* labelChanged = dynamic_cast<LabelChangedEvent*>(event);
        this->model.setLabel(labelChanged->getLabel());

        this->labelLabel->setText(this->model.getLabel());

        configureOscSubscriptions();

        return true;
    }

    return QObject::eventFilter(target, event);
}

AbstractRundownWidget* RundownGroupWidget::clone()
{
    RundownGroupWidget* widget = new RundownGroupWidget(this->model, this->parentWidget(), this->color,
                                                        this->active, this->compactView);

    GroupCommand* command = dynamic_cast<GroupCommand*>(widget->getCommand());
    command->setChannel(this->command.getChannel());
    command->setVideolayer(this->command.getVideolayer());
    command->setDelay(this->command.getDelay());
    command->setAllowGpi(this->command.getAllowGpi());
    command->setAllowRemoteTriggering(this->command.getAllowRemoteTriggering());
    command->setNotes(this->command.getNotes());
    command->setAutoStep(this->command.getAutoStep());
    command->setAutoPlay(this->command.getAutoPlay());

    return widget;
}

void RundownGroupWidget::readProperties(boost::property_tree::wptree& pt)
{
    if (pt.count(L"color") > 0) setColor(QString::fromStdWString(pt.get<std::wstring>(L"color")));
}

void RundownGroupWidget::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("color", this->color);
}

void RundownGroupWidget::setCompactView(bool compactView)
{
    if (compactView)
    {
        this->labelGroupColor->move(this->labelGroupColor->x(), Rundown::COMPACT_ITEM_HEIGHT - 2);
        this->labelIcon->setFixedSize(Rundown::COMPACT_ICON_WIDTH, Rundown::COMPACT_ICON_HEIGHT);
        this->labelGpiConnected->setFixedSize(Rundown::COMPACT_ICON_WIDTH, Rundown::COMPACT_ICON_HEIGHT);
        this->labelAutoStep->setFixedSize(Rundown::COMPACT_ICON_WIDTH, Rundown::COMPACT_ICON_HEIGHT);
        this->labelAutoPlay->setFixedSize(Rundown::COMPACT_ICON_WIDTH, Rundown::COMPACT_ICON_HEIGHT);
    }
    else
    {
        this->labelGroupColor->move(this->labelGroupColor->x(), Rundown::DEFAULT_ITEM_HEIGHT - 2);
        this->labelIcon->setFixedSize(Rundown::DEFAULT_ICON_WIDTH, Rundown::DEFAULT_ICON_HEIGHT);
        this->labelGpiConnected->setFixedSize(Rundown::DEFAULT_ICON_WIDTH, Rundown::DEFAULT_ICON_HEIGHT);
        this->labelAutoStep->setFixedSize(Rundown::DEFAULT_ICON_WIDTH, Rundown::DEFAULT_ICON_HEIGHT);
        this->labelAutoPlay->setFixedSize(Rundown::DEFAULT_ICON_WIDTH, Rundown::DEFAULT_ICON_HEIGHT);
    }

    this->compactView = compactView;
}

bool RundownGroupWidget::isGroup() const
{
    return true;
}

bool RundownGroupWidget::isInGroup() const
{
    return false;
}

AbstractCommand* RundownGroupWidget::getCommand()
{
    return &this->command;
}

LibraryModel* RundownGroupWidget::getLibraryModel()
{
    return &this->model;
}

void RundownGroupWidget::setColor(const QString& color)
{
    this->color = color;
    this->setStyleSheet(QString("#frameItem, #frameStatus { background-color: rgba(%1); }").arg(color));
}

void RundownGroupWidget::setActive(bool active)
{
    this->active = active;

    this->animation->stop();

    if (this->active)
        this->labelActiveColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_ACTIVE_COLOR));
    else
        this->labelActiveColor->setStyleSheet("");
}

bool RundownGroupWidget::executeCommand(enum Playout::PlayoutType::Type type)
{
    if (this->active)
        this->animation->start(1);

    return true;
}

bool RundownGroupWidget::executeOscCommand(enum Playout::PlayoutType::Type type)
{
    if (this->command.getAllowRemoteTriggering())
    {
        // Our parent is the rundown tree.
        QTreeWidget* treeWidgetRundown = dynamic_cast<QTreeWidget*>(this->parentWidget()->parentWidget());
        for (int i = 0; i < treeWidgetRundown->invisibleRootItem()->childCount(); i++)
        {
            QTreeWidgetItem* child = treeWidgetRundown->invisibleRootItem()->child(i);
            QWidget* widget = treeWidgetRundown->itemWidget(child, 0);
            if (widget == this)
            {
                for (int j = 0; j < child->childCount(); j++)
                    dynamic_cast<AbstractPlayoutCommand*>(treeWidgetRundown->itemWidget(child->child(j), 0))->executeCommand(type);
            }
        }
    }

    return true;
}

void RundownGroupWidget::checkGpiConnection()
{
    this->labelGpiConnected->setVisible(this->command.getAllowGpi());

    if (GpiManager::getInstance().getGpiDevice()->isConnected())
        this->labelGpiConnected->setPixmap(QPixmap(":/Graphics/Images/GpiConnected.png"));
    else
        this->labelGpiConnected->setPixmap(QPixmap(":/Graphics/Images/GpiDisconnected.png"));
}

void RundownGroupWidget::configureOscSubscriptions()
{
    if (this->stopControlSubscription != NULL)
        this->stopControlSubscription->disconnect(); // Disconnect all events.

    if (this->playControlSubscription != NULL)
        this->playControlSubscription->disconnect(); // Disconnect all events.

    if (this->loadControlSubscription != NULL)
        this->loadControlSubscription->disconnect(); // Disconnect all events.

    if (this->pauseControlSubscription != NULL)
        this->pauseControlSubscription->disconnect(); // Disconnect all events.

    if (this->nextControlSubscription != NULL)
        this->nextControlSubscription->disconnect(); // Disconnect all events.

    if (this->updateControlSubscription != NULL)
        this->updateControlSubscription->disconnect(); // Disconnect all events.

    if (this->invokeControlSubscription != NULL)
        this->invokeControlSubscription->disconnect(); // Disconnect all events.

    if (this->clearControlSubscription != NULL)
        this->clearControlSubscription->disconnect(); // Disconnect all events.

    if (this->clearVideolayerControlSubscription != NULL)
        this->clearVideolayerControlSubscription->disconnect(); // Disconnect all events.

    if (this->clearChannelControlSubscription != NULL)
        this->clearChannelControlSubscription->disconnect(); // Disconnect all events.

    QString stopControlFilter = Osc::DEFAULT_STOP_CONTROL_FILTER;
    stopControlFilter.replace("#LABEL#", this->model.getLabel());
    this->stopControlSubscription = new OscSubscription(stopControlFilter, this);
    QObject::connect(this->stopControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(stopControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString playControlFilter = Osc::DEFAULT_PLAY_CONTROL_FILTER;
    playControlFilter.replace("#LABEL#", this->model.getLabel());
    this->playControlSubscription = new OscSubscription(playControlFilter, this);
    QObject::connect(this->playControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(playControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString loadControlFilter = Osc::DEFAULT_LOAD_CONTROL_FILTER;
    loadControlFilter.replace("#LABEL#", this->model.getLabel());
    this->loadControlSubscription = new OscSubscription(loadControlFilter, this);
    QObject::connect(this->loadControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(loadControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString pauseControlFilter = Osc::DEFAULT_PAUSE_CONTROL_FILTER;
    pauseControlFilter.replace("#LABEL#", this->model.getLabel());
    this->pauseControlSubscription = new OscSubscription(pauseControlFilter, this);
    QObject::connect(this->pauseControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(pauseControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString nextControlFilter = Osc::DEFAULT_NEXT_CONTROL_FILTER;
    nextControlFilter.replace("#LABEL#", this->model.getLabel());
    this->nextControlSubscription = new OscSubscription(nextControlFilter, this);
    QObject::connect(this->nextControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(nextControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString updateControlFilter = Osc::DEFAULT_UPDATE_CONTROL_FILTER;
    updateControlFilter.replace("#LABEL#", this->model.getLabel());
    this->updateControlSubscription = new OscSubscription(updateControlFilter, this);
    QObject::connect(this->updateControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(updateControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString invokeControlFilter = Osc::DEFAULT_INVOKE_CONTROL_FILTER;
    invokeControlFilter.replace("#LABEL#", this->model.getLabel());
    this->invokeControlSubscription = new OscSubscription(invokeControlFilter, this);
    QObject::connect(this->invokeControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(invokeControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString clearControlFilter = Osc::DEFAULT_CLEAR_CONTROL_FILTER;
    clearControlFilter.replace("#LABEL#", this->model.getLabel());
    this->clearControlSubscription = new OscSubscription(clearControlFilter, this);
    QObject::connect(this->clearControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(clearControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString clearVideolayerControlFilter = Osc::DEFAULT_CLEAR_VIDEOLAYER_CONTROL_FILTER;
    clearVideolayerControlFilter.replace("#LABEL#", this->model.getLabel());
    this->clearVideolayerControlSubscription = new OscSubscription(clearVideolayerControlFilter, this);
    QObject::connect(this->clearVideolayerControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(clearVideolayerControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString clearChannelControlFilter = Osc::DEFAULT_CLEAR_CHANNEL_CONTROL_FILTER;
    clearChannelControlFilter.replace("#LABEL#", this->model.getLabel());
    this->clearChannelControlSubscription = new OscSubscription(clearChannelControlFilter, this);
    QObject::connect(this->clearChannelControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(clearChannelControlSubscriptionReceived(const QString&, const QList<QVariant>&)));
}

void RundownGroupWidget::notesChanged(const QString& note)
{
    this->labelNoteField->setText(this->command.getNotes());
}

void RundownGroupWidget::allowGpiChanged(bool allowGpi)
{
    checkGpiConnection();
}

void RundownGroupWidget::autoStepChanged(bool autoStep)
{
    this->labelAutoStep->setVisible(autoStep);
}

void RundownGroupWidget::gpiConnectionStateChanged(bool connected, GpiDevice* device)
{
    checkGpiConnection();
}

void RundownGroupWidget::autoPlayChanged(bool autoPlay)
{
    this->labelAutoPlay->setVisible(autoPlay);
}

void RundownGroupWidget::stopControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->command.getAllowRemoteTriggering())
        executeOscCommand(Playout::PlayoutType::Stop);
}

void RundownGroupWidget::playControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->command.getAllowRemoteTriggering())
        executeOscCommand(Playout::PlayoutType::Play);
}

void RundownGroupWidget::loadControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->command.getAllowRemoteTriggering())
        executeOscCommand(Playout::PlayoutType::Load);
}

void RundownGroupWidget::pauseControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->command.getAllowRemoteTriggering())
        executeOscCommand(Playout::PlayoutType::Pause);
}

void RundownGroupWidget::invokeControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->command.getAllowRemoteTriggering())
        executeOscCommand(Playout::PlayoutType::Invoke);
}

void RundownGroupWidget::nextControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->command.getAllowRemoteTriggering())
        executeOscCommand(Playout::PlayoutType::Next);
}

void RundownGroupWidget::updateControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->command.getAllowRemoteTriggering())
        executeOscCommand(Playout::PlayoutType::Update);
}

void RundownGroupWidget::clearControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->command.getAllowRemoteTriggering())
        executeOscCommand(Playout::PlayoutType::Clear);
}

void RundownGroupWidget::clearVideolayerControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->command.getAllowRemoteTriggering())
        executeOscCommand(Playout::PlayoutType::ClearVideolayer);
}

void RundownGroupWidget::clearChannelControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->command.getAllowRemoteTriggering())
        executeOscCommand(Playout::PlayoutType::ClearChannel);
}
