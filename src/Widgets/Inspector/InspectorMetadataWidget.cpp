#include "InspectorMetadataWidget.h"

#include "Global.h"

#include "CasparDevice.h"

#include "EventManager.h"
#include "DeviceManager.h"
#include "DatabaseManager.h"
#include "Events/Inspector/DeviceChangedEvent.h"
#include "Events/Library/LibraryItemSelectedEvent.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"

#include <QtGui/QApplication>

InspectorMetadataWidget::InspectorMetadataWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL)
{
    setupUi(this);

    qApp->installEventFilter(this);
}

bool InspectorMetadataWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Event::EventType::LibraryItemSelected))
    {
        LibraryItemSelectedEvent* libraryItemSelectedEvent = dynamic_cast<LibraryItemSelectedEvent*>(event);
        this->model = libraryItemSelectedEvent->getLibraryModel();

        blockAllSignals(true);

        this->labelType->setEnabled(false);
        this->lineEditLabel->setEnabled(false);

        this->lineEditType->setText(this->model->getType());
        this->lineEditLabel->clear();

        blockAllSignals(false);
    }
    else if (event->type() == static_cast<QEvent::Type>(Event::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        this->model = rundownItemSelectedEvent->getLibraryModel();

        blockAllSignals(true);

        this->labelType->setEnabled(true);
        this->lineEditLabel->setEnabled(true);

        this->lineEditLabel->setReadOnly(false);

        this->lineEditType->setText(this->model->getType());
        this->lineEditLabel->setText(this->model->getLabel());

        blockAllSignals(false);
    }
    else if (event->type() == static_cast<QEvent::Type>(Event::EventType::EmptyRundown))
    {
        blockAllSignals(true);

        this->labelType->setEnabled(false);
        this->lineEditLabel->setEnabled(false);

        this->lineEditType->clear();
        this->lineEditLabel->clear();

        blockAllSignals(false);
    }

    return QObject::eventFilter(target, event);
}

void InspectorMetadataWidget::blockAllSignals(bool block)
{
    this->lineEditType->blockSignals(block);
    this->lineEditLabel->blockSignals(block);
}

void InspectorMetadataWidget::labelChanged(QString name)
{
    EventManager::getInstance().fireLabelChangedEvent(this->lineEditLabel->text());
}
