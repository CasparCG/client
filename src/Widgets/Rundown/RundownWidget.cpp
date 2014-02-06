#include "RundownWidget.h"
#include "RundownTreeWidget.h"

#include "EventManager.h"
#include "Events/Rundown/DeleteRundownEvent.h"
#include "Events/Rundown/ActiveRundownChangedEvent.h"

#include <QtCore/QUuid>
#include <QtCore/QDebug>
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

    QToolButton* toolButtonRundownDropdown = new QToolButton(this);
    toolButtonRundownDropdown->setObjectName("toolButtonRundownDropdown");
    toolButtonRundownDropdown->setMenu(this->contextMenuRundownDropdown);
    toolButtonRundownDropdown->setPopupMode(QToolButton::InstantPopup);
    this->tabWidgetRundown->setCornerWidget(toolButtonRundownDropdown);
    //this->tabWidgetRundown->setTabIcon(0, QIcon(":/Graphics/Images/TabSplitter.png"));

    RundownTreeWidget* widget = new RundownTreeWidget(this);
    int index = this->tabWidgetRundown->addTab(widget, QIcon(":/Graphics/Images/TabSplitter.png"), Rundown::DEFAULT_NAME);
    this->tabWidgetRundown->setCurrentIndex(index);

    qApp->installEventFilter(this);
}

void RundownWidget::setupMenus()
{
    this->contextMenuRundownDropdown = new QMenu(this);
    this->contextMenuRundownDropdown->setTitle("Rundown Dropdown");
    this->contextMenuRundownDropdown->addAction(/*QIcon(":/Graphics/Images/RenameRundown.png"),*/ "New Rundown", this, SLOT(newRundown()));
    this->contextMenuRundownDropdown->addAction(/*QIcon(":/Graphics/Images/RenameRundown.png"),*/ "Open Rundown...", this, SLOT(openRundown()));
    this->contextMenuRundownDropdown->addSeparator();
    this->contextMenuRundownDropdown->addAction(/*QIcon(":/Graphics/Images/RenameRundown.png"),*/ "Save", this, SLOT(saveRundown()));
    this->contextMenuRundownDropdown->addAction(/*QIcon(":/Graphics/Images/RenameRundown.png"),*/ "Save As...", this, SLOT(saveAsRundown()));
    this->contextMenuRundownDropdown->addSeparator();
    this->contextMenuRundownDropdown->addAction(/*QIcon(":/Graphics/Images/RenameRundown.png"),*/ "Toggle Compact View", this, SLOT(toggleCompactView()));
}

bool RundownWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(event);
        if (keyEvent->key() >= 49 && keyEvent->key() <= 57 && keyEvent->modifiers() == Qt::AltModifier) // [1-9]
            return selectTab(QString(QChar(keyEvent->key())).toInt());
    }
    if (event->type() == static_cast<QEvent::Type>(Event::EventType::NewRundown))
    {
        RundownTreeWidget* widget = new RundownTreeWidget(this);

        int index = this->tabWidgetRundown->addTab(widget, QIcon(":/Graphics/Images/TabSplitter.png"), Rundown::DEFAULT_NAME);
        this->tabWidgetRundown->setCurrentIndex(index);

        if (this->tabWidgetRundown->count() == Rundown::MAX_NUMBER_OF_RUNDONWS)
            EventManager::getInstance().fireNewRundownMenuEvent(false);

        return true;
    }
    else if (event->type() == static_cast<QEvent::Type>(Event::EventType::DeleteRundown))
    {
        if (this->tabWidgetRundown->count() <= Rundown::MAX_NUMBER_OF_RUNDONWS)
            EventManager::getInstance().fireNewRundownMenuEvent(true);

        DeleteRundownEvent* removeRundownEvent = dynamic_cast<DeleteRundownEvent*>(event);

        // Delete the page widget, which automatically removes the tab as well.
        delete this->tabWidgetRundown->widget(removeRundownEvent->getIndex());

        return true;
    }
    else if (event->type() == static_cast<QEvent::Type>(Event::EventType::OpenRundown))
    {
        QString path = QFileDialog::getOpenFileName(this, "Open Rundown", "", "Rundown (*.xml)");
        if (!path.isEmpty())
        {
            EventManager::getInstance().fireStatusbarEvent("Opening rundown...");
            EventManager::getInstance().fireProcessEvent();

            RundownTreeWidget* widget = new RundownTreeWidget(this);
            widget->openRundown(path);

            QFileInfo info(path);
            int index = this->tabWidgetRundown->addTab(widget, QIcon(":/Graphics/Images/TabSplitter.png"), info.baseName());
            this->tabWidgetRundown->setCurrentIndex(index);

            if (this->tabWidgetRundown->count() == Rundown::MAX_NUMBER_OF_RUNDONWS)
                EventManager::getInstance().fireNewRundownMenuEvent(false);

            EventManager::getInstance().fireActiveRundownChangedEvent(path);
            EventManager::getInstance().fireStatusbarEvent("");
            EventManager::getInstance().fireProcessEvent();
        }

        return true;
    }
    else if (event->type() == static_cast<QEvent::Type>(Event::EventType::SaveRundown))
    {
        SaveRundownEvent* saveRundownEvent = dynamic_cast<SaveRundownEvent*>(event);
        dynamic_cast<RundownTreeWidget*>(this->tabWidgetRundown->currentWidget())->saveRundown(saveRundownEvent->getSaveAs());

        return true;
    }
    else if (event->type() == static_cast<QEvent::Type>(Event::EventType::ActiveRundownChanged))
    {
        ActiveRundownChangedEvent* activeRundownChangedEvent = dynamic_cast<ActiveRundownChangedEvent*>(event);

        QFileInfo info(activeRundownChangedEvent->getPath());
        this->tabWidgetRundown->setTabText(this->tabWidgetRundown->currentIndex(), info.baseName());

        return true;
    }
    else if (event->type() == static_cast<QEvent::Type>(Event::EventType::RundownIsChanged))
    {
        RundownIsChangedEvent* rundownIsChangedEvent = dynamic_cast<RundownIsChangedEvent*>(event);

        if (rundownIsChangedEvent->getChanged())
        {
            //this->tabWidgetRundown->setTabText(this->tabWidgetRundown->currentIndex(),
            //                                   this->tabWidgetRundown->tabText(this->tabWidgetRundown->currentIndex()) + " *");

            //EventManager::getInstance().fireActiveRundownChangedEvent(this->activeRundown);
        }

        return true;
    }

    return QObject::eventFilter(target, event);
}

void RundownWidget::newRundown()
{
    EventManager::getInstance().fireNewRundownEvent();
}

void RundownWidget::openRundown()
{
    EventManager::getInstance().fireOpenRundownEvent();
}

void RundownWidget::saveRundown()
{
    EventManager::getInstance().fireSaveRundownEvent(false);
}

void RundownWidget::saveAsRundown()
{
    EventManager::getInstance().fireSaveRundownEvent(true);
}

void RundownWidget::toggleCompactView()
{
    EventManager::getInstance().fireToggleCompactViewEvent();
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

void RundownWidget::gpiBindingChanged(int gpiPort, Playout::PlayoutType::Type binding)
{
    for (int i = 0; i < this->tabWidgetRundown->count(); i++)
        dynamic_cast<RundownTreeWidget*>(this->tabWidgetRundown->widget(i))->gpiBindingChanged(gpiPort, binding);
}
