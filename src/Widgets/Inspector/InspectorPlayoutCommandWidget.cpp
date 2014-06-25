#include "InspectorPlayoutCommandWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"

#include <QtGui/QApplication>

InspectorPlayoutCommandWidget::InspectorPlayoutCommandWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
}

void InspectorPlayoutCommandWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<PlayoutCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<PlayoutCommand*>(event.getCommand());

        this->comboBoxPlayoutCommand->setCurrentIndex(this->comboBoxPlayoutCommand->findText(this->command->getPlayoutCommand()));
    }

    blockAllSignals(false);
}

void InspectorPlayoutCommandWidget::blockAllSignals(bool block)
{
    this->comboBoxPlayoutCommand->blockSignals(block);
}

void InspectorPlayoutCommandWidget::playoutCommandChanged(QString command)
{
    this->command->setPlayoutCommand(command);
}
