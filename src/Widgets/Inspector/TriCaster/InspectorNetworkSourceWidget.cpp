#include "InspectorNetworkSourceWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Models/TriCaster/TriCasterNetworkTargetModel.h"

#include <QtGui/QApplication>

InspectorNetworkSourceWidget::InspectorNetworkSourceWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));

    loadTriCasterNetworkTarget();
}

void InspectorNetworkSourceWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<NetworkSourceCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<NetworkSourceCommand*>(event.getCommand());

        this->comboBoxTarget->setCurrentIndex(this->comboBoxTarget->findData(this->command->getTarget()));
        this->lineEditSource->setText(this->command->getSource());
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
    }

    blockAllSignals(false);
}

void InspectorNetworkSourceWidget::blockAllSignals(bool block)
{
    this->comboBoxTarget->blockSignals(block);
    this->lineEditSource->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
}

void InspectorNetworkSourceWidget::loadTriCasterNetworkTarget()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTarget->blockSignals(true);

    QList<TriCasterNetworkTargetModel> models = DatabaseManager::getInstance().getTriCasterNetworkTarget();
    foreach (TriCasterNetworkTargetModel model, models)
        this->comboBoxTarget->addItem(model.getName(), model.getValue());

    this->comboBoxTarget->blockSignals(false);
}

void InspectorNetworkSourceWidget::targetChanged(int index)
{
    this->command->setTarget(this->comboBoxTarget->itemData(index).toString());
}

void InspectorNetworkSourceWidget::sourceChanged(QString source)
{
    this->command->setSource(source);
}

void InspectorNetworkSourceWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}
