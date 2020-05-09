#include "InspectorOscOutputWidget.h"

#include "Global.h"

#include "EventManager.h"
#include "DatabaseManager.h"

InspectorOscOutputWidget::InspectorOscOutputWidget(QWidget *parent)
    : QWidget(parent),
      command(0)
{
    setupUi(this);

    this->lineEditPath->setToolTip(getOscFilters());

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(oscOutputChanged(const OscOutputChangedEvent&)), this, SLOT(oscOutputChanged(const OscOutputChangedEvent&)));

    loadOscOutput();
}

QString InspectorOscOutputWidget::getOscFilters()
{
    QString item = QString("<b>Item Control</b><br>"
                           "%1<br>%2<br>%3<br>%4<br>%5<br>%6<br>%7<br>%8<br>%9<br>%10<br>%11<br>%12")
                           .arg(Osc::ITEM_CONTROL_STOP_FILTER)
                           .arg(Osc::ITEM_CONTROL_PLAY_FILTER)
                           .arg(Osc::ITEM_CONTROL_PLAYNOW_FILTER)
                           .arg(Osc::ITEM_CONTROL_LOAD_FILTER)
                           .arg(Osc::ITEM_CONTROL_PAUSE_FILTER)
                           .arg(Osc::ITEM_CONTROL_NEXT_FILTER)
                           .arg(Osc::ITEM_CONTROL_UPDATE_FILTER)
                           .arg(Osc::ITEM_CONTROL_INVOKE_FILTER)
                           .arg(Osc::ITEM_CONTROL_PREVIEW_FILTER)
                           .arg(Osc::ITEM_CONTROL_CLEAR_FILTER)
                           .arg(Osc::ITEM_CONTROL_CLEARVIDEOLAYER_FILTER)
                           .arg(Osc::ITEM_CONTROL_CLEARCHANNEL_FILTER);

    QString rundown = QString("<b>Rundown Control</b><br>"
                              "%1<br>%2<br>%3<br>%4<br>%5<br>%6<br>%7<br>%8<br>%9<br>%10<br>%11<br>%12<br>%13<br>%14<br>%15<br>%16<br>%17")
                              .arg(Osc::RUNDOWN_CONTROL_UP_FILTER)
                              .arg(Osc::RUNDOWN_CONTROL_DOWN_FILTER)
                              .arg(Osc::RUNDOWN_CONTROL_STOP_FILTER)
                              .arg(Osc::RUNDOWN_CONTROL_PLAY_FILTER)
                              .arg(Osc::RUNDOWN_CONTROL_PLAYNOW_FILTER)
                              .arg(Osc::RUNDOWN_CONTROL_LOAD_FILTER)
                              .arg(Osc::RUNDOWN_CONTROL_PAUSE_FILTER)
                              .arg(Osc::RUNDOWN_CONTROL_NEXT_FILTER)
                              .arg(Osc::RUNDOWN_CONTROL_UPDATE_FILTER)
                              .arg(Osc::RUNDOWN_CONTROL_INVOKE_FILTER)
                              .arg(Osc::RUNDOWN_CONTROL_PREVIEW_FILTER)
                              .arg(Osc::RUNDOWN_CONTROL_CLEAR_FILTER)
                              .arg(Osc::RUNDOWN_CONTROL_CLEARVIDEOLAYER_FILTER)
                              .arg(Osc::RUNDOWN_CONTROL_CLEARCHANNELFILTER)
                              .arg(Osc::RUNDOWN_CONTROL_PLAYNOWIFCHANNEL_FILTER)
                              .arg(Osc::RUNDOWN_CONTROL_PLAYANDAUTOSTEP_FILTER)
                              .arg(Osc::RUNDOWN_CONTROL_PLAYNOWANDAUTOSTEP_FILTER);

    return QString("%1<br><br>%2").arg(item).arg(rundown);
}

void InspectorOscOutputWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->command = nullptr;

    blockAllSignals(true);

    if (dynamic_cast<OscOutputCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<OscOutputCommand*>(event.getCommand());

        this->comboBoxOutput->setCurrentIndex(this->comboBoxOutput->findText(this->command->getOutput()));
        this->lineEditPath->setText(this->command->getPath());
        this->lineEditMessage->setText(this->command->getMessage());
        this->comboBoxType->setCurrentIndex(this->comboBoxType->findText(this->command->getType()));
        this->checkBoxTriggerOnNext->setChecked(this->command->getTriggerOnNext());
        this->checkBoxUseBundle->setChecked(this->command->getUseBundle());
    }

    checkEmptyOutput();
    checkEmptyPath();

    blockAllSignals(false);
}

void InspectorOscOutputWidget::oscOutputChanged(const OscOutputChangedEvent& event)
{
    Q_UNUSED(event);

    blockAllSignals(true);

    loadOscOutput();

    checkEmptyOutput();
    checkEmptyPath();

    blockAllSignals(false);
}

void InspectorOscOutputWidget::loadOscOutput()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxOutput->blockSignals(true);

    this->comboBoxOutput->clear();
    QList<OscOutputModel> models = DatabaseManager::getInstance().getOscOutput();
    foreach (OscOutputModel model, models)
        this->comboBoxOutput->addItem(model.getName());

    this->comboBoxOutput->blockSignals(false);
}

void InspectorOscOutputWidget::blockAllSignals(bool block)
{
    this->comboBoxOutput->blockSignals(block);
    this->lineEditPath->blockSignals(block);
    this->lineEditMessage->blockSignals(block);
    this->comboBoxType->blockSignals(block);
    this->checkBoxTriggerOnNext->blockSignals(block);
    this->checkBoxUseBundle->blockSignals(block);
}

void InspectorOscOutputWidget::checkEmptyOutput()
{
    if (this->comboBoxOutput->isEnabled() && this->comboBoxOutput->currentText() == "")
        this->comboBoxOutput->setStyleSheet("border-color: firebrick;");
    else
        this->comboBoxOutput->setStyleSheet("");
}

void InspectorOscOutputWidget::checkEmptyPath()
{
    if (this->lineEditPath->text().isEmpty())
        this->lineEditPath->setStyleSheet("border-color: firebrick;");
    else
        this->lineEditPath->setStyleSheet("");
}

void InspectorOscOutputWidget::outputChanged(QString output)
{
    this->command->setOutput(output);

    checkEmptyOutput();
}

void InspectorOscOutputWidget::pathChanged(QString path)
{
    this->command->setPath(path);

    checkEmptyPath();
}

void InspectorOscOutputWidget::messageChanged(QString message)
{
    this->command->setMessage(message);
}

void InspectorOscOutputWidget::typeChanged(QString type)
{
    this->command->setType(type);
}

void InspectorOscOutputWidget::triggerOnNextChanged(int state)
{
    this->command->setTriggerOnNext((state == Qt::Checked) ? true : false);
}

void InspectorOscOutputWidget::useBundleChanged(int state)
{
    this->command->setUseBundle((state == Qt::Checked) ? true : false);
}
