#include "InspectorTemplateWidget.h"
#include "TemplateDataDialog.h"

#include "Global.h"

#include "Events/AddTemplateDataEvent.h"
#include "Events/RundownItemSelectedEvent.h"

#include <QtCore/QDebug>

#include <QtGui/QResizeEvent>

InspectorTemplateWidget::InspectorTemplateWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    this->treeWidgetTemplateData->setColumnWidth(1, 87);
    this->rowIndex = this->treeWidgetTemplateData->invisibleRootItem()->childCount();

    qApp->installEventFilter(this);
}

bool InspectorTemplateWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Event::EventType::RundownItemSelected))
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

            for (int i = this->treeWidgetTemplateData->invisibleRootItem()->childCount() - 1; i >= 0; i--)
                delete this->treeWidgetTemplateData->invisibleRootItem()->child(i);

            this->rowIndex = 0;
            foreach (TemplateDataModel model, this->command->getTemplateDataModels())
            {
                QTreeWidgetItem* treeItem = new QTreeWidgetItem();
                treeItem->setText(0, model.getKey());
                treeItem->setText(1, model.getValue());

                this->treeWidgetTemplateData->invisibleRootItem()->addChild(treeItem);

                this->rowIndex++;
            }
        }

        blockAllSignals(false);
    }
    else if (event->type() == static_cast<QEvent::Type>(Event::EventType::AddTemplateData))
    {
        AddTemplateDataEvent* addTemplateDataEvent = dynamic_cast<AddTemplateDataEvent*>(event);

        QTreeWidgetItem* treeItem = new QTreeWidgetItem();
        treeItem->setText(0, QString("f%1").arg(this->rowIndex));
        treeItem->setText(1, addTemplateDataEvent->getValue());

        this->treeWidgetTemplateData->invisibleRootItem()->addChild(treeItem);
        this->treeWidgetTemplateData->setCurrentItem(treeItem);

        this->checkBoxUseStoredData->setChecked(addTemplateDataEvent->getStoredData());

        this->rowIndex++;
    }

    return QObject::eventFilter(target, event);
}

void InspectorTemplateWidget::blockAllSignals(bool block)
{
    this->spinBoxFlashlayer->blockSignals(block);
    this->lineEditInvoke->blockSignals(block);
    this->checkBoxUseStoredData->blockSignals(block);
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

void InspectorTemplateWidget::addRow()
{
    TemplateDataDialog* dialog = new TemplateDataDialog(this);
    dialog->setName(QString("f%1").arg(this->rowIndex));
    if (dialog->exec() == QDialog::Accepted)
    {
        QTreeWidgetItem* treeItem = new QTreeWidgetItem();
        treeItem->setText(0, dialog->getName());
        treeItem->setText(1, dialog->getValue());

        this->treeWidgetTemplateData->invisibleRootItem()->addChild(treeItem);
        this->treeWidgetTemplateData->setCurrentItem(treeItem);

        this->rowIndex++;
    }
}

void InspectorTemplateWidget::removeRow()
{
    delete this->treeWidgetTemplateData->currentItem();
    updateDataTemplateModels();
}

void InspectorTemplateWidget::itemDoubleClicked(QTreeWidgetItem* current, int index)
{
    if (current == NULL)
        return;

    TemplateDataDialog* dialog = new TemplateDataDialog(this);
    dialog->setName(current->text(0));
    dialog->setValue(current->text(1));
    if (dialog->exec() == QDialog::Accepted)
    {
        current->setText(0, dialog->getName());
        current->setText(1, dialog->getValue());

        updateDataTemplateModels();
    }
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

void InspectorTemplateWidget::currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
    if (current == NULL)
        return;

    updateDataTemplateModels();
}

void InspectorTemplateWidget::resetFlashlayer(QString flashlayer)
{
    this->spinBoxFlashlayer->setValue(Template::DEFAULT_FLASHLAYER);
    this->command->setFlashlayer(this->spinBoxFlashlayer->value());
}

void InspectorTemplateWidget::resetUseStoredData(QString)
{
    this->checkBoxUseStoredData->setChecked(false);
    this->command->setUseStoredData(this->checkBoxUseStoredData->isChecked());
}

void InspectorTemplateWidget::resetInvoke(QString invoke)
{
    this->lineEditInvoke->clear();
    this->command->setInvoke(this->lineEditInvoke->text());
}
