#include "InspectorPreviewInputWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/TriCaster/TriCasterInputModel.h"

#include <QtGui/QApplication>

InspectorPreviewInputWidget::InspectorPreviewInputWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    loadTriCasterInput();

    qApp->installEventFilter(this);
}

bool InspectorPreviewInputWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Event::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        this->model = rundownItemSelectedEvent->getLibraryModel();

        blockAllSignals(true);

        if (dynamic_cast<PreviewInputCommand*>(rundownItemSelectedEvent->getCommand()))
        {  
            this->command = dynamic_cast<PreviewInputCommand*>(rundownItemSelectedEvent->getCommand());

            this->comboBoxInput->setCurrentIndex(this->comboBoxInput->findData(this->command->getInput()));
        }

        blockAllSignals(false);
    }

    return QObject::eventFilter(target, event);
}

void InspectorPreviewInputWidget::blockAllSignals(bool block)
{
    this->comboBoxInput->blockSignals(block);
}

void InspectorPreviewInputWidget::loadTriCasterInput()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxInput->blockSignals(true);

    QList<TriCasterInputModel> models = DatabaseManager::getInstance().getTriCasterInput();
    foreach (TriCasterInputModel model, models)
        this->comboBoxInput->addItem(model.getName(), model.getValue());

    this->comboBoxInput->blockSignals(false);
}

void InspectorPreviewInputWidget::inputChanged(int index)
{
    this->command->setInput(this->comboBoxInput->itemData(index).toString());
}
