#include "InspectorBlendModeWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/PreviewEvent.h"
#include "Models/BlendModeModel.h"

#include <QtGui/QApplication>

InspectorBlendModeWidget::InspectorBlendModeWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));

    loadBlendMode();
}

void InspectorBlendModeWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<BlendModeCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<BlendModeCommand*>(event.getCommand());

        this->comboBoxBlendMode->setCurrentIndex(this->comboBoxBlendMode->findText(this->command->getBlendMode()));
    }

    blockAllSignals(false);
}

void InspectorBlendModeWidget::blockAllSignals(bool block)
{
    this->comboBoxBlendMode->blockSignals(block);
}

void InspectorBlendModeWidget::loadBlendMode()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxBlendMode->blockSignals(true);

    QList<BlendModeModel> models = DatabaseManager::getInstance().getBlendMode();
    foreach (BlendModeModel model, models)
        this->comboBoxBlendMode->addItem(model.getValue());

    this->comboBoxBlendMode->blockSignals(false);
}

void InspectorBlendModeWidget::blendModeChanged(QString blendMode)
{
    this->command->setBlendMode(blendMode);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorBlendModeWidget::resetBlendMode(QString blendMode)
{
    this->comboBoxBlendMode->setCurrentIndex(this->comboBoxBlendMode->findText("Normal"));
    this->command->setBlendMode(this->comboBoxBlendMode->currentText());

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}
