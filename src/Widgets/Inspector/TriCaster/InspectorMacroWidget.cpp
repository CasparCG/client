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

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
}

void InspectorMacroWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<MacroCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<MacroCommand*>(event.getCommand());

        this->lineEditMacro->setText(this->command->getMacro());
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
    }

    blockAllSignals(false);
}

void InspectorMacroWidget::blockAllSignals(bool block)
{
    this->lineEditMacro->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
}

void InspectorMacroWidget::macroChanged(QString macro)
{
    this->command->setMacro(macro);
}

void InspectorMacroWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}
