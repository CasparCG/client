#include "RundownWidget.h"
#include "RundownTreeWidget.h"

#include "EventManager.h"
#include "Events/RemoveRundownEvent.h"

#include <QtGui/QApplication>
#include <QtGui/QIcon>
#include <QtGui/QToolButton>

RundownWidget::RundownWidget(QWidget* parent)
    : QWidget(parent),
      tabCount(1)
{
    setupUi(this);

    setupMenus();

    QToolButton* toolButtonRundownDropdown = new QToolButton(this);
    toolButtonRundownDropdown->setObjectName("toolButtonRundownDropdown");
    toolButtonRundownDropdown->setMenu(this->contextMenuRundownDropdown);
    toolButtonRundownDropdown->setPopupMode(QToolButton::InstantPopup);
    this->tabWidgetRundown->setCornerWidget(toolButtonRundownDropdown);
    this->tabWidgetRundown->setTabIcon(0, QIcon(":/Graphics/Images/TabSplitter.png"));

    if (this->tabWidgetRundown->count() == 0)
        EventManager::getInstance().fireAddRundownEvent();

    qApp->installEventFilter(this);
}

void RundownWidget::setupMenus()
{
    this->contextMenuColor = new QMenu(this);
    this->contextMenuColor->setTitle("Colorize Tab");
    //this->contextMenuColor->setIcon(QIcon(":/Graphics/Images/Color.png"));
    this->contextMenuColor->addAction(/*QIcon(":/Graphics/Images/Color.png"),*/ "Chocolate");
    this->contextMenuColor->addAction(/*QIcon(":/Graphics/Images/Color.png"),*/ "DarkKhaki");
    this->contextMenuColor->addAction(/*QIcon(":/Graphics/Images/Color.png"),*/ "DarkSlateGray");
    this->contextMenuColor->addAction(/*QIcon(":/Graphics/Images/Color.png"),*/ "Maroon");
    this->contextMenuColor->addAction(/*QIcon(":/Graphics/Images/Color.png"),*/ "MaroonLight");
    this->contextMenuColor->addAction(/*QIcon(":/Graphics/Images/Color.png"),*/ "OliveDrab");
    this->contextMenuColor->addAction(/*QIcon(":/Graphics/Images/Color.png"),*/ "RoyalBlue");
    this->contextMenuColor->addAction(/*QIcon(":/Graphics/Images/Color.png"),*/ "SeaGreen");
    this->contextMenuColor->addAction(/*QIcon(":/Graphics/Images/Color.png"),*/ "Sienna");
    this->contextMenuColor->addAction(/*QIcon(":/Graphics/Images/Color.png"),*/ "SteelBlue");
    this->contextMenuColor->addSeparator();
    this->contextMenuColor->addAction(/*QIcon(":/Graphics/Images/Color.png"),*/ "Reset");

    this->contextMenuRundownDropdown = new QMenu(this);
    this->contextMenuRundownDropdown->setTitle("Rundown Dropdown");
    this->contextMenuRundownDropdown->addAction(/*QIcon(":/Graphics/Images/AddRundown.png"),*/ "Add rundown", this, SLOT(addRundown()));
    this->contextMenuRundownDropdown->addAction(/*QIcon(":/Graphics/Images/RenameRundown.png"),*/ "Rename rundown", this, SLOT(renameRundown()));
    this->contextMenuRundownDropdown->addSeparator();
    this->contextMenuRundownDropdown->addMenu(this->contextMenuColor);
}

bool RundownWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(event);
        if (keyEvent->key() >= 49 && keyEvent->key() <= 57 && keyEvent->modifiers() == Qt::AltModifier) // [1-9]
            return selectTab(keyEvent->text().toInt());
    }
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::AddRundown))
    {
        RundownTreeWidget* widget = new RundownTreeWidget(this);
        widget->setActive(true);

        int index = this->tabWidgetRundown->addTab(widget, QIcon(":/Graphics/Images/TabSplitter.png"), QString("Rundown %1").arg(this->tabCount++));
        this->tabWidgetRundown->setCurrentIndex(index);

        if (this->tabWidgetRundown->count() == Rundown::MAX_NUMBER_OF_RUNDONWS)
            this->contextMenuRundownDropdown->actions().at(0)->setEnabled(false);

        return true;
    }
    else if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RemoveRundown))
    {
        if (this->tabWidgetRundown->count() <= Rundown::MAX_NUMBER_OF_RUNDONWS)
            this->contextMenuRundownDropdown->actions().at(0)->setEnabled(true);

        RemoveRundownEvent* removeRundownEvent = dynamic_cast<RemoveRundownEvent*>(event);

        // Delete the page widget, which automatically removes the tab as well.
        delete this->tabWidgetRundown->widget(removeRundownEvent->getIndex());

        return true;
    }

    return QObject::eventFilter(target, event);
}

void RundownWidget::addRundown()
{
    if (this->tabWidgetRundown->count() < Rundown::MAX_NUMBER_OF_RUNDONWS)
        EventManager::getInstance().fireAddRundownEvent();
}

void RundownWidget::renameRundown()
{
}

bool RundownWidget::selectTab(int index)
{
    if (index <= this->tabWidgetRundown->count())
        this->tabWidgetRundown->setCurrentIndex(index - 1);

    return true;
}

void RundownWidget::tabCloseRequested(int index)
{
    if (this->tabWidgetRundown->count() > 1)
        EventManager::getInstance().fireRemoveRundownEvent(index);
}

void RundownWidget::currentChanged(int index)
{
    for (int i = 0; i < this->tabWidgetRundown->count(); i++)
        dynamic_cast<RundownTreeWidget*>(this->tabWidgetRundown->widget(i))->setActive(false);

    dynamic_cast<RundownTreeWidget*>(this->tabWidgetRundown->widget(index))->setActive(true);
}
