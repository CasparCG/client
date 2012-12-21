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

    this->labelContainer->setVisible(false);
    this->labelPreset->setVisible(false);
    this->labelTune->setVisible(false);
    this->comboBoxContainer->setVisible(false);
    this->comboBoxPreset->setVisible(false);
    this->comboBoxTune->setVisible(false);

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

            this->lineEditFilename->setText(this->command->getFilename());
            //this->comboBoxContainer->setCurrentIndex(this->comboBoxContainer->findText(this->model->getDeviceName()));
            this->comboBoxCodec->setCurrentIndex(this->comboBoxCodec->findText(this->command->getCodec()));
            //this->comboBoxPreset->setCurrentIndex(this->comboBoxPreset->findText(this->model->getDeviceName()));
            //this->comboBoxTune->setCurrentIndex(this->comboBoxTune->findText(this->model->getDeviceName()));

            this->preview = true;
        }
    }

    return QObject::eventFilter(target, event);
}

void InspectorFileRecorderWidget::filenameChanged(QString filename)
{
    this->command->setFilename(filename);
}

void InspectorFileRecorderWidget::containerChanged(QString container)
{
    this->command->setContainer(container);
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

void InspectorFileRecorderWidget::resetFilename(QString filename)
{
    this->lineEditFilename->setText(FileRecorder::DEFAULT_OUTPUT);
    this->command->setFilename(this->lineEditFilename->text());
}

void InspectorFileRecorderWidget::resetContainer(QString container)
{
    this->comboBoxContainer->setCurrentIndex(this->comboBoxContainer->findText(FileRecorder::DEFAULT_CONTAINER));
    this->command->setContainer(this->comboBoxContainer->currentText());
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
