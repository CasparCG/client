#include "InspectorMacroWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"

#include <QtGui/QApplication>

InspectorMacroWidget::InspectorMacroWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    qApp->installEventFilter(this);
}

bool InspectorMacroWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Event::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        this->model = rundownItemSelectedEvent->getLibraryModel();

        blockAllSignals(true);

        if (dynamic_cast<MacroCommand*>(rundownItemSelectedEvent->getCommand()))
        {  
            this->command = dynamic_cast<MacroCommand*>(rundownItemSelectedEvent->getCommand());

            this->lineEditMacro->setText(this->command->getMacro());
        }

        blockAllSignals(false);
    }

    return QObject::eventFilter(target, event);
}

void InspectorMacroWidget::blockAllSignals(bool block)
{
    this->lineEditMacro->blockSignals(block);
}

void InspectorMacroWidget::macroChanged(QString macro)
{
    this->command->setMacro(macro);
}
