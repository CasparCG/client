#include "RundownGroupWidget.h"

#include "Global.h"
#include "GpiManager.h"

#include "Events/RundownItemChangedEvent.h"

#include <QtGui/QApplication>
#include <QtCore/QObject>

RundownGroupWidget::RundownGroupWidget(const LibraryModel& model, QWidget* parent, bool active)
    : QWidget(parent),
      active(active), model(model)
{
    setupUi(this);

    setActive(active);

    this->labelGroupColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_GROUP_COLOR));
    this->labelColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_GROUP_COLOR));

    this->labelName->setText(this->model.getName());

    QObject::connect(&this->command, SIGNAL(allowGpiChanged(bool)), this, SLOT(allowGpiChanged(bool)));
    QObject::connect(GpiManager::getInstance().getGpiDevice().data(), SIGNAL(connectionStateChanged(bool, GpiDevice*)),
                     this, SLOT(gpiDeviceConnected(bool, GpiDevice*)));

    checkGpiTriggerable();

    qApp->installEventFilter(this);
}

bool RundownGroupWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemChanged))
    {
        // This event is not for us.
        if (!this->active)
            return false;

        RundownItemChangedEvent* rundownItemChangedEvent = dynamic_cast<RundownItemChangedEvent*>(event);
        this->model.setName(rundownItemChangedEvent->getName());

        this->labelName->setText(this->model.getName());
    }

    return QObject::eventFilter(target, event);
}

IRundownWidget* RundownGroupWidget::clone()
{
    RundownGroupWidget* widget = new RundownGroupWidget(this->model, this->parentWidget(), this->active);

    GroupCommand* command = dynamic_cast<GroupCommand*>(widget->getCommand());
    command->setChannel(this->command.getChannel());
    command->setVideolayer(this->command.getVideolayer());
    command->setDelay(this->command.getDelay());
    command->setAllowGpi(this->command.getAllowGpi());

    return widget;
}

bool RundownGroupWidget::isGroup() const
{
    return true;
}

ICommand* RundownGroupWidget::getCommand()
{
    return &this->command;
}

LibraryModel* RundownGroupWidget::getLibraryModel()
{
    return &this->model;
}

void RundownGroupWidget::setActive(bool active)
{
    this->active = active;

    if (this->active)
        this->labelActiveColor->setStyleSheet("background-color: red;");
    else
        this->labelActiveColor->setStyleSheet("");
}

void RundownGroupWidget::setExpanded(bool expanded)
{
    //this->labelGroupColor->setVisible(expanded);
}

void RundownGroupWidget::channelChanged(int channel)
{

}

void RundownGroupWidget::videolayerChanged(int videolayer)
{

}

void RundownGroupWidget::delayChanged(int delay)
{

}

void RundownGroupWidget::checkGpiTriggerable()
{
    labelGpiTriggerable->setVisible(this->command.getAllowGpi());

    if (GpiManager::getInstance().getGpiDevice()->isConnected())
        labelGpiTriggerable->setPixmap(QPixmap(":/Graphics/Images/GpiConnected.png"));
    else
        labelGpiTriggerable->setPixmap(QPixmap(":/Graphics/Images/GpiDisconnected.png"));
}

void RundownGroupWidget::allowGpiChanged(bool allowGpi)
{
    checkGpiTriggerable();
}

void RundownGroupWidget::gpiDeviceConnected(bool connected, GpiDevice* device)
{
    checkGpiTriggerable();
}
