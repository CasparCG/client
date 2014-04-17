#include "InspectorAtemAudioGainWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Models/Atem/AtemAudioInputModel.h"

#include <QtGui/QApplication>

InspectorAtemAudioGainWidget::InspectorAtemAudioGainWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));

    loadAtemSource();
}

void InspectorAtemAudioGainWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<AtemAudioGainCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<AtemAudioGainCommand*>(event.getCommand());

        this->comboBoxSource->setCurrentIndex(this->comboBoxSource->findData(this->command->getSource()));
        this->sliderGain->setValue(this->command->getGain() * 100);
        this->doubleSpinBoxGain->setValue(this->command->getGain());
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
    }

    blockAllSignals(false);
}

void InspectorAtemAudioGainWidget::blockAllSignals(bool block)
{
    this->comboBoxSource->blockSignals(block);
    this->sliderGain->blockSignals(block);
    this->doubleSpinBoxGain->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
}

void InspectorAtemAudioGainWidget::loadAtemSource()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxSource->blockSignals(true);

    this->comboBoxSource->addItem("Master", "0");

    QList<AtemAudioInputModel> models = DatabaseManager::getInstance().getAtemAudioInput();
    foreach (AtemAudioInputModel model, models)
        this->comboBoxSource->addItem(model.getName(), model.getValue());

    this->comboBoxSource->blockSignals(false);
}

void InspectorAtemAudioGainWidget::sourceChanged(int index)
{
    this->command->setSource(this->comboBoxSource->itemData(index).toString());
}

void InspectorAtemAudioGainWidget::sliderGainChanged(int value)
{
    this->command->setGain(static_cast<float>(value) / 100);

    this->doubleSpinBoxGain->setValue(static_cast<float>(value) / 100);
}

void InspectorAtemAudioGainWidget::doubleSpinBoxGainChanged(double value)
{
    this->sliderGain->setValue(value * 100);
}

void InspectorAtemAudioGainWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}
