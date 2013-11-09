#include "InspectorGroupWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/PreviewEvent.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/TweenModel.h"

#include <QtCore/QDebug>

#include <QtGui/QApplication>

InspectorGroupWidget::InspectorGroupWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL), disableVideoProgress(false)
{
    setupUi(this);

    this->disableVideoProgress = (DatabaseManager::getInstance().getConfigurationByName("DisableVideoProgress").getValue() == "true") ? true : false;

    qApp->installEventFilter(this);
}

bool InspectorGroupWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Event::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        this->model = rundownItemSelectedEvent->getLibraryModel();

        blockAllSignals(true);

        if (dynamic_cast<GroupCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->command = dynamic_cast<GroupCommand*>(rundownItemSelectedEvent->getCommand());

            this->plainTextEditNotes->setPlainText(this->command->getNotes());
            this->checkBoxAutoStep->setChecked(this->command->getAutoStep());

            if (!this->disableVideoProgress)
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

    return QObject::eventFilter(target, event);
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
    this->plainTextEditNotes->setPlainText(Group::DEFAULT_NOTE);
    this->command->setNotes(this->plainTextEditNotes->toPlainText());
}

void InspectorGroupWidget::autoStepChanged(int state)
{
    this->command->setAutoStep((state == Qt::Checked) ? true : false);
}

void InspectorGroupWidget::resetAutoStep(QString note)
{
    this->checkBoxAutoStep->setChecked(Group::DEFAULT_AUTO_STEP);
    this->command->setAutoStep(this->checkBoxAutoStep->isChecked());
}

void InspectorGroupWidget::autoPlayChanged(int state)
{
    this->command->setAutoPlay((state == Qt::Checked) ? true : false);

    EventManager::getInstance().fireAutoPlayChangedEvent(this->checkBoxAutoPlay->isChecked());
}
