#include "InspectorOutputWidget.h"

#include "Global.h"

#include "Commands/CommitCommand.h"
#include "Commands/GpiOutputCommand.h"
#include "Commands/GroupCommand.h"
#include "Commands/FileRecorderCommand.h"
#include "Commands/SeparatorCommand.h"
#include "Commands/PrintCommand.h"
#include "Commands/GridCommand.h"
#include "Events/RundownItemSelectedEvent.h"

InspectorOutputWidget::InspectorOutputWidget(QWidget* parent)
    : QWidget(parent),
      command(NULL), model(NULL)
{
    setupUi(this);

    qApp->installEventFilter(this);
}

bool InspectorOutputWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        this->model = rundownItemSelectedEvent->getLibraryModel();

        blockAllSignals(true);

        this->spinBoxChannel->setEnabled(true);
        this->spinBoxVideolayer->setEnabled(true);
        this->spinBoxDelay->setEnabled(true);

        if (rundownItemSelectedEvent->getCommand() != NULL && rundownItemSelectedEvent->getLibraryModel() != NULL)
        {
            this->command = rundownItemSelectedEvent->getCommand();

            this->spinBoxChannel->setEnabled(true);
            this->spinBoxVideolayer->setEnabled(true);

            this->spinBoxChannel->setValue(this->command->getChannel());
            this->spinBoxVideolayer->setValue(this->command->getVideolayer());
            this->spinBoxDelay->setValue(this->command->getDelay());
            this->checkBoxAllowGpi->setChecked(this->command->getAllowGpi());

            if (dynamic_cast<GpiOutputCommand*>(rundownItemSelectedEvent->getCommand()))
            {
                this->spinBoxChannel->setEnabled(false);
                this->spinBoxVideolayer->setEnabled(false);

                this->spinBoxChannel->setValue(Output::DEFAULT_CHANNEL);
                this->spinBoxVideolayer->setValue(Output::DEFAULT_VIDEOLAYER);
            }
            else if (dynamic_cast<CommitCommand*>(rundownItemSelectedEvent->getCommand()) ||
                     dynamic_cast<PrintCommand*>(rundownItemSelectedEvent->getCommand()) ||
                     dynamic_cast<FileRecorderCommand*>(rundownItemSelectedEvent->getCommand()) ||
                     dynamic_cast<GridCommand*>(rundownItemSelectedEvent->getCommand()))
            {
                this->spinBoxVideolayer->setEnabled(false);

                this->spinBoxVideolayer->setValue(Output::DEFAULT_VIDEOLAYER);
            }
            else if (dynamic_cast<GroupCommand*>(rundownItemSelectedEvent->getCommand()))
            {
                this->spinBoxChannel->setEnabled(false);
                this->spinBoxVideolayer->setEnabled(false);
                this->spinBoxDelay->setEnabled(false);

                this->spinBoxChannel->setValue(Output::DEFAULT_CHANNEL);
                this->spinBoxVideolayer->setValue(Output::DEFAULT_VIDEOLAYER);
                this->spinBoxDelay->setValue(Output::DEFAULT_DELAY);
            }
            else if (dynamic_cast<SeparatorCommand*>(rundownItemSelectedEvent->getCommand()))
            {

                this->spinBoxChannel->setEnabled(false);
                this->spinBoxVideolayer->setEnabled(false);
                this->spinBoxDelay->setEnabled(false);

                this->spinBoxChannel->setValue(Output::DEFAULT_CHANNEL);
                this->spinBoxVideolayer->setValue(Output::DEFAULT_VIDEOLAYER);
                this->spinBoxDelay->setValue(Output::DEFAULT_DELAY);
                this->checkBoxAllowGpi->setChecked(Output::DEFAULT_ALLOW_GPI);
            } 

            blockAllSignals(false);
        }
    }

    return QObject::eventFilter(target, event);
}

void InspectorOutputWidget::blockAllSignals(bool block)
{
    this->spinBoxChannel->blockSignals(block);
    this->spinBoxVideolayer->blockSignals(block);
    this->spinBoxDelay->blockSignals(block);
    this->checkBoxAllowGpi->blockSignals(block);
}

void InspectorOutputWidget::allowGpiChanged(int state)
{
    this->command->setAllowGpi((state == Qt::Checked) ? true : false);
}

void InspectorOutputWidget::channelChanged(int channel)
{
    this->command->setChannel(channel);
}

void InspectorOutputWidget::videolayerChanged(int videolayer)
{
    this->command->setVideolayer(videolayer);
}

void InspectorOutputWidget::delayChanged(int delay)
{
    this->command->setDelay(delay);
}

void InspectorOutputWidget::resetAllowGpi(QString)
{
    this->checkBoxAllowGpi->setChecked(Output::DEFAULT_ALLOW_GPI);
    this->command->setAllowGpi(this->checkBoxAllowGpi->isChecked());
}

void InspectorOutputWidget::resetChannel(QString channel)
{
    this->spinBoxChannel->setValue(Output::DEFAULT_CHANNEL);
    this->command->setChannel(this->spinBoxChannel->value());
}

void InspectorOutputWidget::resetVideolayer(QString videolayer)
{
    this->spinBoxVideolayer->setValue(Output::DEFAULT_VIDEOLAYER);
    this->command->setVideolayer(this->spinBoxVideolayer->value());
}

void InspectorOutputWidget::resetDelay(QString delay)
{
    this->spinBoxDelay->setValue(Output::DEFAULT_DELAY);
    this->command->setDelay(this->spinBoxDelay->value());
}
