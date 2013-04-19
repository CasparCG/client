#include "RundownWidget.h"
#include "RundownTreeWidget.h"

#include "EventManager.h"
#include "Events/DeleteRundownEvent.h"
#include "Events/ActiveRundownChangedEvent.h"

#include <QtCore/QFileInfo>

#include <QtGui/QApplication>
#include <QtGui/QIcon>
#include <QtGui/QFileDialog>
#include <QtGui/QToolButton>

RundownWidget::RundownWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUi(this);

    setupMenus();

    //QToolButton* toolButtonRundownDropdown = new QToolButton(this);
    //toolButtonRundownDropdown->setObjectName("toolButtonRundownDropdown");
    //toolButtonRundownDropdown->setMenu(this->contextMenuRundownDropdown);
    //toolButtonRundownDropdown->setPopupMode(QToolButton::InstantPopup);
    //this->tabWidgetRundown->setCornerWidget(toolButtonRundownDropdown);
    //this->tabWidgetRundown->setTabIcon(0, QIcon(":/Graphics/Images/TabSplitter.png"));

    RundownTreeWidget* widget = new RundownTreeWidget(this);
    int index = this->tabWidgetRundown->addTab(widget, QIcon(":/Graphics/Images/TabSplitter.png"), Rundown::DEFAULT_NAME);
    this->tabWidgetRundown->setCurrentIndex(index);

    qApp->installEventFilter(this);
}

void RundownWidget::setupMenus()
{
    //this->contextMenuRundownDropdown = new QMenu(this);
    //this->contextMenuRundownDropdown->setTitle("Rundown Dropdown");
    //this->contextMenuRundownDropdown->addAction(/*QIcon(":/Graphics/Images/RenameRundown.png"),*/ "Edit...", this, SLOT(editRundown()));
}

bool RundownWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(event);
        if (keyEvent->key() >= 49 && keyEvent->key() <= 57 && keyEvent->modifiers() == Qt::AltModifier) // [1-9]
            return selectTab(keyEvent->text().toInt());
    }
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::NewRundown))
    {
        RundownTreeWidget* widget = new RundownTreeWidget(this);

        int index = this->tabWidgetRundown->addTab(widget, QIcon(":/Graphics/Images/TabSplitter.png"), Rundown::DEFAULT_NAME);
        this->tabWidgetRundown->setCurrentIndex(index);

        if (this->tabWidgetRundown->count() == Rundown::MAX_NUMBER_OF_RUNDONWS)
            EventManager::getInstance().fireNewRundownMenuEvent(false);

        return true;
    }
    else if (event->type() == static_cast<QEvent::Type>(Enum::EventType::DeleteRundown))
    {
        if (this->tabWidgetRundown->count() <= Rundown::MAX_NUMBER_OF_RUNDONWS)
            EventManager::getInstance().fireNewRundownMenuEvent(true);

        DeleteRundownEvent* removeRundownEvent = dynamic_cast<DeleteRundownEvent*>(event);

        // Delete the page widget, which automatically removes the tab as well.
        delete this->tabWidgetRundown->widget(removeRundownEvent->getIndex());

        return true;
    }
    else if (event->type() == static_cast<QEvent::Type>(Enum::EventType::OpenRundown))
    {
        RundownTreeWidget* widget = new RundownTreeWidget(this);
        widget->openRundown();

        int index = this->tabWidgetRundown->addTab(widget, QIcon(":/Graphics/Images/TabSplitter.png"), "");
        this->tabWidgetRundown->setCurrentIndex(index);

        if (this->tabWidgetRundown->count() == Rundown::MAX_NUMBER_OF_RUNDONWS)
            EventManager::getInstance().fireNewRundownMenuEvent(false);

        return true;
    }
    else if (event->type() == static_cast<QEvent::Type>(Enum::EventType::SaveRundown))
    {
        SaveRundownEvent* saveRundownEvent = dynamic_cast<SaveRundownEvent*>(event);
        dynamic_cast<RundownTreeWidget*>(this->tabWidgetRundown->currentWidget())->saveRundown(saveRundownEvent->getSaveAs());

        return true;
    }
    else if (event->type() == static_cast<QEvent::Type>(Enum::EventType::ActiveRundownChanged))
    {
        ActiveRundownChangedEvent* activeRundownChangedEvent = dynamic_cast<ActiveRundownChangedEvent*>(event);

        QFileInfo info(activeRundownChangedEvent->getPath());
        this->tabWidgetRundown->setTabText(this->tabWidgetRundown->currentIndex(), info.baseName());

        return true;
    }

    return QObject::eventFilter(target, event);
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
        EventManager::getInstance().fireDeleteRundownEvent(index);
}

void RundownWidget::currentChanged(int index)
{
    for (int i = 0; i < this->tabWidgetRundown->count(); i++)
        dynamic_cast<RundownTreeWidget*>(this->tabWidgetRundown->widget(i))->setActive(false);

    dynamic_cast<RundownTreeWidget*>(this->tabWidgetRundown->widget(index))->setActive(true);
}
