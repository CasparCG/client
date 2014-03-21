#include "InspectorFileRecorderWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Models/FormatModel.h"

InspectorFileRecorderWidget::InspectorFileRecorderWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
}

void InspectorFileRecorderWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<FileRecorderCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<FileRecorderCommand*>(event.getCommand());

        this->lineEditOutput->setText(this->command->getOutput());
        this->comboBoxCodec->setCurrentIndex(this->comboBoxCodec->findText(this->command->getCodec()));
        this->comboBoxPreset->setCurrentIndex(this->comboBoxPreset->findText(this->command->getPreset()));
        this->comboBoxTune->setCurrentIndex(this->comboBoxTune->findText(this->command->getTune()));
        this->checkBoxWithAlpha->setChecked(this->command->getWithAlpha());
    }

    blockAllSignals(false);
}

void InspectorFileRecorderWidget::blockAllSignals(bool block)
{
    this->lineEditOutput->blockSignals(block);
    this->comboBoxCodec->blockSignals(block);
    this->comboBoxPreset->blockSignals(block);
    this->comboBoxTune->blockSignals(block);
    this->checkBoxWithAlpha->blockSignals(block);
}

void InspectorFileRecorderWidget::outputChanged(QString output)
{
    this->command->setOutput(output);
}

void InspectorFileRecorderWidget::codecChanged(QString codec)
{
    this->command->setCodec(codec);
}

void InspectorFileRecorderWidget::presetChanged(QString preset)
{
    this->command->setPreset(preset);
}

void InspectorFileRecorderWidget::tuneChanged(QString tune)
{
    this->command->setTune(tune);
}

void InspectorFileRecorderWidget::withAlphaChanged(int state)
{
    this->command->setWithAlpha((state == Qt::Checked) ? true : false);
}

void InspectorFileRecorderWidget::resetOutput(QString output)
{
    this->lineEditOutput->setText(FileRecorder::DEFAULT_OUTPUT);
    this->command->setOutput(this->lineEditOutput->text());
}

void InspectorFileRecorderWidget::resetCodec(QString codec)
{
    this->comboBoxCodec->setCurrentIndex(this->comboBoxCodec->findText(FileRecorder::DEFAULT_CODEC));
    this->command->setCodec(this->comboBoxCodec->currentText());
}

void InspectorFileRecorderWidget::resetPreset(QString preset)
{
    this->comboBoxPreset->setCurrentIndex(this->comboBoxPreset->findText(FileRecorder::DEFAULT_PRESET));
    this->command->setPreset(this->comboBoxPreset->currentText());
}

void InspectorFileRecorderWidget::resetTune(QString tune)
{
    this->comboBoxTune->setCurrentIndex(this->comboBoxTune->findText(FileRecorder::DEFAULT_TUNE));
    this->command->setTune(this->comboBoxTune->currentText());
}

void InspectorFileRecorderWidget::resetWithAlpha(QString withAlpha)
{
    this->checkBoxWithAlpha->setChecked(false);
    this->command->setWithAlpha(this->checkBoxWithAlpha->isChecked());
}
