#include "InspectorOscOutputWidget.h"

#include "Global.h"

#include "EventManager.h"
#include "DatabaseManager.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"

InspectorOscOutputWidget::InspectorOscOutputWidget(QWidget *parent)
    : QWidget(parent),
      command(0)
{
    setupUi(this);

    loadOscOutput();

    qApp->installEventFilter(this);
}

bool InspectorOscOutputWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Event::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);

        blockAllSignals(true);

        if (dynamic_cast<OscOutputCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->command = dynamic_cast<OscOutputCommand*>(rundownItemSelectedEvent->getCommand());

            this->comboBoxOutput->setCurrentIndex(this->comboBoxOutput->findText(this->command->getOutput()));
            this->lineEditPath->setText(this->command->getPath());
            this->lineEditMessage->setText(this->command->getMessage());
            this->comboBoxType->setCurrentIndex(this->comboBoxType->findText(this->command->getType()));
        }

        checkEmptyOutput();
        checkEmptyPath();

        blockAllSignals(false);
    }
    else if (event->type() == static_cast<QEvent::Type>(Event::EventType::OscOutputChanged))
    {
        blockAllSignals(true);

        loadOscOutput();

        checkEmptyOutput();
        checkEmptyPath();

        blockAllSignals(false);
    }

    return QObject::eventFilter(target, event);
}

void InspectorOscOutputWidget::loadOscOutput()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxOutput->blockSignals(true);

    this->comboBoxOutput->clear();
    QList<OscOutputModel> models = DatabaseManager::getInstance().getOscOutput();
    foreach (OscOutputModel model, models)
        this->comboBoxOutput->addItem(model.getName());

    this->comboBoxOutput->blockSignals(false);
}

void InspectorOscOutputWidget::blockAllSignals(bool block)
{
    this->comboBoxOutput->blockSignals(block);
    this->lineEditPath->blockSignals(block);
    this->lineEditMessage->blockSignals(block);
    this->comboBoxType->blockSignals(block);
}

void InspectorOscOutputWidget::checkEmptyOutput()
{
    if (this->comboBoxOutput->isEnabled() && this->comboBoxOutput->currentText() == "")
        this->comboBoxOutput->setStyleSheet("border-color: red;");
    else
        this->comboBoxOutput->setStyleSheet("");
}

void InspectorOscOutputWidget::checkEmptyPath()
{
    if (this->lineEditPath->text().isEmpty())
        this->lineEditPath->setStyleSheet("border-color: red;");
    else
        this->lineEditPath->setStyleSheet("");
}

void InspectorOscOutputWidget::outputChanged(QString output)
{
    this->command->setOutput(output);

    checkEmptyOutput();
}

void InspectorOscOutputWidget::pathChanged(QString path)
{
    this->command->setPath(path);

    checkEmptyPath();
}

void InspectorOscOutputWidget::messageChanged(QString message)
{
    this->command->setMessage(message);
}

void InspectorOscOutputWidget::typeChanged(QString type)
{
    this->command->setType(type);
}
