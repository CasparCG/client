#include "InspectorPrintWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "Events/RundownItemSelectedEvent.h"
#include "Models/FormatModel.h"

InspectorPrintWidget::InspectorPrintWidget(QWidget* parent)
    : QWidget(parent),
      preview(false), model(NULL), command(NULL)
{
    setupUi(this);

    qApp->installEventFilter(this);
}

bool InspectorPrintWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        if (dynamic_cast<PrintCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->preview = false;

            this->model = rundownItemSelectedEvent->getLibraryModel();
            this->command = dynamic_cast<PrintCommand*>(rundownItemSelectedEvent->getCommand());

            this->lineEditOutput->setText(this->command->getOutput());

            this->preview = true;
        }
    }

    return QObject::eventFilter(target, event);
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
