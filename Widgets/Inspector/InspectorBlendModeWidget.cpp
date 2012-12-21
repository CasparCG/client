#include "InspectorBlendModeWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "Events/RundownItemPreviewEvent.h"
#include "Events/RundownItemSelectedEvent.h"
#include "Models/BlendModeModel.h"

#include <QtGui/QApplication>

InspectorBlendModeWidget::InspectorBlendModeWidget(QWidget* parent)
    : QWidget(parent), preview(false), model(NULL), command(NULL)
{
    setupUi(this);

    loadBlendMode();

    qApp->installEventFilter(this);
}

bool InspectorBlendModeWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        if (dynamic_cast<BlendModeCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->preview = false;

            this->model = rundownItemSelectedEvent->getLibraryModel();
            this->command = dynamic_cast<BlendModeCommand*>(rundownItemSelectedEvent->getCommand());

            this->comboBoxBlendMode->setCurrentIndex(this->comboBoxBlendMode->findText(this->command->getBlendMode()));

            this->preview = true;
        }
    }

    return QObject::eventFilter(target, event);
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

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
}

void InspectorBlendModeWidget::resetBlendMode(QString blendMode)
{
    this->comboBoxBlendMode->setCurrentIndex(this->comboBoxBlendMode->findText("Normal"));
    this->command->setBlendMode(this->comboBoxBlendMode->currentText());

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
}
