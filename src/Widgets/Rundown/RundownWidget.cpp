#include "RundownWidget.h"
#include "RundownTreeWidget.h"
#include "OpenRundownFromUrlDialog.h"

#include "EventManager.h"
#include "DatabaseManager.h"
#include "Events/Rundown/CompactViewEvent.h"
#include "Events/Rundown/CopyItemPropertiesEvent.h"
#include "Events/Rundown/PasteItemPropertiesEvent.h"
#include "Events/Rundown/AllowRemoteTriggeringEvent.h"
#include "Events/Rundown/InsertRepositoryChangesEvent.h"

#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QUuid>

#include <QtGui/QIcon>

#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QToolButton>

RundownWidget::RundownWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUi(this);
    setupMenus();

    RundownTreeWidget* widget = new RundownTreeWidget(this);
    int index = this->tabWidgetRundown->addTab(widget/*, QIcon(":/Graphics/Images/TabSplitter.png")*/, Rundown::DEFAULT_NAME);
    this->tabWidgetRundown->setTabToolTip(index, Rundown::DEFAULT_NAME);
    this->tabWidgetRundown->setCurrentIndex(index);

    QObject::connect(&EventManager::getInstance(), SIGNAL(newRundownMenu(const NewRundownMenuEvent&)), this, SLOT(newRundownMenu(const NewRundownMenuEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(openRundownMenu(const OpenRundownMenuEvent&)), this, SLOT(openRundownMenu(const OpenRundownMenuEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(openRundownFromUrlMenu(const OpenRundownFromUrlMenuEvent&)), this, SLOT(openRundownFromUrlMenu(const OpenRundownFromUrlMenuEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(newRundown(const NewRundownEvent&)), this, SLOT(newRundown(const NewRundownEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(compactView(const CompactViewEvent&)), this, SLOT(compactView(const CompactViewEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(allowRemoteTriggering(const AllowRemoteTriggeringEvent&)), this, SLOT(allowRemoteTriggering(const AllowRemoteTriggeringEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(repositoryRundown(const RepositoryRundownEvent&)), this, SLOT(repositoryRundown(const RepositoryRundownEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(closeRundown(const CloseRundownEvent&)), this, SLOT(closeRundown(const CloseRundownEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(deleteRundown(const DeleteRundownEvent&)), this, SLOT(deleteRundown(const DeleteRundownEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(openRundown(const OpenRundownEvent&)), this, SLOT(openRundown(const OpenRundownEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(openRundownFromUrl(const OpenRundownFromUrlEvent&)), this, SLOT(openRundownFromUrl(const OpenRundownFromUrlEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(saveRundown(const SaveRundownEvent&)), this, SLOT(saveRundown(const SaveRundownEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(activeRundownChanged(const ActiveRundownChangedEvent&)), this, SLOT(activeRundownChanged(const ActiveRundownChangedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(reloadRundown(const ReloadRundownEvent&)), this, SLOT(reloadRundown(const ReloadRundownEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(markItemAsUsed(const MarkItemAsUsedEvent&)), this, SLOT(markItemAsUsed(const MarkItemAsUsedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(markItemAsUnused(const MarkItemAsUnusedEvent&)), this, SLOT(markItemAsUnused(const MarkItemAsUnusedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(markAllItemsAsUsed(const MarkAllItemsAsUsedEvent&)), this, SLOT(markAllItemsAsUsed(const MarkAllItemsAsUsedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(markAllItemsAsUnused(const MarkAllItemsAsUnusedEvent&)), this, SLOT(markAllItemsAsUnused(const MarkAllItemsAsUnusedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(reloadRundownMenu(const ReloadRundownMenuEvent&)), this, SLOT(reloadRundownMenu(const ReloadRundownMenuEvent&)));
}

void RundownWidget::setupMenus()
{
    this->openRecentMenu = new QMenu(this);
    this->openRecentMenu->setTitle("Open Recent Rundown");

    QObject::connect(this->openRecentMenu, SIGNAL(aboutToShow()), this, SLOT(refreshOpenRecent()));
    QObject::connect(this->openRecentMenu, SIGNAL(triggered(QAction*)), this, SLOT(openRecentMenuActionTriggered(QAction*)));

    this->contextMenuMark = new QMenu(this);
    this->contextMenuMark->setTitle("Mark Item");
    this->contextMenuMark->addAction(/*QIcon(":/Graphics/Images/RenameRundown.png"),*/ "As Used", this, SLOT(markItemAsUsedInRundown()));
    this->contextMenuMark->addAction(/*QIcon(":/Graphics/Images/RenameRundown.png"),*/ "As Unused", this, SLOT(markItemAsUnusedInRundown()));
    this->contextMenuMark->addAction(/*QIcon(":/Graphics/Images/RenameRundown.png"),*/ "All as Used", this, SLOT(markAllItemsAsUsedInRundown()));
    this->contextMenuMark->addAction(/*QIcon(":/Graphics/Images/RenameRundown.png"),*/ "All as Unused", this, SLOT(markAllItemsAsUnusedInRundown()));

    this->contextMenuRundownDropdown = new QMenu(this);
    this->contextMenuRundownDropdown->setTitle("Dropdown");
    this->newRundownAction = this->contextMenuRundownDropdown->addAction("New Rundown", this, SLOT(createNewRundown()));
    this->openRundownAction = this->contextMenuRundownDropdown->addAction("Open Rundown...", this, SLOT(openRundownFromDisk()));
    this->openRundownFromUrlAction = this->contextMenuRundownDropdown->addAction("Open Rundown from repository...", this, SLOT(openRundownFromRepo()));
    this->contextMenuRundownDropdown->addSeparator();
    this->openRecentMenuAction = this->contextMenuRundownDropdown->addMenu(this->openRecentMenu);
    this->contextMenuRundownDropdown->addSeparator();
    this->saveAction = this->contextMenuRundownDropdown->addAction("Save", this, SLOT(saveRundownToDisk()));
    this->saveAsAction = this->contextMenuRundownDropdown->addAction("Save As...", this, SLOT(saveAsRundownToDisk()));
    this->contextMenuRundownDropdown->addSeparator();
    this->contextMenuRundownDropdown->addMenu(this->contextMenuMark);
    this->contextMenuRundownDropdown->addSeparator();
    this->contextMenuRundownDropdown->addAction("Copy Item Properties", this, SLOT(copyItemProperties()));
    this->contextMenuRundownDropdown->addAction("Paste Item Properties", this, SLOT(pasteItemProperties()));
    this->contextMenuRundownDropdown->addSeparator();
    this->compactViewAction = this->contextMenuRundownDropdown->addAction("Compact View");
    this->compactViewAction->setCheckable(true);
    this->allowRemoteTriggeringAction = this->contextMenuRundownDropdown->addAction("Allow Remote Triggering");
    this->allowRemoteTriggeringAction->setCheckable(true);
    this->contextMenuRundownDropdown->addSeparator();
    this->insertRepositoryChangesAction = this->contextMenuRundownDropdown->addAction("Insert Repository Changes", this, SLOT(insertRepositoryChanges()));
    this->insertRepositoryChangesAction->setEnabled(false);
    this->contextMenuRundownDropdown->addSeparator();
    this->reloadRundownAction = this->contextMenuRundownDropdown->addAction("Reload Rundown", this, SLOT(reloadCurrentRundown()));
    this->contextMenuRundownDropdown->addSeparator();
    this->contextMenuRundownDropdown->addAction("Close Rundown", this, SLOT(closeCurrentRundown()));

    QObject::connect(this->compactViewAction, SIGNAL(toggled(bool)), this, SLOT(compactView(bool)));
    QObject::connect(this->allowRemoteTriggeringAction, SIGNAL(toggled(bool)), this, SLOT(remoteTriggering(bool)));

    QToolButton* toolButtonRundownDropdown = new QToolButton(this);
    toolButtonRundownDropdown->setObjectName("toolButtonRundownDropdown");
    toolButtonRundownDropdown->setMenu(this->contextMenuRundownDropdown);
    toolButtonRundownDropdown->setPopupMode(QToolButton::InstantPopup);
    this->tabWidgetRundown->setCornerWidget(toolButtonRundownDropdown);
    //this->tabWidgetRundown->setTabIcon(0, QIcon(":/Graphics/Images/TabSplitter.png"));
}

void RundownWidget::refreshOpenRecent()
{
    foreach (QAction* action, this->openRecentMenu->actions())
        this->openRecentMenu->removeAction(action);

    QList<QString> paths = DatabaseManager::getInstance().getOpenRecent();
    foreach (QString path, paths)
        this->openRecentMenu->addAction(/*QIcon(":/Graphics/Images/OpenRecent.png"),*/ path);

    if (this->openRecentMenu->actions().count() > 0)
    {
        this->openRecentMenu->addSeparator();
        this->openRecentMenu->addAction(/*QIcon(":/Graphics/Images/ClearOpenRecent.png"),*/ "Clear Menu", this, SLOT(clearOpenRecent()));
    }
}

void RundownWidget::openRecentMenuActionTriggered(QAction* action)
{
    if (action->text().contains("Clear"))
        return;

    EventManager::getInstance().fireOpenRundownEvent(OpenRundownEvent(action->text()));
}

void RundownWidget::clearOpenRecent()
{
   DatabaseManager::getInstance().deleteOpenRecent();
}

bool RundownWidget::checkForSaveBeforeQuit()
{
    for (int i = 0; i < this->tabWidgetRundown->count(); i++)
    {
        if (dynamic_cast<RundownTreeWidget*>(this->tabWidgetRundown->widget(i))->checkForSave())
        {
            QMessageBox box(this);
            box.setWindowTitle("Quit Application");
            box.setWindowIcon(QIcon(":/Graphics/Images/CasparCG.png"));
            box.setText(QString("You have unsaved changes in your %1 rundown. Do you want to save it before you quit?").arg(this->tabWidgetRundown->tabText(i)));
            box.setIconPixmap(QPixmap(":/Graphics/Images/Attention.png"));
            box.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            box.buttons().at(0)->setIcon(QIcon());
            box.buttons().at(0)->setFocusPolicy(Qt::NoFocus);
            box.buttons().at(1)->setIcon(QIcon());
            box.buttons().at(1)->setFocusPolicy(Qt::NoFocus);
            box.buttons().at(2)->setIcon(QIcon());
            box.buttons().at(2)->setFocusPolicy(Qt::NoFocus);

            int result = box.exec();
            if (result == QMessageBox::Yes)
                dynamic_cast<RundownTreeWidget*>(this->tabWidgetRundown->widget(i))->saveRundown(false);
            else if (result == QMessageBox::Cancel)
                return false;
        }
    }

    return true;
}

void RundownWidget::reloadRundownMenu(const ReloadRundownMenuEvent& event)
{
    this->reloadRundownAction->setEnabled(event.getEnabled());
}

void RundownWidget::newRundownMenu(const NewRundownMenuEvent& event)
{
    this->newRundownAction->setEnabled(event.getEnabled());
}

void RundownWidget::openRundownMenu(const OpenRundownMenuEvent& event)
{
    this->openRundownAction->setEnabled(event.getEnabled());
}

void RundownWidget::openRundownFromUrlMenu(const OpenRundownFromUrlMenuEvent& event)
{
    this->openRundownFromUrlAction->setEnabled(event.getEnabled());
}

void RundownWidget::newRundown(const NewRundownEvent& event)
{
    Q_UNUSED(event);

    RundownTreeWidget* widget = new RundownTreeWidget(this);
    int index = this->tabWidgetRundown->addTab(widget/*, QIcon(":/Graphics/Images/TabSplitter.png")*/, Rundown::DEFAULT_NAME);
    this->tabWidgetRundown->setCurrentIndex(index);

    if (this->tabWidgetRundown->count() == Rundown::MAX_NUMBER_OF_RUNDONWS)
    {
        EventManager::getInstance().fireNewRundownMenuEvent(NewRundownMenuEvent(false));
        EventManager::getInstance().fireOpenRundownMenuEvent(OpenRundownMenuEvent(false));
        EventManager::getInstance().fireOpenRundownFromUrlMenuEvent(OpenRundownFromUrlMenuEvent(false));
    }
}

void RundownWidget::compactView(const CompactViewEvent& event)
{
    // We do not want to trigger check changed event.
    this->compactViewAction->blockSignals(true);
    this->compactViewAction->setChecked(event.getEnabled());
    this->compactViewAction->blockSignals(false);
}

void RundownWidget::allowRemoteTriggering(const AllowRemoteTriggeringEvent& event)
{
    // We do not want to trigger check changed event.
    this->allowRemoteTriggeringAction->blockSignals(true);
    this->allowRemoteTriggeringAction->setChecked(event.getEnabled());
    this->allowRemoteTriggeringAction->blockSignals(false);

    if (!event.getEnabled())
        this->tabWidgetRundown->setTabIcon(this->tabWidgetRundown->currentIndex(), QIcon());
    else
        this->tabWidgetRundown->setTabIcon(this->tabWidgetRundown->currentIndex(), QIcon(":/Graphics/Images/RemoteTriggeringSmall.png"));
}

void RundownWidget::repositoryRundown(const RepositoryRundownEvent& event)
{
    this->saveAction->setEnabled(!event.getRepositoryRundown());
    this->saveAsAction->setEnabled(!event.getRepositoryRundown());
    this->allowRemoteTriggeringAction->setEnabled(!event.getRepositoryRundown());
    this->insertRepositoryChangesAction->setEnabled(event.getRepositoryRundown());
}

void RundownWidget::closeRundown(const CloseRundownEvent& event)
{
    Q_UNUSED(event);

    if (this->tabWidgetRundown->count() > 1)
        EventManager::getInstance().fireDeleteRundownEvent(DeleteRundownEvent(this->tabWidgetRundown->currentIndex()));
}

void RundownWidget::deleteRundown(const DeleteRundownEvent& event)
{
    if (dynamic_cast<RundownTreeWidget*>(this->tabWidgetRundown->widget(event.getIndex()))->checkForSave())
    {
        QMessageBox box(this);
        box.setWindowTitle("Close Rundown");
        box.setWindowIcon(QIcon(":/Graphics/Images/CasparCG.png"));
        box.setText(QString("You have unsaved changes in your %1 rundown. Do you want to save before you close?").arg(this->tabWidgetRundown->tabText(event.getIndex())));
        box.setIconPixmap(QPixmap(":/Graphics/Images/Attention.png"));
        box.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        box.buttons().at(0)->setIcon(QIcon());
        box.buttons().at(0)->setFocusPolicy(Qt::NoFocus);
        box.buttons().at(1)->setIcon(QIcon());
        box.buttons().at(1)->setFocusPolicy(Qt::NoFocus);
        box.buttons().at(2)->setIcon(QIcon());
        box.buttons().at(2)->setFocusPolicy(Qt::NoFocus);

        int result = box.exec();
        if (result == QMessageBox::Yes)
            dynamic_cast<RundownTreeWidget*>(this->tabWidgetRundown->widget(event.getIndex()))->saveRundown(false);
        else if (result == QMessageBox::Cancel)
            return;
    }

    // Delete the page widget, which automatically removes the tab as well.
    delete this->tabWidgetRundown->widget(event.getIndex());

    if (this->tabWidgetRundown->count() <= Rundown::MAX_NUMBER_OF_RUNDONWS)
    {
        EventManager::getInstance().fireNewRundownMenuEvent(NewRundownMenuEvent(true));
        EventManager::getInstance().fireOpenRundownMenuEvent(OpenRundownMenuEvent(true));
        EventManager::getInstance().fireOpenRundownFromUrlMenuEvent(OpenRundownFromUrlMenuEvent(true));
    }
}

void RundownWidget::openRundown(const OpenRundownEvent& event)
{
    QString path = "";

    if (event.getPath().isEmpty()){
        QList<QString> paths = DatabaseManager::getInstance().getOpenRecent();
        if(paths.count() > 0){
                path = paths.at(0);
                QFileInfo fi(path);
                path = fi.absolutePath();
        }else{
                path = QDir::homePath();
        }
        path = QFileDialog::getOpenFileName(this, "Open Rundown", path , "Rundown (*.xml)");
    }else
        path = event.getPath();

    if (!path.isEmpty())
    {
        RundownTreeWidget* widget = new RundownTreeWidget(this);       

        int index = this->tabWidgetRundown->addTab(widget/*, QIcon(":/Graphics/Images/TabSplitter.png")*/, Rundown::DEFAULT_NAME);
        this->tabWidgetRundown->setTabToolTip(index, path);
        this->tabWidgetRundown->setCurrentIndex(index);

        EventManager::getInstance().fireActiveRundownChangedEvent(ActiveRundownChangedEvent(path));

        widget->openRundown(path);

        if (this->tabWidgetRundown->count() == Rundown::MAX_NUMBER_OF_RUNDONWS)
        {
            EventManager::getInstance().fireNewRundownMenuEvent(NewRundownMenuEvent(false));
            EventManager::getInstance().fireOpenRundownMenuEvent(OpenRundownMenuEvent(false));
            EventManager::getInstance().fireOpenRundownFromUrlMenuEvent(OpenRundownFromUrlMenuEvent(false));
        }
    }
}

void RundownWidget::openRundownFromUrl(const OpenRundownFromUrlEvent& event)
{
    QString path = "";

    if (event.getPath().isEmpty())
    {
        OpenRundownFromUrlDialog* dialog = new OpenRundownFromUrlDialog(this);
        if (dialog->exec() == QDialog::Accepted)
            path = dialog->getPath();
    }

    if (!path.isEmpty())
    {
        RundownTreeWidget* widget = new RundownTreeWidget(this);

        int index = this->tabWidgetRundown->addTab(widget/*, QIcon(":/Graphics/Images/TabSplitter.png")*/, Rundown::DEFAULT_NAME);
        this->tabWidgetRundown->setTabToolTip(index, path);
        this->tabWidgetRundown->setCurrentIndex(index);

        EventManager::getInstance().fireActiveRundownChangedEvent(ActiveRundownChangedEvent(path));

        widget->openRundownFromUrl(path);

        if (this->tabWidgetRundown->count() == Rundown::MAX_NUMBER_OF_RUNDONWS)
        {
            EventManager::getInstance().fireNewRundownMenuEvent(NewRundownMenuEvent(false));
            EventManager::getInstance().fireOpenRundownMenuEvent(OpenRundownMenuEvent(false));
            EventManager::getInstance().fireOpenRundownFromUrlMenuEvent(OpenRundownFromUrlMenuEvent(false));
        } 
    }
}

void RundownWidget::markItemAsUsed(const MarkItemAsUsedEvent& event)
{
    Q_UNUSED(event);

    dynamic_cast<RundownTreeWidget*>(this->tabWidgetRundown->currentWidget())->setUsed(true);
}

void RundownWidget::markItemAsUnused(const MarkItemAsUnusedEvent& event)
{
    Q_UNUSED(event);

    dynamic_cast<RundownTreeWidget*>(this->tabWidgetRundown->currentWidget())->setUsed(false);
}

void RundownWidget::markAllItemsAsUsed(const MarkAllItemsAsUsedEvent& event)
{
    Q_UNUSED(event);

    dynamic_cast<RundownTreeWidget*>(this->tabWidgetRundown->currentWidget())->setAllUsed(true);
}

void RundownWidget::markAllItemsAsUnused(const MarkAllItemsAsUnusedEvent& event)
{
    Q_UNUSED(event);

    dynamic_cast<RundownTreeWidget*>(this->tabWidgetRundown->currentWidget())->setAllUsed(false);
}

void RundownWidget::reloadRundown(const ReloadRundownEvent& event)
{
    Q_UNUSED(event);

    EventManager::getInstance().fireStatusbarEvent(StatusbarEvent("Reloading rundown..."));

    dynamic_cast<RundownTreeWidget*>(this->tabWidgetRundown->currentWidget())->reloadRundown();

    EventManager::getInstance().fireStatusbarEvent(StatusbarEvent(""));
}

void RundownWidget::saveRundown(const SaveRundownEvent& event)
{
    dynamic_cast<RundownTreeWidget*>(this->tabWidgetRundown->currentWidget())->saveRundown(event.getSaveAs());
}

void RundownWidget::activeRundownChanged(const ActiveRundownChangedEvent& event)
{
    QFileInfo info(event.getPath());
    this->tabWidgetRundown->setTabText(this->tabWidgetRundown->currentIndex(), (event.getPath().startsWith("http") == true) ? event.getPath().split("/").last() : info.baseName());
}

void RundownWidget::createNewRundown()
{
    EventManager::getInstance().fireNewRundownEvent(NewRundownEvent());
}

void RundownWidget::openRundownFromDisk()
{
    EventManager::getInstance().fireOpenRundownEvent(OpenRundownEvent());
}

void RundownWidget::openRundownFromRepo()
{
    EventManager::getInstance().fireOpenRundownFromUrlEvent(OpenRundownFromUrlEvent());
}

void RundownWidget::saveRundownToDisk()
{
    EventManager::getInstance().fireSaveRundownEvent(SaveRundownEvent(false));
}

void RundownWidget::saveAsRundownToDisk()
{
    EventManager::getInstance().fireSaveRundownEvent(SaveRundownEvent(true));
}

void RundownWidget::copyItemProperties()
{
    EventManager::getInstance().fireCopyItemPropertiesEvent(CopyItemPropertiesEvent());
}

void RundownWidget::pasteItemProperties()
{
    EventManager::getInstance().firePasteItemPropertiesEvent(PasteItemPropertiesEvent());
}

void RundownWidget::reloadCurrentRundown()
{
    EventManager::getInstance().fireReloadRundownEvent(ReloadRundownEvent());
}

void RundownWidget::closeCurrentRundown()
{
    EventManager::getInstance().fireCloseRundownEvent(CloseRundownEvent());
}

void RundownWidget::markItemAsUsedInRundown()
{
    EventManager::getInstance().fireMarkItemAsUsedEvent(MarkItemAsUsedEvent());
}

void RundownWidget::markItemAsUnusedInRundown()
{
    EventManager::getInstance().fireMarkItemAsUnusedEvent(MarkItemAsUnusedEvent());
}

void RundownWidget::markAllItemsAsUsedInRundown()
{
    EventManager::getInstance().fireMarkAllItemsAsUsedEvent(MarkAllItemsAsUsedEvent());
}

void RundownWidget::markAllItemsAsUnusedInRundown()
{
    EventManager::getInstance().fireMarkAllItemsAsUnusedEvent(MarkAllItemsAsUnusedEvent());
}

void RundownWidget::compactView(bool enabled)
{
    EventManager::getInstance().fireCompactViewEvent(CompactViewEvent(enabled));
}

void RundownWidget::remoteTriggering(bool enabled)
{
    EventManager::getInstance().fireAllowRemoteTriggeringEvent(AllowRemoteTriggeringEvent(enabled));
}

void RundownWidget::insertRepositoryChanges()
{
    EventManager::getInstance().fireInsertRepositoryChangesEvent(InsertRepositoryChangesEvent());
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
        EventManager::getInstance().fireDeleteRundownEvent(DeleteRundownEvent(index));
}

void RundownWidget::currentChanged(int index)
{
    for (int i = 0; i < this->tabWidgetRundown->count(); i++)
        dynamic_cast<RundownTreeWidget*>(this->tabWidgetRundown->widget(i))->setActive(false);

    dynamic_cast<RundownTreeWidget*>(this->tabWidgetRundown->widget(index))->setActive(true);

    bool allowRemoteTriggering = dynamic_cast<RundownTreeWidget*>(this->tabWidgetRundown->widget(index))->getAllowRemoteTriggering();

    // We do not want to trigger check changed event.
    this->allowRemoteTriggeringAction->blockSignals(true);
    this->allowRemoteTriggeringAction->setChecked(allowRemoteTriggering);
    this->allowRemoteTriggeringAction->blockSignals(false);
}

void RundownWidget::gpiBindingChanged(int gpiPort, Playout::PlayoutType binding)
{
    for (int i = 0; i < this->tabWidgetRundown->count(); i++)
        dynamic_cast<RundownTreeWidget*>(this->tabWidgetRundown->widget(i))->gpiBindingChanged(gpiPort, binding);
}
