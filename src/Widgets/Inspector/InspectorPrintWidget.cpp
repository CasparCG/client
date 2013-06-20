#include "InspectorPrintWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "Events/RundownItemSelectedEvent.h"
#include "Models/FormatModel.h"

InspectorPrintWidget::InspectorPrintWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    qApp->installEventFilter(this);
}

bool InspectorPrintWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Event::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        this->model = rundownItemSelectedEvent->getLibraryModel();

        blockAllSignals(true);

        if (dynamic_cast<PrintCommand*>(rundownItemSelectedEvent->getCommand()))
        {       
            this->command = dynamic_cast<PrintCommand*>(rundownItemSelectedEvent->getCommand());

            this->lineEditOutput->setText(this->command->getOutput());
        }

        blockAllSignals(false);
    }

    return QObject::eventFilter(target, event);
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
