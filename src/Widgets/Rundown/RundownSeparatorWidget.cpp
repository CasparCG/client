#include "RundownSeparatorWidget.h"

#include "Global.h"
#include "GpiManager.h"

#include "Events/Inspector/LabelChangedEvent.h"

#include <QtGui/QApplication>
#include <QtCore/QObject>

RundownSeparatorWidget::RundownSeparatorWidget(const LibraryModel& model, QWidget* parent, const QString& color,
                                               bool active, bool inGroup, bool compactView)
    : QWidget(parent),
      active(active), inGroup(inGroup), compactView(compactView), color(color), model(model)
{
    setupUi(this);

    this->animation = new ActiveAnimation(this->labelActiveColor);

    setColor(this->color);
    setActive(this->active);
    setCompactView(this->compactView);

    this->labelGroupColor->setVisible(this->inGroup);
    this->labelGroupColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_GROUP_COLOR));
    this->labelColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_SEPARATOR_COLOR));

    this->labelLabel->setText(this->model.getLabel());

    qApp->installEventFilter(this);
}

bool RundownSeparatorWidget::eventFilter(QObject* target, QEvent* event)
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
        this->labelIcon->setFixedSize(Rundown::COMPACT_ICON_WIDTH, Rundown::COMPACT_ICON_HEIGHT);
    else
        this->labelIcon->setFixedSize(Rundown::DEFAULT_ICON_WIDTH, Rundown::DEFAULT_ICON_HEIGHT);

    this->compactView = compactView;
}

bool RundownSeparatorWidget::isGroup() const
{
    return false;
}

bool RundownSeparatorWidget::isInGroup() const
{
    return this->inGroup;
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
    this->setStyleSheet(QString("#frameItem, #frameStatus { background-color: %1; }").arg(color));
}

void RundownSeparatorWidget::setActive(bool active)
{
    this->active = active;

    this->animation->stop();

    if (this->active)
        this->labelActiveColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_ACTIVE_COLOR));
    else
        this->labelActiveColor->setStyleSheet("");
}

void RundownSeparatorWidget::setInGroup(bool inGroup)
{
    this->inGroup = inGroup;
    this->labelGroupColor->setVisible(this->inGroup);
}

bool RundownSeparatorWidget::executeCommand(Playout::PlayoutType::Type type)
{
    if (this->active)
        this->animation->start(1);

    return false;
}
