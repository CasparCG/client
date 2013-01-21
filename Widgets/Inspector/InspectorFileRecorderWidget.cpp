#include "InspectorFileRecorderWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "Events/RundownItemSelectedEvent.h"
#include "Models/FormatModel.h"

InspectorFileRecorderWidget::InspectorFileRecorderWidget(QWidget* parent)
    : QWidget(parent),
      preview(false), model(NULL), command(NULL)
{
    setupUi(this);

    qApp->installEventFilter(this);
}

bool InspectorFileRecorderWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        if (dynamic_cast<FileRecorderCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->preview = false;

            this->model = rundownItemSelectedEvent->getLibraryModel();
            this->command = dynamic_cast<FileRecorderCommand*>(rundownItemSelectedEvent->getCommand());

            this->lineEditOutput->setText(this->command->getOutput());
            this->comboBoxCodec->setCurrentIndex(this->comboBoxCodec->findText(this->command->getCodec()));
            this->comboBoxPreset->setCurrentIndex(this->comboBoxPreset->findText(this->command->getPreset()));
            this->comboBoxTune->setCurrentIndex(this->comboBoxTune->findText(this->command->getTune()));
            this->checkBoxWithAlpha->setChecked(this->command->getWithAlpha());

            this->preview = true;
        }
    }

    return QObject::eventFilter(target, event);
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
