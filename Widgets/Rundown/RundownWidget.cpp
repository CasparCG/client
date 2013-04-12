#include "RundownWidget.h"
#include "RundownTreeWidget.h"
#include "AddRundownDialog.h"

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

    RundownTreeWidget* widget = new RundownTreeWidget(this);
    widget->setActive(true);
    int index = this->tabWidgetRundown->addTab(widget, QIcon(":/Graphics/Images/TabSplitter.png"), QString("Rundown %1").arg(this->tabCount++));
    this->tabWidgetRundown->setCurrentIndex(index);

    qApp->installEventFilter(this);
}

void RundownWidget::setupMenus()
{
    this->contextMenuRundownDropdown = new QMenu(this);
    this->contextMenuRundownDropdown->setTitle("Rundown Dropdown");
    this->contextMenuRundownDropdown->addAction(/*QIcon(":/Graphics/Images/AddRundown.png"),*/ "Add rundown...", this, SLOT(addRundown()));
    this->contextMenuRundownDropdown->addAction(/*QIcon(":/Graphics/Images/AddRundown.png"),*/ "Remove rundown", this, SLOT(removeRundown()));
    this->contextMenuRundownDropdown->addSeparator();
    this->contextMenuRundownDropdown->addAction(/*QIcon(":/Graphics/Images/RenameRundown.png"),*/ "Edit rundown...", this, SLOT(renameRundown()));
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
        AddRundownDialog* dialog = new AddRundownDialog(this);
        dialog->setName(QString("Rundown %1").arg(this->tabCount));
        if (dialog->exec() == QDialog::Accepted)
        {
            RundownTreeWidget* widget = new RundownTreeWidget(this);
            widget->setActive(true);

            int index = this->tabWidgetRundown->addTab(widget, QIcon(":/Graphics/Images/TabSplitter.png"), QString("%1").arg(dialog->getName()));
            this->tabWidgetRundown->setCurrentIndex(index);

            if (this->tabWidgetRundown->count() == Rundown::MAX_NUMBER_OF_RUNDONWS)
                this->contextMenuRundownDropdown->actions().at(0)->setEnabled(false);

            this->tabCount++;
        }

        return true;
    }
    else if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RemoveRundown))
    {
        if (this->tabWidgetRundown->count() <= Rundown::MAX_NUMBER_OF_RUNDONWS)
            this->contextMenuRundownDropdown->actions().at(0)->setEnabled(true);

        RemoveRundownEvent* removeRundownEvent = dynamic_cast<RemoveRundownEvent*>(event);

        // Delete the page widget, which automatically removes the tab as well.
        delete this->tabWidgetRundown->widget(removeRundownEvent->getIndex());

        this->tabCount--;

        return true;
    }

    return QObject::eventFilter(target, event);
}

void RundownWidget::addRundown()
{
    if (this->tabWidgetRundown->count() < Rundown::MAX_NUMBER_OF_RUNDONWS)
        EventManager::getInstance().fireAddRundownEvent();
}

void RundownWidget::removeRundown()
{
    if (this->tabWidgetRundown->count() > 1)
        EventManager::getInstance().fireRemoveRundownEvent(this->tabWidgetRundown->currentIndex());
}

void RundownWidget::renameRundown()
{
    AddRundownDialog* dialog = new AddRundownDialog(this);
    dialog->setName(this->tabWidgetRundown->tabText(this->tabWidgetRundown->currentIndex()));
    dialog->setEditMode(true);
    if (dialog->exec() == QDialog::Accepted)
    {
        this->tabWidgetRundown->setTabText(this->tabWidgetRundown->currentIndex(), dialog->getName());
    }
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
