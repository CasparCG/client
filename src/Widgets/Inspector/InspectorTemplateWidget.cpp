#include "InspectorTemplateWidget.h"
#include "TemplateDataDialog.h"

#include "Global.h"

#include "Events/Inspector/AddTemplateDataEvent.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"

#include <QtCore/QDebug>

#include <QtGui/QApplication>
#include <QtGui/QClipboard>
#include <QtGui/QCursor>
#include <QtGui/QResizeEvent>

InspectorTemplateWidget::InspectorTemplateWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    this->treeWidgetTemplateData->setColumnWidth(1, 87);
    this->fieldCounter = this->treeWidgetTemplateData->invisibleRootItem()->childCount();

    qApp->installEventFilter(this);
}

bool InspectorTemplateWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(event);
        if (target == this->treeWidgetTemplateData)
        {
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
    }
    else if (event->type() == static_cast<QEvent::Type>(Event::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        this->model = rundownItemSelectedEvent->getLibraryModel();

        blockAllSignals(true);

        if (dynamic_cast<TemplateCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->command = dynamic_cast<TemplateCommand*>(rundownItemSelectedEvent->getCommand());

            this->spinBoxFlashlayer->setValue(this->command->getFlashlayer());
            this->lineEditInvoke->setText(this->command->getInvoke());
            this->checkBoxUseStoredData->setChecked(this->command->getUseStoredData());
            this->checkBoxUseUppercaseData->setChecked(this->command->getUseUppercaseData());

            for (int i = this->treeWidgetTemplateData->invisibleRootItem()->childCount() - 1; i >= 0; i--)
                delete this->treeWidgetTemplateData->invisibleRootItem()->child(i);

            this->fieldCounter = 0;
            foreach (TemplateDataModel model, this->command->getTemplateDataModels())
            {
                QTreeWidgetItem* treeItem = new QTreeWidgetItem();
                treeItem->setText(0, model.getKey());
                treeItem->setText(1, model.getValue());

                this->treeWidgetTemplateData->invisibleRootItem()->addChild(treeItem);

                this->fieldCounter++;
            }
        }

        blockAllSignals(false);
    }
    else if (event->type() == static_cast<QEvent::Type>(Event::EventType::AddTemplateData))
    {
        AddTemplateDataEvent* addTemplateDataEvent = dynamic_cast<AddTemplateDataEvent*>(event);

        QTreeWidgetItem* treeItem = new QTreeWidgetItem();
        treeItem->setText(0, QString("f%1").arg(this->fieldCounter));
        treeItem->setText(1, addTemplateDataEvent->getValue());

        this->treeWidgetTemplateData->invisibleRootItem()->addChild(treeItem);
        this->treeWidgetTemplateData->setCurrentItem(treeItem);

        this->checkBoxUseStoredData->setChecked(addTemplateDataEvent->getStoredData());

        this->fieldCounter++;
    }

    return QObject::eventFilter(target, event);
}

void InspectorTemplateWidget::blockAllSignals(bool block)
{
    this->spinBoxFlashlayer->blockSignals(block);
    this->lineEditInvoke->blockSignals(block);
    this->checkBoxUseStoredData->blockSignals(block);
    this->checkBoxUseUppercaseData->blockSignals(block);
    this->treeWidgetTemplateData->blockSignals(block);
}

void InspectorTemplateWidget::updateDataTemplateModels()
{
    QList<TemplateDataModel> models;
    for (int i = 0; i < this->treeWidgetTemplateData->invisibleRootItem()->childCount(); i++)
        models.push_back(TemplateDataModel(this->treeWidgetTemplateData->invisibleRootItem()->child(i)->text(0),
                                           this->treeWidgetTemplateData->invisibleRootItem()->child(i)->text(1)));

    this->command->setTemplateDataModels(models);
}

