#include "LibraryWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "Events/AddRudnownItemEvent.h"
#include "Events/DataChangedEvent.h"
#include "Events/LibraryItemSelectedEvent.h"
#include "Events/MediaChangedEvent.h"
#include "Events/StatusbarEvent.h"
#include "Events/SynchronizeEvent.h"
#include "Events/TemplateChangedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QDebug>
#include <QtCore/QPoint>
#include <QtCore/QSharedDataPointer>

#include <QtGui/QApplication>
#include <QtGui/QClipboard>
#include <QtGui/QTreeWidgetItem>

LibraryWidget::LibraryWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUi(this);
    setupUiMenu();

    this->treeWidgetAudio->setColumnHidden(1, true);
    this->treeWidgetAudio->setColumnHidden(2, true);
    this->treeWidgetAudio->setColumnHidden(3, true);
    this->treeWidgetImage->setColumnHidden(1, true);
    this->treeWidgetImage->setColumnHidden(2, true);
    this->treeWidgetImage->setColumnHidden(3, true);
    this->treeWidgetTemplate->setColumnHidden(1, true);
    this->treeWidgetTemplate->setColumnHidden(2, true);
    this->treeWidgetTemplate->setColumnHidden(3, true);
    this->treeWidgetVideo->setColumnHidden(1, true);
    this->treeWidgetVideo->setColumnHidden(2, true);
    this->treeWidgetVideo->setColumnHidden(3, true);

    QObject::connect(this->treeWidgetAudio, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(customContextMenuRequested(const QPoint &)));
    QObject::connect(this->treeWidgetImage, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(customContextMenuRequested(const QPoint &)));
    QObject::connect(this->treeWidgetTemplate, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(customContextMenuRequested(const QPoint &)));
    QObject::connect(this->treeWidgetVideo, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(customContextMenuRequested(const QPoint &)));

    qApp->installEventFilter(this);

    qApp->postEvent(qApp, new MediaChangedEvent());
    qApp->postEvent(qApp, new TemplateChangedEvent());
}

void LibraryWidget::setupUiMenu()
{
    this->contextMenu = new QMenu(this);
    this->contextMenu->addAction("Add to rundown");

    QObject::connect(this->contextMenu, SIGNAL(triggered(QAction*)), this, SLOT(contextMenuTriggered(QAction*)));
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
                    widget->setText(1, "-1");
                    widget->setText(2, model.getDeviceName());
                    widget->setText(3, model.getType());
                }    
                else if (model.getType() == "STILL")
                {
                    QTreeWidgetItem* widget = new QTreeWidgetItem(this->treeWidgetImage);
                    widget->setIcon(0, QIcon(":/Graphics/Images/Still.png"));
                    widget->setText(0, model.getName());
                    widget->setText(1, "-1");
                    widget->setText(2, model.getDeviceName());
                    widget->setText(3, model.getType());
                }
                else if (model.getType() == "MOVIE")
                {
                    QTreeWidgetItem* widget = new QTreeWidgetItem(this->treeWidgetVideo);
                    widget->setIcon(0, QIcon(":/Graphics/Images/Movie.png"));
                    widget->setText(0, model.getName());
                    widget->setText(1, "-1");
                    widget->setText(2, model.getDeviceName());
                    widget->setText(3, model.getType());
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
                widget->setText(1, "-1");
                widget->setText(2, model.getDeviceName());
                widget->setText(3, model.getType());
            }
        }

        this->toolBoxLibrary->setItemText(2, QString("Templates (%1)").arg(this->treeWidgetTemplate->topLevelItemCount()));
    }

    return QObject::eventFilter(target, event);
}

void LibraryWidget::customContextMenuRequested(const QPoint& point)
{
    if (this->toolBoxLibrary->currentIndex() == 0)
    {
        if (this->treeWidgetAudio->selectedItems().count() == 0)
            return;

        this->contextMenu->exec(this->treeWidgetAudio->mapToGlobal(point));
    }
    else if (this->toolBoxLibrary->currentIndex() == 1)
    {
        if (this->treeWidgetImage->selectedItems().count() == 0)
            return;

        this->contextMenu->exec(this->treeWidgetImage->mapToGlobal(point));
    }
    else if (this->toolBoxLibrary->currentIndex() == 2)
    {
        if (this->treeWidgetTemplate->selectedItems().count() == 0)
            return;

        this->contextMenu->exec(this->treeWidgetTemplate->mapToGlobal(point));
    }
    else if (this->toolBoxLibrary->currentIndex() == 3)
    {
        if (this->treeWidgetVideo->selectedItems().count() == 0)
            return;

        this->contextMenu->exec(this->treeWidgetVideo->mapToGlobal(point));
    }
}

void LibraryWidget::contextMenuTriggered(QAction* action)
{
    if (this->toolBoxLibrary->currentIndex() == 0)
    {
        foreach (QTreeWidgetItem* item, this->treeWidgetAudio->selectedItems())
            qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(item->text(1).toInt(), item->text(0), item->text(2), item->text(3))));
    }
    else if (this->toolBoxLibrary->currentIndex() == 1)
    {
        foreach (QTreeWidgetItem* item, this->treeWidgetImage->selectedItems())
            qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(item->text(1).toInt(), item->text(0), item->text(2), item->text(3))));
    }
    else if (this->toolBoxLibrary->currentIndex() == 2)
    {
        foreach (QTreeWidgetItem* item, this->treeWidgetTemplate->selectedItems())
            qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(item->text(1).toInt(), item->text(0), item->text(2), item->text(3))));
    }
    else if (this->toolBoxLibrary->currentIndex() == 3)
    {
        foreach (QTreeWidgetItem* item, this->treeWidgetVideo->selectedItems())
            qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(item->text(1).toInt(), item->text(0), item->text(2), item->text(3))));
    }
}

void LibraryWidget::filterLibrary()
{
    qApp->postEvent(qApp, new MediaChangedEvent());
    qApp->postEvent(qApp, new TemplateChangedEvent());
}

void LibraryWidget::synchronizeLibrary()
{
    qApp->postEvent(qApp, new SynchronizeEvent(0));
}

void LibraryWidget::currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
    if (current == NULL)
        return;

    this->model = QSharedPointer<LibraryModel>(new LibraryModel(current->text(1).toInt(), current->text(0), current->text(2), current->text(3)));
    qApp->postEvent(qApp, new LibraryItemSelectedEvent(NULL, this->model.data()));
}
