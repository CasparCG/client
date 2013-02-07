#include "RundownSeparatorWidget.h"

#include "Global.h"
#include "GpiManager.h"

#include "Events/RundownItemChangedEvent.h"

#include <QtGui/QApplication>
#include <QtCore/QObject>

RundownSeparatorWidget::RundownSeparatorWidget(const LibraryModel& model, QWidget* parent, const QString& color,
                                               bool active, bool inGroup, bool compactView)
    : QWidget(parent),
      active(active), inGroup(inGroup), compactView(compactView), color(color), model(model)
{
    setupUi(this);

    this->animation = new ColorAnimation(this->labelActiveColor);

    setColor(color);
    setActive(active);
    setCompactView(compactView);

    this->labelGroupColor->setVisible(this->inGroup);
    this->labelGroupColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_GROUP_COLOR));
    this->labelColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_SEPARATOR_COLOR));

    this->labelLabel->setText(this->model.getLabel());

    qApp->installEventFilter(this);
}

bool RundownSeparatorWidget::eventFilter(QObject* target, QEvent* event)
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

AbstractRundownWidget* RundownSeparatorWidget::clone()
{
    RundownSeparatorWidget* widget = new RundownSeparatorWidget(this->model, this->parentWidget(), this->color,
                                                                this->active, this->inGroup, this->compactView);

    return widget;
}

void RundownSeparatorWidget::readProperties(boost::property_tree::wptree& pt)
{
    if (pt.count(L"color") > 0) setColor(QString::fromStdWString(pt.get<std::wstring>(L"color")));
}

void RundownSeparatorWidget::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("color", this->color);
}

void RundownSeparatorWidget::setCompactView(bool compactView)
{
    if (compactView)
        this->labelThumbnail->setFixedSize(Define::COMPACT_VIEW_WIDTH, Define::COMPACT_VIEW_HEIGHT);
    else
        this->labelThumbnail->setFixedSize(Define::DEFAULT_VIEW_WIDTH, Define::DEFAULT_VIEW_HEIGHT);

    this->compactView = compactView;
}

bool RundownSeparatorWidget::isGroup() const
{
    return false;
}

AbstractCommand* RundownSeparatorWidget::getCommand()
{
    return &this->command;
}

LibraryModel* RundownSeparatorWidget::getLibraryModel()
{
    return &this->model;
}

void RundownSeparatorWidget::setColor(const QString& color)
{
    this->color = color;
    this->setStyleSheet(QString("#frameItem, #frameStatus { background-color: rgba(%1); }").arg(color));
}

void RundownSeparatorWidget::setActive(bool active)
{
    this->active = active;

    this->animation->stop();

    if (this->active)
        this->labelActiveColor->setStyleSheet("background-color: red;");
    else
        this->labelActiveColor->setStyleSheet("");
}

void RundownSeparatorWidget::setInGroup(bool inGroup)
{
    this->inGroup = inGroup;
    this->labelGroupColor->setVisible(inGroup);
}

bool RundownSeparatorWidget::executeCommand(enum Playout::PlayoutType::Type type)
{
    if (this->active)
        this->animation->start(1);

    return false;
}
