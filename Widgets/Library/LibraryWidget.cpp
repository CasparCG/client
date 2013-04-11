#include "LibraryWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/LibraryItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QPoint>
#include <QtCore/QSharedDataPointer>

#include <QtGui/QApplication>
#include <QtGui/QClipboard>
#include <QtGui/QColor>
#include <QtGui/QIcon>
#include <QtGui/QTreeWidgetItem>

LibraryWidget::LibraryWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUi(this);
    setupUiMenu();

    this->treeWidgetAudio->setColumnHidden(1, true);
    this->treeWidgetAudio->setColumnHidden(2, true);
    this->treeWidgetAudio->setColumnHidden(3, true);
    this->treeWidgetAudio->setColumnHidden(4, true);
    this->treeWidgetAudio->setColumnHidden(5, true);

    this->treeWidgetImage->setColumnHidden(1, true);
    this->treeWidgetImage->setColumnHidden(2, true);
    this->treeWidgetImage->setColumnHidden(3, true);
    this->treeWidgetImage->setColumnHidden(4, true);
    this->treeWidgetImage->setColumnHidden(5, true);

    this->treeWidgetTemplate->setColumnHidden(1, true);
    this->treeWidgetTemplate->setColumnHidden(2, true);
    this->treeWidgetTemplate->setColumnHidden(3, true);
    this->treeWidgetTemplate->setColumnHidden(4, true);
    this->treeWidgetTemplate->setColumnHidden(5, true);

    this->treeWidgetVideo->setColumnHidden(1, true);
    this->treeWidgetVideo->setColumnHidden(2, true);
    this->treeWidgetVideo->setColumnHidden(3, true);
    this->treeWidgetVideo->setColumnHidden(4, true);
    this->treeWidgetVideo->setColumnHidden(5, true);

    this->treeWidgetData->setColumnHidden(1, true);
    this->treeWidgetData->setColumnHidden(2, true);
    this->treeWidgetData->setColumnHidden(3, true);
    this->treeWidgetData->setColumnHidden(4, true);
    this->treeWidgetData->setColumnHidden(5, true);

    connect(this->treeWidgetAudio, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(customContextMenuRequested(const QPoint &)));
    connect(this->treeWidgetTemplate, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(customContextMenuRequested(const QPoint &)));
    connect(this->treeWidgetVideo, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(customContextMenuRequested(const QPoint &)));
    connect(this->treeWidgetImage, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(customContextMenuImageRequested(const QPoint &)));

    qApp->installEventFilter(this);

    EventManager::getInstance().fireMediaChangedEvent();
    EventManager::getInstance().fireTemplateChangedEvent();
    EventManager::getInstance().fireDataChangedEvent();
}

void LibraryWidget::setupUiMenu()
{
    this->contextMenu = new QMenu(this);
    this->contextMenu->addAction("Add item");

    this->contextMenuImage = new QMenu(this);
    this->contextMenuImage->addAction("Add as image");
    this->contextMenuImage->addAction("Add as image scroller");

    connect(this->contextMenu, SIGNAL(triggered(QAction*)), this, SLOT(contextMenuTriggered(QAction*)));
    connect(this->contextMenuImage, SIGNAL(triggered(QAction*)), this, SLOT(contextMenuImageTriggered(QAction*)));
}

