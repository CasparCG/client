#include "InspectorClearOutputWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "Events/RundownItemPreviewEvent.h"
#include "Events/RundownItemSelectedEvent.h"
#include "Models/TweenModel.h"

#include <QtCore/QDebug>

#include <QtGui/QApplication>

InspectorClearOutputWidget::InspectorClearOutputWidget(QWidget* parent)
    : QWidget(parent), preview(false), model(NULL), command(NULL)
{
    setupUi(this);

    qApp->installEventFilter(this);
}

bool InspectorClearOutputWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        if (dynamic_cast<ClearOutputCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->preview = false;

            this->model = rundownItemSelectedEvent->getLibraryModel();
            this->command = dynamic_cast<ClearOutputCommand*>(rundownItemSelectedEvent->getCommand());

            this->checkBoxClearChannel->setChecked(this->command->getClearChannel());

            this->preview = true;
        }
    }

    return QObject::eventFilter(target, event);
}

void InspectorClearOutputWidget::clearChannelChanged(int state)
{
    this->command->setClearChannel((state == Qt::Checked) ? true : false);
}

void InspectorClearOutputWidget::resetClearChannel(QString clearChannel)
{
    this->checkBoxClearChannel->setChecked(ClearOutput::DEFAULT_CLEAR_CHANNEL);
    this->command->setClearChannel(this->checkBoxClearChannel->isChecked());
}
