#include "InspectorGroupWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/PreviewEvent.h"
#include "Events/Inspector/AutoPlayChangedEvent.h"
#include "Models/TweenModel.h"

#include <QtCore/QDebug>

#include <QtWidgets/QApplication>

InspectorGroupWidget::InspectorGroupWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL), enableOscInputControl(true)
{
    setupUi(this);

    this->enableOscInputControl = (DatabaseManager::getInstance().getConfigurationByName("EnableOscInputControl").getValue() == "true") ? true : false;

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
}

void InspectorGroupWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->command = nullptr;
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<GroupCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<GroupCommand*>(event.getCommand());

        this->plainTextEditNotes->setPlainText(this->command->getNotes());
        this->checkBoxAutoStep->setChecked(this->command->getAutoStep());

        if (this->enableOscInputControl)
        {
            this->labelAutoPlay->setVisible(true);
            this->checkBoxAutoPlay->setVisible(true);
            this->checkBoxAutoPlay->setChecked(this->command->getAutoPlay());
        }
        else
        {
            this->labelAutoPlay->setVisible(false);
            this->checkBoxAutoPlay->setVisible(false);
            this->checkBoxAutoPlay->setChecked(false);
        }
    }

    blockAllSignals(false);
}

void InspectorGroupWidget::blockAllSignals(bool block)
{
    this->plainTextEditNotes->blockSignals(block);
    this->checkBoxAutoStep->blockSignals(block);
    this->checkBoxAutoPlay->blockSignals(block);
}

void InspectorGroupWidget::notesChanged()
{
    this->command->setNotes(this->plainTextEditNotes->toPlainText());
}

void InspectorGroupWidget::resetNotes(QString note)
{
    Q_UNUSED(note);

    this->plainTextEditNotes->setPlainText(Group::DEFAULT_NOTE);
    this->command->setNotes(this->plainTextEditNotes->toPlainText());
}

void InspectorGroupWidget::autoStepChanged(int state)
{
    this->command->setAutoStep((state == Qt::Checked) ? true : false);
}

void InspectorGroupWidget::resetAutoStep(QString note)
{
    Q_UNUSED(note);

    this->checkBoxAutoStep->setChecked(Group::DEFAULT_AUTO_STEP);
    this->command->setAutoStep(this->checkBoxAutoStep->isChecked());
}

void InspectorGroupWidget::autoPlayChanged(int state)
{
    this->command->setAutoPlay((state == Qt::Checked) ? true : false);

    EventManager::getInstance().fireAutoPlayChangedEvent(AutoPlayChangedEvent(this->checkBoxAutoPlay->isChecked()));
}