bool LibraryWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::MediaChanged))
    {
        MediaChangedEvent* mediaChangedEvent = dynamic_cast<MediaChangedEvent*>(event);

        // TODO: Only add / remove necessary items.
        this->treeWidgetAudio->clear();
        this->treeWidgetImage->clear();
        this->treeWidgetVideo->clear();
        this->treeWidgetAudio->clearSelection();
        this->treeWidgetImage->clearSelection();
        this->treeWidgetVideo->clearSelection();

        QList<LibraryModel> models;
        if (this->lineEditFilter->text().isEmpty())
            models = DatabaseManager::getInstance().getLibraryMedia();
        else
            models = DatabaseManager::getInstance().getLibraryMediaByFilter(this->lineEditFilter->text());

        if (models.count() > 0)
        {
            foreach (LibraryModel model, models)
            {
                if (model.getType() == "AUDIO")
                {
                    QTreeWidgetItem* widget = new QTreeWidgetItem(this->treeWidgetAudio);
                    widget->setIcon(0, QIcon(":/Graphics/Images/Audio.png"));
                    widget->setText(0, model.getName());
                    widget->setText(1, QString("%1").arg(model.getId()));
                    widget->setText(2, model.getLabel());
                    widget->setText(3, model.getDeviceName());
                    widget->setText(4, model.getType());
                    widget->setText(5, QString("%1").arg(model.getThumbnailId()));
                }    
                else if (model.getType() == "STILL")
                {
                    QTreeWidgetItem* widget = new QTreeWidgetItem(this->treeWidgetImage);
                    widget->setIcon(0, QIcon(":/Graphics/Images/Still.png"));
                    widget->setText(0, model.getName());
                    widget->setText(1, QString("%1").arg(model.getId()));
                    widget->setText(2, model.getLabel());
                    widget->setText(3, model.getDeviceName());
                    widget->setText(4, model.getType());
                    widget->setText(5, QString("%1").arg(model.getThumbnailId()));
                }
                else if (model.getType() == "MOVIE")
                {
                    QTreeWidgetItem* widget = new QTreeWidgetItem(this->treeWidgetVideo);
                    widget->setIcon(0, QIcon(":/Graphics/Images/Movie.png"));
                    widget->setText(0, model.getName());
                    widget->setText(1, QString("%1").arg(model.getId()));
                    widget->setText(2, model.getLabel());
                    widget->setText(3, model.getDeviceName());
                    widget->setText(4, model.getType());
                    widget->setText(5, QString("%1").arg(model.getThumbnailId()));
                }
            }
        }

        this->toolBoxLibrary->setItemText(0, QString("Audio (%1)").arg(this->treeWidgetAudio->topLevelItemCount()));
        this->toolBoxLibrary->setItemText(1, QString("Images (%1)").arg(this->treeWidgetImage->topLevelItemCount()));
        this->toolBoxLibrary->setItemText(3, QString("Videos (%1)").arg(this->treeWidgetVideo->topLevelItemCount()));
    }
    else if(event->type() == static_cast<QEvent::Type>(Enum::EventType::TemplateChanged))
    {
        TemplateChangedEvent* templateChangedEvent = dynamic_cast<TemplateChangedEvent*>(event);

        // TODO: Only add / remove necessary items.
        this->treeWidgetTemplate->clear();
        this->treeWidgetTemplate->clearSelection();

        QList<LibraryModel> models;
        if (this->lineEditFilter->text().isEmpty())
            models = DatabaseManager::getInstance().getLibraryTemplate();
        else
            models = DatabaseManager::getInstance().getLibraryTemplateByFilter(this->lineEditFilter->text());

        if (models.count() > 0)
        {
            foreach (LibraryModel model, models)
            {
                QTreeWidgetItem* widget = new QTreeWidgetItem(this->treeWidgetTemplate);
                widget->setIcon(0, QIcon(":/Graphics/Images/Template.png"));
                widget->setText(0, model.getName());
                widget->setText(1, QString("%1").arg(model.getId()));
                widget->setText(2, model.getLabel());
                widget->setText(3, model.getDeviceName());
                widget->setText(4, model.getType());
                widget->setText(5, QString("%1").arg(model.getThumbnailId()));
            }
        }

        this->toolBoxLibrary->setItemText(2, QString("Templates (%1)").arg(this->treeWidgetTemplate->topLevelItemCount()));
    }
    else if(event->type() == static_cast<QEvent::Type>(Enum::EventType::DataChanged))
    {
        DataChangedEvent* dataChangedEvent = dynamic_cast<DataChangedEvent*>(event);

        // TODO: Only add / remove necessary items.
        this->treeWidgetData->clear();
        this->treeWidgetData->clearSelection();

        QList<LibraryModel> models;
        if (this->lineEditFilter->text().isEmpty())
            models = DatabaseManager::getInstance().getLibraryData();
        else
            models = DatabaseManager::getInstance().getLibraryDataByFilter(this->lineEditFilter->text());

        if (models.count() > 0)
        {
            foreach (LibraryModel model, models)
            {
                QTreeWidgetItem* widget = new QTreeWidgetItem(this->treeWidgetData);
                widget->setIcon(0, QIcon(":/Graphics/Images/Data.png"));
                widget->setText(0, model.getName());
                widget->setText(1, QString("%1").arg(model.getId()));
                widget->setText(2, model.getLabel());
                widget->setText(3, model.getDeviceName());
                widget->setText(4, model.getType());
                widget->setText(5, QString("%1").arg(model.getThumbnailId()));
            }
        }

        this->toolBoxLibrary->setItemText(4, QString("Stored (%1)").arg(this->treeWidgetData->topLevelItemCount()));
    }

    return QObject::eventFilter(target, event);
}

void LibraryWidget::customContextMenuRequested(const QPoint& point)
{
    if (this->toolBoxLibrary->currentIndex() == Library::AUDIO_PAGE_INDEX)
    {
        if (this->treeWidgetAudio->selectedItems().count() == 0)
            return;

        this->contextMenu->exec(this->treeWidgetAudio->mapToGlobal(point));
    }
    else if (this->toolBoxLibrary->currentIndex() == Library::STILL_PAGE_INDEX)
    {
        if (this->treeWidgetImage->selectedItems().count() == 0)
            return;

        this->contextMenu->exec(this->treeWidgetImage->mapToGlobal(point));
    }
    else if (this->toolBoxLibrary->currentIndex() == Library::TEMPLATE_PAGE_INDEX)
    {
        if (this->treeWidgetTemplate->selectedItems().count() == 0)
            return;

        this->contextMenu->exec(this->treeWidgetTemplate->mapToGlobal(point));
    }
    else if (this->toolBoxLibrary->currentIndex() == Library::MOVIE_PAGE_INDEX)
    {
        if (this->treeWidgetVideo->selectedItems().count() == 0)
            return;

        this->contextMenu->exec(this->treeWidgetVideo->mapToGlobal(point));
    }
}

