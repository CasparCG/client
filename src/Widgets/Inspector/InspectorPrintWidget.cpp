#include "InspectorPrintWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Models/FormatModel.h"

InspectorPrintWidget::InspectorPrintWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
}

void InspectorPrintWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<PrintCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<PrintCommand*>(event.getCommand());

        this->lineEditOutput->setText(this->command->getOutput());
    }

    blockAllSignals(false);
}

void InspectorPrintWidget::blockAllSignals(bool block)
{
    this->lineEditOutput->blockSignals(block);
}

void InspectorPrintWidget::outputChanged(QString output)
{
    this->command->setOutput(output);
}

void InspectorPrintWidget::resetOutput(QString output)
{
    this->lineEditOutput->setText(Print::DEFAULT_OUTPUT);
    this->command->setOutput(this->lineEditOutput->text());
}
