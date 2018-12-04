#include "InspectorFileRecorderWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Models/FormatModel.h"

#include <QtCore/QFileInfo>

InspectorFileRecorderWidget::InspectorFileRecorderWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
}

void InspectorFileRecorderWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->command = nullptr;
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<FileRecorderCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<FileRecorderCommand*>(event.getCommand());

        this->lineEditOutput->setText(this->command->getOutput());
        this->comboBoxCodec->setCurrentIndex(this->comboBoxCodec->findText(this->command->getCodec()));
        this->lineEditPreset->setText(this->command->getPreset());
        this->checkBoxWithAlpha->setChecked(this->command->getWithAlpha());
    }

    blockAllSignals(false);
}

void InspectorFileRecorderWidget::blockAllSignals(bool block)
{
    this->lineEditOutput->blockSignals(block);
    this->comboBoxCodec->blockSignals(block);
    this->lineEditPreset->blockSignals(block);
    this->checkBoxWithAlpha->blockSignals(block);
}

void InspectorFileRecorderWidget::outputChanged(QString output)
{
    this->command->setOutput(output);
}

void InspectorFileRecorderWidget::codecChanged(QString codec)
{
    this->command->setCodec(codec);

    QFileInfo output(this->command->getOutput());
    if (codec == FileRecorder::DEFAULT_DNXHD_CODEC)
    {
        this->lineEditOutput->setText(output.completeBaseName() + ".mxf");
        this->lineEditPreset->setText(FileRecorder::DEFAULT_DNXHD_PRESET);
    }
    else if (codec == FileRecorder::DEFAULT_H264_CODEC)
    {
        this->lineEditOutput->setText(output.completeBaseName() + ".mp4");
        this->lineEditPreset->setText(FileRecorder::DEFAULT_H264_PRESET);
    }
}

void InspectorFileRecorderWidget::presetChanged(QString preset)
{
    this->command->setPreset(preset);
}

void InspectorFileRecorderWidget::withAlphaChanged(int state)
{
    this->command->setWithAlpha((state == Qt::Checked) ? true : false);
}

void InspectorFileRecorderWidget::resetOutput(QString output)
{
    Q_UNUSED(output);

    this->lineEditOutput->setText(FileRecorder::DEFAULT_OUTPUT);
    this->command->setOutput(this->lineEditOutput->text());
}

void InspectorFileRecorderWidget::resetCodec(QString codec)
{
    Q_UNUSED(codec);

    this->comboBoxCodec->setCurrentIndex(this->comboBoxCodec->findText(FileRecorder::DEFAULT_CODEC));
    this->command->setCodec(this->comboBoxCodec->currentText());
}

void InspectorFileRecorderWidget::resetPreset(QString preset)
{
    Q_UNUSED(preset);

    this->lineEditPreset->setText(FileRecorder::DEFAULT_PRESET);
    this->command->setPreset(this->lineEditPreset->text());
}

void InspectorFileRecorderWidget::resetWithAlpha(QString withAlpha)
{
    Q_UNUSED(withAlpha);

    this->checkBoxWithAlpha->setChecked(false);
    this->command->setWithAlpha(this->checkBoxWithAlpha->isChecked());
}
