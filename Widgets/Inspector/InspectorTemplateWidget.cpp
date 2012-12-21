#include "InspectorTemplateWidget.h"

#include "Global.h"

#include "Events/RundownItemSelectedEvent.h"

InspectorTemplateWidget::InspectorTemplateWidget(QWidget* parent)
    : QWidget(parent), preview(false), model(NULL), command(NULL)
{
    setupUi(this);

    this->rowIndex = this->tableWidgetTemplateData->rowCount();

    qApp->installEventFilter(this);
}

bool InspectorTemplateWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        if (dynamic_cast<TemplateCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->preview = false;

            this->model = rundownItemSelectedEvent->getLibraryModel();
            this->command = dynamic_cast<TemplateCommand*>(rundownItemSelectedEvent->getCommand());

            this->spinBoxFlashlayer->setValue(this->command->getFlashlayer());
            this->lineEditInvoke->setText(this->command->getInvoke());
            this->checkBoxUseStoredData->setChecked(this->command->getUseStoredData());

            for (int i = this->tableWidgetTemplateData->rowCount() - 1; i >= 0; i--)
                this->tableWidgetTemplateData->removeRow(i);

            this->rowIndex = 0;
            foreach (TemplateDataModel model, this->command->getTemplateDataModels())
            {
                this->tableWidgetTemplateData->insertRow(this->tableWidgetTemplateData->rowCount());

                QTableWidgetItem* nameItem = new QTableWidgetItem(model.getKey());
                QTableWidgetItem* valueItem = new QTableWidgetItem(model.getValue());
                this->tableWidgetTemplateData->setItem(this->tableWidgetTemplateData->rowCount() - 1, 0, nameItem);
                this->tableWidgetTemplateData->setItem(this->tableWidgetTemplateData->rowCount() - 1, 1, valueItem);

                this->rowIndex++;
            }

            this->preview = true;
        }
    }

    return QObject::eventFilter(target, event);
}

void InspectorTemplateWidget::updateDataTemplateModels()
{
    QList<TemplateDataModel> models;
    for (int i = 0; i < this->tableWidgetTemplateData->rowCount(); i++)
    {
        if (this->tableWidgetTemplateData->item(i, 0) != NULL && this->tableWidgetTemplateData->item(i, 1) != NULL)
            models.push_back(TemplateDataModel(this->tableWidgetTemplateData->item(i, 0)->text(), this->tableWidgetTemplateData->item(i, 1)->text()));
    }

    this->command->setTemplateDataModels(models);
}

void InspectorTemplateWidget::addRow()
{
    this->tableWidgetTemplateData->insertRow(this->tableWidgetTemplateData->rowCount());

    QTableWidgetItem* nameItem = new QTableWidgetItem(QString("f%1").arg(this->rowIndex));
    QTableWidgetItem* valueItem = new QTableWidgetItem("");
    this->tableWidgetTemplateData->setItem(this->tableWidgetTemplateData->rowCount() - 1, 0, nameItem);
    this->tableWidgetTemplateData->setItem(this->tableWidgetTemplateData->rowCount() - 1, 1, valueItem);

    this->rowIndex++;
}

void InspectorTemplateWidget::removeRow()
{
    this->tableWidgetTemplateData->removeRow(this->tableWidgetTemplateData->currentIndex().row());
    updateDataTemplateModels();
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

void InspectorTemplateWidget::itemChanged(QTableWidgetItem* item)
{
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