void LibraryWidget::customContextMenuImageRequested(const QPoint& point)
{
    if (this->treeWidgetImage->selectedItems().count() == 0)
        return;

    this->contextMenuImage->exec(this->treeWidgetImage->mapToGlobal(point));
}

void LibraryWidget::contextMenuTriggered(QAction* action)
{
    if (this->toolBoxLibrary->currentIndex() == Library::AUDIO_PAGE_INDEX)
    {
        foreach (QTreeWidgetItem* item, this->treeWidgetAudio->selectedItems())
            EventManager::getInstance().fireAddRudnownItemEvent(LibraryModel(item->text(1).toInt(), item->text(2), item->text(0),
                                                                             item->text(3), item->text(4), item->text(5).toInt()));
    }
    else if (this->toolBoxLibrary->currentIndex() == Library::TEMPLATE_PAGE_INDEX)
    {
        foreach (QTreeWidgetItem* item, this->treeWidgetTemplate->selectedItems())
            EventManager::getInstance().fireAddRudnownItemEvent(LibraryModel(item->text(1).toInt(), item->text(2), item->text(0),
                                                                             item->text(3), item->text(4), item->text(5).toInt()));
    }
    else if (this->toolBoxLibrary->currentIndex() == Library::MOVIE_PAGE_INDEX)
    {
        foreach (QTreeWidgetItem* item, this->treeWidgetVideo->selectedItems())
            EventManager::getInstance().fireAddRudnownItemEvent(LibraryModel(item->text(1).toInt(), item->text(2), item->text(0),
                                                                             item->text(3), item->text(4), item->text(5).toInt()));
    }
}

void LibraryWidget::contextMenuImageTriggered(QAction* action)
{
    if (action->text() == "Add as image")
    {
        foreach (QTreeWidgetItem* item, this->treeWidgetImage->selectedItems())
            EventManager::getInstance().fireAddRudnownItemEvent(LibraryModel(item->text(1).toInt(), item->text(2), item->text(0),
                                                                             item->text(3), item->text(4), item->text(5).toInt()));
    }
    else if (action->text() == "Add as image scroller")
    {
        foreach (QTreeWidgetItem* item, this->treeWidgetImage->selectedItems())
            EventManager::getInstance().fireAddRudnownItemEvent(LibraryModel(item->text(1).toInt(), item->text(2), item->text(0),
                                                                             item->text(3), "IMAGESCROLLER", item->text(5).toInt()));
    }
}

void LibraryWidget::filterLibrary()
{
    EventManager::getInstance().fireMediaChangedEvent();
    EventManager::getInstance().fireTemplateChangedEvent();
    EventManager::getInstance().fireDataChangedEvent();
}

void LibraryWidget::itemClicked(QTreeWidgetItem* current, int index)
{
    if (current == NULL)
        return;

    this->model = QSharedPointer<LibraryModel>(new LibraryModel(current->text(1).toInt(), current->text(2), current->text(0),
                                                                current->text(3), current->text(4), current->text(5).toInt()));

    EventManager::getInstance().fireLibraryItemSelectedEvent(NULL, this->model.data());
}

void LibraryWidget::itemDoubleClicked(QTreeWidgetItem* current, int index)
{
    if (current == NULL)
        return;

    if (this->toolBoxLibrary->currentIndex() == Library::AUDIO_PAGE_INDEX)
    {
        EventManager::getInstance().fireAddRudnownItemEvent(LibraryModel(current->text(1).toInt(), current->text(2), current->text(0),
                                                                         current->text(3), current->text(4), current->text(5).toInt()));
    }
    else if (this->toolBoxLibrary->currentIndex() == Library::STILL_PAGE_INDEX)
    {
        EventManager::getInstance().fireAddRudnownItemEvent(LibraryModel(current->text(1).toInt(), current->text(2), current->text(0),
                                                                         current->text(3), current->text(4), current->text(5).toInt()));
    }
    else if (this->toolBoxLibrary->currentIndex() == Library::TEMPLATE_PAGE_INDEX)
    {
        EventManager::getInstance().fireAddRudnownItemEvent(LibraryModel(current->text(1).toInt(), current->text(2), current->text(0),
                                                                         current->text(3), current->text(4), current->text(5).toInt()));
    }
    else if (this->toolBoxLibrary->currentIndex() == Library::MOVIE_PAGE_INDEX)
    {
        EventManager::getInstance().fireAddRudnownItemEvent(LibraryModel(current->text(1).toInt(), current->text(2), current->text(0),
                                                                         current->text(3), current->text(4), current->text(5).toInt()));
    }
}

void LibraryWidget::currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{ 
    if (current == NULL)
        return;

    this->model = QSharedPointer<LibraryModel>(new LibraryModel(current->text(1).toInt(), current->text(2), current->text(0),
                                                                current->text(3), current->text(4), current->text(5).toInt()));

    EventManager::getInstance().fireLibraryItemSelectedEvent(NULL, this->model.data());
}
