#include "InspectorHttpGetWidget.h"
#include "KeyValueDialog.h"

#include "Global.h"

#include "EventManager.h"
#include "Models/KeyValueModel.h"

#include <QtCore/QDebug>

#include <QtGui/QApplication>
#include <QtGui/QClipboard>
#include <QtGui/QCursor>
#include <QtGui/QKeyEvent>
#include <QtGui/QResizeEvent>

InspectorHttpGetWidget::InspectorHttpGetWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    this->treeWidgetHttpData->setColumnWidth(1, 87);
    this->fieldCounter = this->treeWidgetHttpData->invisibleRootItem()->childCount();

    QObject::connect(&EventManager::getInstance(), SIGNAL(showAddHttpGetDataDialog(const ShowAddHttpGetDataDialogEvent&)), this, SLOT(showAddHttpGetDataDialog(const ShowAddHttpGetDataDialogEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));

    this->treeWidgetHttpData->installEventFilter(this);
}

bool InspectorHttpGetWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Delete)
            return removeRow();
        else if ((keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) && keyEvent->modifiers() == Qt::ShiftModifier)
            return editRow();
        else if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
            return addRow();
        else if (keyEvent->key() == Qt::Key_D && keyEvent->modifiers() == Qt::ControlModifier)
            return duplicateSelectedItem();
        else if (keyEvent->key() == Qt::Key_C && keyEvent->modifiers() == Qt::ControlModifier)
            return copySelectedItem();
        else if (keyEvent->key() == Qt::Key_V && keyEvent->modifiers() == Qt::ControlModifier)
            return pasteSelectedItem();
    }

    return QObject::eventFilter(target, event);
}

void InspectorHttpGetWidget::showAddHttpGetDataDialog(const ShowAddHttpGetDataDialogEvent& event)
{
    int index = this->treeWidgetHttpData->invisibleRootItem()->childCount() - 1;
    this->treeWidgetHttpData->setCurrentItem(this->treeWidgetHttpData->invisibleRootItem()->child(index));

    KeyValueDialog* dialog = new KeyValueDialog(this);
    dialog->setTitle("New HTTP GET Data");
    if (dialog->exec() == QDialog::Accepted)
    {
        QTreeWidgetItem* treeItem = new QTreeWidgetItem();
        treeItem->setText(0, dialog->getKey());
        treeItem->setText(1, dialog->getValue());

        this->treeWidgetHttpData->invisibleRootItem()->insertChild(this->treeWidgetHttpData->currentIndex().row() + 1, treeItem);
        this->treeWidgetHttpData->setCurrentItem(treeItem);
    }
}

void InspectorHttpGetWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<HttpGetCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<HttpGetCommand*>(event.getCommand());

        this->lineEditUrl->setText(this->command->getUrl());
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());

        for (int i = this->treeWidgetHttpData->invisibleRootItem()->childCount() - 1; i >= 0; i--)
            delete this->treeWidgetHttpData->invisibleRootItem()->child(i);

        foreach (KeyValueModel model, this->command->getHttpDataModels())
        {
            QTreeWidgetItem* treeItem = new QTreeWidgetItem();
            treeItem->setText(0, model.getKey());
            treeItem->setText(1, model.getValue());

            this->treeWidgetHttpData->invisibleRootItem()->addChild(treeItem);
        }
    }

    checkEmptyUrl();

    blockAllSignals(false);
}

void InspectorHttpGetWidget::checkEmptyUrl()
{
    if (this->lineEditUrl->text().isEmpty())
        this->lineEditUrl->setStyleSheet("border-color: firebrick;");
    else
        this->lineEditUrl->setStyleSheet("");
}

void InspectorHttpGetWidget::blockAllSignals(bool block)
{
    this->lineEditUrl->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
    this->treeWidgetHttpData->blockSignals(block);
}

