#include "RundownGroupWidget.h"

#include "Global.h"
#include "GpiManager.h"

#include "Events/RundownItemChangedEvent.h"

#include <QtGui/QApplication>
#include <QtCore/QObject>

RundownGroupWidget::RundownGroupWidget(const LibraryModel& model, QWidget* parent, const QString& color, bool active,
                                       bool autoStep, bool compactView)
    : QWidget(parent),
      active(active), autoStep(autoStep), compactView(compactView), color(color), model(model)
{
    setupUi(this);

    this->animation = new ActiveAnimation(this->labelActiveColor);

    setColor(color);
    setActive(active);
    setCompactView(compactView);

    this->labelAutoStep->setVisible(this->autoStep);
    this->labelGroupColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_GROUP_COLOR));
    this->labelColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_GROUP_COLOR));

    this->labelLabel->setText(this->model.getLabel());

    QObject::connect(&this->command, SIGNAL(notesChanged(const QString&)), this, SLOT(notesChanged(const QString&)));
    QObject::connect(&this->command, SIGNAL(allowGpiChanged(bool)), this, SLOT(allowGpiChanged(bool)));
    QObject::connect(&this->command, SIGNAL(autoStepChanged(bool)), this, SLOT(autoStepChanged(bool)));
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
        this->model.setLabel(rundownItemChangedEvent->getLabel());
        this->model.setName(rundownItemChangedEvent->getName());

        this->labelLabel->setText(this->model.getLabel());
    }

    return QObject::eventFilter(target, event);
}

AbstractRundownWidget* RundownGroupWidget::clone()
{
    RundownGroupWidget* widget = new RundownGroupWidget(this->model, this->parentWidget(), this->color, this->autoStep,
                                                        this->active, this->compactView);

    GroupCommand* command = dynamic_cast<GroupCommand*>(widget->getCommand());
    command->setChannel(this->command.getChannel());
    command->setVideolayer(this->command.getVideolayer());
    command->setDelay(this->command.getDelay());
    command->setAllowGpi(this->command.getAllowGpi());
    command->setNotes(this->command.getNotes());
    command->setAutoStep(this->command.getAutoStep());

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
        this->labelGroupColor->move(this->labelGroupColor->x(), Define::COMPACT_ITEM_HEIGHT - 2);
        this->labelThumbnail->setFixedSize(Define::COMPACT_VIEW_WIDTH, Define::COMPACT_VIEW_HEIGHT);
        this->labelGpiConnected->setFixedSize(Define::COMPACT_VIEW_WIDTH, Define::COMPACT_VIEW_HEIGHT);
        this->labelAutoStep->setFixedSize(Define::COMPACT_VIEW_WIDTH, Define::COMPACT_VIEW_HEIGHT);
    }
    else
    {
        this->labelGroupColor->move(this->labelGroupColor->x(), Define::DEFAULT_ITEM_HEIGHT - 2);
        this->labelThumbnail->setFixedSize(Define::DEFAULT_VIEW_WIDTH, Define::DEFAULT_VIEW_HEIGHT);
        this->labelGpiConnected->setFixedSize(Define::DEFAULT_VIEW_WIDTH, Define::DEFAULT_VIEW_HEIGHT);
        this->labelAutoStep->setFixedSize(Define::DEFAULT_VIEW_WIDTH, Define::DEFAULT_VIEW_HEIGHT);
    }

    this->compactView = compactView;
}

bool RundownGroupWidget::isGroup() const
{
    return true;
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
        this->labelActiveColor->setStyleSheet("background-color: red;");
    else
        this->labelActiveColor->setStyleSheet("");
}

bool RundownGroupWidget::executeCommand(enum Playout::PlayoutType::Type type)
{
    if (this->active)
        this->animation->start(1);

    return false;
}

void RundownGroupWidget::checkGpiTriggerable()
{
    labelGpiConnected->setVisible(this->command.getAllowGpi());

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
    checkGpiTriggerable();
}

void RundownGroupWidget::autoStepChanged(bool autoStep)
{
    this->autoStep = autoStep;
    this->labelAutoStep->setVisible(this->autoStep);
}

void RundownGroupWidget::gpiDeviceConnected(bool connected, GpiDevice* device)
{
    checkGpiTriggerable();
}
