#include "InspectorAtemVideoFormatWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Models/Atem/AtemVideoFormatModel.h"

#include <QtGui/QApplication>

InspectorAtemVideoFormatWidget::InspectorAtemVideoFormatWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));

    loadAtemFormat();
}

void InspectorAtemVideoFormatWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<AtemVideoFormatCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<AtemVideoFormatCommand*>(event.getCommand());

        this->comboBoxFormat->setCurrentIndex(this->comboBoxFormat->findData(this->command->getFormat()));
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
    }

    blockAllSignals(false);
}

void InspectorAtemVideoFormatWidget::blockAllSignals(bool block)
{
    this->comboBoxFormat->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
}

void InspectorAtemVideoFormatWidget::loadAtemFormat()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxFormat->blockSignals(true);

    QList<AtemVideoFormatModel> models = DatabaseManager::getInstance().getAtemVideoFormat();
    foreach (AtemVideoFormatModel model, models)
        this->comboBoxFormat->addItem(model.getName(), model.getValue());

    this->comboBoxFormat->blockSignals(false);
}

void InspectorAtemVideoFormatWidget::formatChanged(int index)
{
    this->command->setFormat(this->comboBoxFormat->itemData(index).toString());
}

void InspectorAtemVideoFormatWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}
