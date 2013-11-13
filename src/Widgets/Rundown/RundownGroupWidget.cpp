#include "RundownGroupWidget.h"

#include "Global.h"
#include "GpiManager.h"

#include "Events/Inspector/LabelChangedEvent.h"

#include <QtGui/QApplication>
#include <QtCore/QObject>

RundownGroupWidget::RundownGroupWidget(const LibraryModel& model, QWidget* parent, const QString& color, bool active, bool compactView)
    : QWidget(parent),
      active(active), compactView(compactView), color(color), model(model)
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

    return false;
}

void RundownGroupWidget::checkGpiConnection()
{
    this->labelGpiConnected->setVisible(this->command.getAllowGpi());

    if (GpiManager::getInstance().getGpiDevice()->isConnected())
        this->labelGpiConnected->setPixmap(QPixmap(":/Graphics/Images/GpiConnected.png"));
    else
        this->labelGpiConnected->setPixmap(QPixmap(":/Graphics/Images/GpiDisconnected.png"));
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
