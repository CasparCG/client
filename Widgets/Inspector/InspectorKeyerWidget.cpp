#include "InspectorKeyerWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "Events/RundownItemPreviewEvent.h"
#include "Events/RundownItemSelectedEvent.h"
#include "Models/TweenModel.h"

#include <QtCore/QDebug>

#include <QtGui/QApplication>

InspectorKeyerWidget::InspectorKeyerWidget(QWidget* parent)
    : QWidget(parent), preview(false), model(NULL), command(NULL)
{
    setupUi(this);

    qApp->installEventFilter(this);
}

bool InspectorKeyerWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        if (dynamic_cast<KeyerCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->preview = false;

            this->model = rundownItemSelectedEvent->getLibraryModel();
            this->command = dynamic_cast<KeyerCommand*>(rundownItemSelectedEvent->getCommand());

            this->checkBoxDefer->setChecked(this->command->getDefer());

            this->preview = true;
        }
    }

    return QObject::eventFilter(target, event);
}

void InspectorKeyerWidget::deferChanged(int state)
{
    this->command->setDefer((state == Qt::Checked) ? true : false);
}

void InspectorKeyerWidget::resetDefer(QString defer)
{
    this->checkBoxDefer->setChecked(false);
    this->command->setDefer(this->checkBoxDefer->isChecked());
}
