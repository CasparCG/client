#include "InspectorGroupWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "Events/RundownItemPreviewEvent.h"
#include "Events/RundownItemSelectedEvent.h"
#include "Models/TweenModel.h"

#include <QtCore/QDebug>

#include <QtGui/QApplication>

InspectorGroupWidget::InspectorGroupWidget(QWidget* parent)
    : QWidget(parent),
      preview(false), model(NULL), command(NULL)
{
    setupUi(this);

    qApp->installEventFilter(this);
}

bool InspectorGroupWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        if (dynamic_cast<GroupCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->preview = false;

            this->model = rundownItemSelectedEvent->getLibraryModel();
            this->command = dynamic_cast<GroupCommand*>(rundownItemSelectedEvent->getCommand());

            this->plainTextEditNotes->setPlainText(this->command->getNotes());

            this->preview = true;
        }
    }

    return QObject::eventFilter(target, event);
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
