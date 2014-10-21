#include "InspectorMetadataWidget.h"

#include "Global.h"

#include "CasparDevice.h"

#include "EventManager.h"
#include "DeviceManager.h"
#include "DatabaseManager.h"
#include "Events/Inspector/LabelChangedEvent.h"

#include <QtGui/QApplication>

InspectorMetadataWidget::InspectorMetadataWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(libraryItemSelected(const LibraryItemSelectedEvent&)), this, SLOT(libraryItemSelected(const LibraryItemSelectedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(emptyRundown(const EmptyRundownEvent&)), this, SLOT(emptyRundown(const EmptyRundownEvent&)));
}

void InspectorMetadataWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    this->lineEditLabel->setEnabled(true);
    this->lineEditLabel->setReadOnly(false);
    this->lineEditLabel->setText(this->model->getLabel());

    blockAllSignals(false);
}

void InspectorMetadataWidget::libraryItemSelected(const LibraryItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    this->lineEditLabel->setEnabled(false);
    this->lineEditLabel->clear();

    blockAllSignals(false);
}

void InspectorMetadataWidget::emptyRundown(const EmptyRundownEvent& event)
{
    blockAllSignals(true);

    this->lineEditLabel->setEnabled(false);
    this->lineEditLabel->clear();

    blockAllSignals(false);
}

void InspectorMetadataWidget::blockAllSignals(bool block)
{
    this->lineEditLabel->blockSignals(block);
}

void InspectorMetadataWidget::labelChanged(QString name)
{
    EventManager::getInstance().fireLabelChangedEvent(LabelChangedEvent(this->lineEditLabel->text()));
}