void InspectorHttpGetWidget::updateHttpDataModels()
{
    QList<KeyValueModel> models;
    for (int i = 0; i < this->treeWidgetHttpData->invisibleRootItem()->childCount(); i++)
        models.push_back(KeyValueModel(this->treeWidgetHttpData->invisibleRootItem()->child(i)->text(0),
                                       this->treeWidgetHttpData->invisibleRootItem()->child(i)->text(1)));

    this->command->setHttpDataModels(models);
}

bool InspectorHttpGetWidget::addRow()
{
    KeyValueDialog* dialog = new KeyValueDialog(this);
    dialog->move(QPoint(QCursor::pos().x() - dialog->width() + 40, QCursor::pos().y() - dialog->height() - 10));
    dialog->setTitle("New HTTP GET Data");
    if (dialog->exec() == QDialog::Accepted)
    {
        QTreeWidgetItem* treeItem = new QTreeWidgetItem();
        treeItem->setText(0, dialog->getKey());
        treeItem->setText(1, dialog->getValue());

        this->treeWidgetHttpData->invisibleRootItem()->insertChild(this->treeWidgetHttpData->currentIndex().row() + 1, treeItem);
        this->treeWidgetHttpData->setCurrentItem(treeItem);
    }

    return true;
}

bool InspectorHttpGetWidget::editRow()
{
    if (this->treeWidgetHttpData->currentItem() == NULL)
        return true;

    KeyValueDialog* dialog = new KeyValueDialog(this);
    dialog->move(QPoint(QCursor::pos().x() - dialog->width() + 40, QCursor::pos().y() - dialog->height() - 10));
    dialog->setTitle("Edit HTTP GET Data");
    dialog->setKey(this->treeWidgetHttpData->currentItem()->text(0));
    dialog->setValue(this->treeWidgetHttpData->currentItem()->text(1));
    if (dialog->exec() == QDialog::Accepted)
    {
        this->treeWidgetHttpData->currentItem()->setText(0, dialog->getKey());
        this->treeWidgetHttpData->currentItem()->setText(1, dialog->getValue());

        updateHttpDataModels();
    }
}

bool InspectorHttpGetWidget::removeRow()
{
    if (this->treeWidgetHttpData->currentItem() == NULL)
        return true;

    delete this->treeWidgetHttpData->currentItem();
    updateHttpDataModels();

    return true;
}

bool InspectorHttpGetWidget::duplicateSelectedItem()
{
    if (!copySelectedItem())
        return true;

    if (!pasteSelectedItem())
        return true;

    return true;
}

bool InspectorHttpGetWidget::copySelectedItem()
{
    QString data;

    if (this->treeWidgetHttpData->selectedItems().count() == 0)
        return true;

    data = this->treeWidgetHttpData->selectedItems().at(0)->text(0);
    data += "#" + this->treeWidgetHttpData->selectedItems().at(0)->text(1);

    qApp->clipboard()->setText(data);

    return true;
}

bool InspectorHttpGetWidget::pasteSelectedItem()
{
    if (qApp->clipboard()->text().isEmpty())
        return true;

    if (qApp->clipboard()->text().split("#").count() < 2)
        return true;

    QTreeWidgetItem* treeItem = new QTreeWidgetItem();
    treeItem->setText(0, qApp->clipboard()->text().split("#").at(0));
    treeItem->setText(1, qApp->clipboard()->text().split("#").at(1));

    this->treeWidgetHttpData->invisibleRootItem()->insertChild(this->treeWidgetHttpData->currentIndex().row() + 1, treeItem);

    return true;
}

void InspectorHttpGetWidget::itemDoubleClicked(QTreeWidgetItem* current, int index)
{
    editRow();
}

void InspectorHttpGetWidget::urlChanged(QString url)
{
    this->command->setUrl(url);

    checkEmptyUrl();
}

void InspectorHttpGetWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}

void InspectorHttpGetWidget::currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
    if (current == NULL)
        return;

    updateHttpDataModels();
}