bool InspectorTemplateWidget::addRow()
{
    TemplateDataDialog* dialog = new TemplateDataDialog(this);
    dialog->move(QPoint(QCursor::pos().x() - dialog->width() + 40, QCursor::pos().y() - dialog->height() - 10));
    dialog->setName(QString("f%1").arg(this->fieldCounter));
    if (dialog->exec() == QDialog::Accepted)
    {
        QTreeWidgetItem* treeItem = new QTreeWidgetItem();
        treeItem->setText(0, dialog->getName());
        treeItem->setText(1, dialog->getValue());

        this->treeWidgetTemplateData->invisibleRootItem()->insertChild(this->treeWidgetTemplateData->currentIndex().row() + 1, treeItem);
        this->treeWidgetTemplateData->setCurrentItem(treeItem);

        this->fieldCounter++;
    }

    return true;
}

bool InspectorTemplateWidget::editRow()
{
    if (this->treeWidgetTemplateData->currentItem() == NULL)
        return true;

    TemplateDataDialog* dialog = new TemplateDataDialog(this);
    dialog->move(QPoint(QCursor::pos().x() - dialog->width() + 40, QCursor::pos().y() - dialog->height() - 10));
    dialog->setName(this->treeWidgetTemplateData->currentItem()->text(0));
    dialog->setValue(this->treeWidgetTemplateData->currentItem()->text(1));
    if (dialog->exec() == QDialog::Accepted)
    {
        this->treeWidgetTemplateData->currentItem()->setText(0, dialog->getName());
        this->treeWidgetTemplateData->currentItem()->setText(1, dialog->getValue());

        updateDataTemplateModels();
    }
}

bool InspectorTemplateWidget::removeRow()
{
    if (this->treeWidgetTemplateData->currentItem() == NULL)
        return true;

    delete this->treeWidgetTemplateData->currentItem();
    updateDataTemplateModels();

    if (this->treeWidgetTemplateData->invisibleRootItem()->childCount() == 0)
        this->fieldCounter = 0;

    return true;
}

bool InspectorTemplateWidget::duplicateSelectedItem()
{
    if (!copySelectedItem())
        return true;

    if (!pasteSelectedItem())
        return true;

    return true;
}

bool InspectorTemplateWidget::copySelectedItem()
{
    QString data;

    if (this->treeWidgetTemplateData->selectedItems().count() == 0)
        return true;

    data = this->treeWidgetTemplateData->selectedItems().at(0)->text(0);
    data += "#" + this->treeWidgetTemplateData->selectedItems().at(0)->text(1);

    qApp->clipboard()->setText(data);

    return true;
}

bool InspectorTemplateWidget::pasteSelectedItem()
{
    if (qApp->clipboard()->text().isEmpty())
        return true;

    if (qApp->clipboard()->text().split("#").count() < 2)
        return true;

    QTreeWidgetItem* treeItem = new QTreeWidgetItem();
    treeItem->setText(0, qApp->clipboard()->text().split("#").at(0));
    treeItem->setText(1, qApp->clipboard()->text().split("#").at(1));

    this->treeWidgetTemplateData->invisibleRootItem()->insertChild(this->treeWidgetTemplateData->currentIndex().row() + 1, treeItem);

    return true;
}

void InspectorTemplateWidget::itemDoubleClicked(QTreeWidgetItem* current, int index)
{
    editRow();
}

void InspectorTemplateWidget::invokeChanged(QString invoke)
{
    this->command->setInvoke(invoke);
}

void InspectorTemplateWidget::flashlayerChanged(int flashlayer)
{
    this->command->setFlashlayer(flashlayer);
}

void InspectorTemplateWidget::useStoredDataChanged(int state)
{
    this->command->setUseStoredData((state == Qt::Checked) ? true : false);
}

void InspectorTemplateWidget::useUppercaseDataChanged(int state)
{
    this->command->setUseUppercaseData((state == Qt::Checked) ? true : false);
}

void InspectorTemplateWidget::currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
    if (current == NULL)
        return;

    updateDataTemplateModels();
}
