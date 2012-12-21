#include "SettingsDialog.h"
#include "AddDeviceDialog.h"

#include "DatabaseManager.h"
#include "GpiManager.h"
#include "Events/AutoSynchronizeEvent.h"
#include "Events/SynchronizeEvent.h"
#include "Models/ConfigurationModel.h"
#include "Models/GpiModel.h"

#include <QtCore/QDebug>

#include <QtGui/QComboBox>

SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent)
{
    setupUi(this);

    this->stylesheet = qApp->styleSheet();

    bool isFullscreen = (DatabaseManager::getInstance().getConfigurationByName("StartFullscreen").getValue() == "true") ? true : false;
    this->checkBoxFullscreen->setChecked(isFullscreen);

    this->spinBoxFontSize->setValue(DatabaseManager::getInstance().getConfigurationByName("FontSize").getValue().toInt());

    bool isAutoSynchronize = (DatabaseManager::getInstance().getConfigurationByName("AutoSynchronize").getValue() == "true") ? true : false;
    this->checkBoxAutoSynchronize->setChecked(isAutoSynchronize);
    this->labelInterval->setEnabled(isAutoSynchronize);
    this->spinBoxSynchronizeInterval->setEnabled(isAutoSynchronize);
    this->labelSeconds->setEnabled(isAutoSynchronize);

    this->spinBoxSynchronizeInterval->setValue(DatabaseManager::getInstance().getConfigurationByName("SynchronizeInterval").getValue().toInt());

    loadDevices();
    loadGpi();
}

void SettingsDialog::loadDevices()
{
    this->treeWidgetDevice->clear();
    this->treeWidgetDevice->setColumnHidden(0, true);
    this->treeWidgetDevice->setColumnWidth(3, 50);

    QList<DeviceModel> models = DatabaseManager::getInstance().getDevice();
    foreach (DeviceModel model, models)
    {
        QTreeWidgetItem* treeItem = new QTreeWidgetItem(this->treeWidgetDevice);
        treeItem->setText(0, QString("%1").arg(model.getId()));
        treeItem->setText(1, model.getName());
        treeItem->setText(2, model.getAddress());
        treeItem->setText(3, QString("%1").arg(model.getPort()));
        treeItem->setText(4, model.getUsername());

        QString password = model.getPassword();
        treeItem->setText(5, password.replace(QRegExp("."), "*"));

        treeItem->setText(6, model.getDescription());
        treeItem->setText(7, model.getVersion());
        treeItem->setText(8, model.getShadow());
    }

    checkEmptyDeviceList();
}

void SettingsDialog::loadGpi()
{
    QList<GpiPortModel> inputs = DatabaseManager::getInstance().getGpiPorts();

    const QList<Playout::PlayoutType::Type>& actions = Playout::enumConstants();
    this->comboBoxAction1->setCurrentIndex(actions.indexOf(inputs.at(0).getAction()));
    this->comboBoxAction2->setCurrentIndex(actions.indexOf(inputs.at(1).getAction()));
    this->comboBoxAction3->setCurrentIndex(actions.indexOf(inputs.at(2).getAction()));
    this->comboBoxAction4->setCurrentIndex(actions.indexOf(inputs.at(3).getAction()));
    this->comboBoxAction5->setCurrentIndex(actions.indexOf(inputs.at(4).getAction()));
    this->comboBoxAction6->setCurrentIndex(actions.indexOf(inputs.at(5).getAction()));
    this->comboBoxAction7->setCurrentIndex(actions.indexOf(inputs.at(6).getAction()));
    this->comboBoxAction8->setCurrentIndex(actions.indexOf(inputs.at(7).getAction()));

    this->comboBoxGpiVoltageChange1->setCurrentIndex(inputs.at(0).isRisingEdge() ? 0 : 1);
    this->comboBoxGpiVoltageChange2->setCurrentIndex(inputs.at(1).isRisingEdge() ? 0 : 1);
    this->comboBoxGpiVoltageChange3->setCurrentIndex(inputs.at(2).isRisingEdge() ? 0 : 1);
    this->comboBoxGpiVoltageChange4->setCurrentIndex(inputs.at(3).isRisingEdge() ? 0 : 1);
    this->comboBoxGpiVoltageChange5->setCurrentIndex(inputs.at(4).isRisingEdge() ? 0 : 1);
    this->comboBoxGpiVoltageChange6->setCurrentIndex(inputs.at(5).isRisingEdge() ? 0 : 1);
    this->comboBoxGpiVoltageChange7->setCurrentIndex(inputs.at(6).isRisingEdge() ? 0 : 1);
    this->comboBoxGpiVoltageChange8->setCurrentIndex(inputs.at(7).isRisingEdge() ? 0 : 1);

    QList<GpoPortModel> outputs = DatabaseManager::getInstance().getGpoPorts();

    this->comboBoxGpoVoltageChange1->setCurrentIndex(outputs.at(0).isRisingEdge() ? 0 : 1);
    this->comboBoxGpoVoltageChange2->setCurrentIndex(outputs.at(1).isRisingEdge() ? 0 : 1);
    this->comboBoxGpoVoltageChange3->setCurrentIndex(outputs.at(2).isRisingEdge() ? 0 : 1);
    this->comboBoxGpoVoltageChange4->setCurrentIndex(outputs.at(3).isRisingEdge() ? 0 : 1);
    this->comboBoxGpoVoltageChange5->setCurrentIndex(outputs.at(4).isRisingEdge() ? 0 : 1);
    this->comboBoxGpoVoltageChange6->setCurrentIndex(outputs.at(5).isRisingEdge() ? 0 : 1);
    this->comboBoxGpoVoltageChange7->setCurrentIndex(outputs.at(6).isRisingEdge() ? 0 : 1);
    this->comboBoxGpoVoltageChange8->setCurrentIndex(outputs.at(7).isRisingEdge() ? 0 : 1);

    this->spinBoxPulseLength1->setValue(outputs.at(0).getPulseLengthMillis());
    this->spinBoxPulseLength2->setValue(outputs.at(1).getPulseLengthMillis());
    this->spinBoxPulseLength3->setValue(outputs.at(2).getPulseLengthMillis());
    this->spinBoxPulseLength4->setValue(outputs.at(3).getPulseLengthMillis());
    this->spinBoxPulseLength5->setValue(outputs.at(4).getPulseLengthMillis());
    this->spinBoxPulseLength6->setValue(outputs.at(5).getPulseLengthMillis());
    this->spinBoxPulseLength7->setValue(outputs.at(6).getPulseLengthMillis());
    this->spinBoxPulseLength8->setValue(outputs.at(7).getPulseLengthMillis());

    GpiDeviceModel device = DatabaseManager::getInstance().getGpiDevice();
    this->lineEditSerialPort->setText(device.getSerialPort());
    this->comboBoxGpiBaudRate->setCurrentIndex(
            comboBoxGpiBaudRate->findText(QString("%1").arg(
                    device.getBaudRate())));
}

void SettingsDialog::checkEmptyDeviceList()
{
    if (this->treeWidgetDevice->topLevelItemCount() == 0)
    {
        this->tabWidget->setCurrentIndex(1);
        this->treeWidgetDevice->setStyleSheet("border-color: red;");
    }
    else
        this->treeWidgetDevice->setStyleSheet("");
}

void SettingsDialog::showAddDeviceDialog()
{
    AddDeviceDialog* dialog = new AddDeviceDialog(this);
    if (dialog->exec() == QDialog::Accepted)
    {
        DatabaseManager::getInstance().insertDevice(DeviceModel(0, dialog->getName(), dialog->getAddress(), dialog->getPort().toInt(), dialog->getUsername(),
                                                                dialog->getPassword(), dialog->getDescription(), dialog->getVersion(), dialog->getShadow()));
        loadDevices();

        qApp->postEvent(qApp, new SynchronizeEvent(0));
    }
}

void SettingsDialog::removeDevice()
{
    if (this->treeWidgetDevice->selectedItems().count() == 0)
        return;

    DatabaseManager::getInstance().deleteDevice(this->treeWidgetDevice->currentItem()->text(0).toInt());
    delete this->treeWidgetDevice->currentItem();

    loadDevices();

    qApp->postEvent(qApp, new SynchronizeEvent(0));
}

void SettingsDialog::startFullscreenChanged(int state)
{
    QString isFullscreen = (state == Qt::Checked) ? "true" : "false";
    DatabaseManager::getInstance().updateConfiguration(ConfigurationModel(-1, "StartFullscreen", isFullscreen));
}

void SettingsDialog::fontSizeChanged(int size)
{
    qApp->setStyleSheet(this->stylesheet + QString("QWidget { font-size: %1px; }").arg(size));
    DatabaseManager::getInstance().updateConfiguration(ConfigurationModel(-1, "FontSize", QString("%1").arg(size)));
}

void SettingsDialog::autoSynchronizeChanged(int state)
{
    QString isAutoSynchronize = (state == Qt::Checked) ? "true" : "false";
    DatabaseManager::getInstance().updateConfiguration(ConfigurationModel(-1, "AutoSynchronize", isAutoSynchronize));

    this->labelInterval->setEnabled((isAutoSynchronize == "true") ? true : false);
    this->spinBoxSynchronizeInterval->setEnabled((isAutoSynchronize == "true") ? true : false);
    this->labelSeconds->setEnabled((isAutoSynchronize == "true") ? true : false);

    qApp->postEvent(qApp, new AutoSynchronizeEvent((isAutoSynchronize == "true") ? true : false,
                                                   this->spinBoxSynchronizeInterval->value() * 1000));
}

void SettingsDialog::synchronizeIntervalChanged(int interval)
{
    DatabaseManager::getInstance().updateConfiguration(ConfigurationModel(-1, "SynchronizeInterval", QString("%1").arg(interval)));

    qApp->postEvent(qApp, new AutoSynchronizeEvent(this->checkBoxAutoSynchronize->checkState(), interval * 1000));
}

void SettingsDialog::updateGpi(int gpi, const QComboBox* voltage, const QComboBox* action)
{
    if (!isVisible()) // During construction of dialog we don't want to rewrite
        return;       // values in the database that are already there.

    bool risingEdge = voltage->currentIndex() == 0;
    Playout::PlayoutType::Type playoutType =
            Playout::enumConstants().at(action->currentIndex());
    qDebug() << "GPI " << gpi
             << " changed -- rising edge: " << risingEdge
             << " action: " << Playout::toString(playoutType);
    DatabaseManager::getInstance().updateGpiPort(
            GpiPortModel(gpi, risingEdge, playoutType));
    GpiManager::getInstance().getGpiDevice()->setupGpiPort(gpi, risingEdge);
    emit gpiBindingChanged(gpi, playoutType);
}

void SettingsDialog::gpi1Changed()
{
    updateGpi(0, comboBoxGpiVoltageChange1, comboBoxAction1);
}

void SettingsDialog::gpi2Changed()
{
    updateGpi(1, comboBoxGpiVoltageChange2, comboBoxAction2);
}

void SettingsDialog::gpi3Changed()
{
    updateGpi(2, comboBoxGpiVoltageChange3, comboBoxAction3);
}

void SettingsDialog::gpi4Changed()
{
    updateGpi(3, comboBoxGpiVoltageChange4, comboBoxAction4);
}

void SettingsDialog::gpi5Changed()
{
    updateGpi(4, comboBoxGpiVoltageChange5, comboBoxAction5);
}

void SettingsDialog::gpi6Changed()
{
    updateGpi(5, comboBoxGpiVoltageChange6, comboBoxAction6);
}

void SettingsDialog::gpi7Changed()
{
    updateGpi(6, comboBoxGpiVoltageChange7, comboBoxAction7);
}

void SettingsDialog::gpi8Changed()
{
    updateGpi(7, comboBoxGpiVoltageChange8, comboBoxAction8);
}

void SettingsDialog::updateGpo(
        int gpo, const QComboBox* voltage, const QSpinBox* pulseLength)
{
    if (!isVisible()) // During construction of dialog we don't want to rewrite
        return;       // values in the database that are already there.

    bool risingEdge = voltage->currentIndex() == 0;
    int pulseLengthMillis = pulseLength->value();
    qDebug() << "GPO " << gpo
             << " changed -- rising edge: " << risingEdge
             << " pulse length: " << pulseLengthMillis << "ms";
    DatabaseManager::getInstance().updateGpoPort(
            GpoPortModel(gpo, risingEdge, pulseLengthMillis));
    GpiManager::getInstance().getGpiDevice()->setupGpoPort(
            gpo, pulseLengthMillis, risingEdge);
}

void SettingsDialog::gpo1Changed()
{
    updateGpo(0, comboBoxGpoVoltageChange1, spinBoxPulseLength1);
}

void SettingsDialog::gpo2Changed()
{
    updateGpo(1, comboBoxGpoVoltageChange2, spinBoxPulseLength2);
}

void SettingsDialog::gpo3Changed()
{
    updateGpo(2, comboBoxGpoVoltageChange3, spinBoxPulseLength3);
}

void SettingsDialog::gpo4Changed()
{
    updateGpo(3, comboBoxGpoVoltageChange4, spinBoxPulseLength4);
}

void SettingsDialog::gpo5Changed()
{
    updateGpo(4, comboBoxGpoVoltageChange5, spinBoxPulseLength5);
}

void SettingsDialog::gpo6Changed()
{
    updateGpo(5, comboBoxGpoVoltageChange6, spinBoxPulseLength6);
}

void SettingsDialog::gpo7Changed()
{
    updateGpo(6, comboBoxGpoVoltageChange7, spinBoxPulseLength7);
}

void SettingsDialog::gpo8Changed()
{
    updateGpo(7, comboBoxGpoVoltageChange8, spinBoxPulseLength8);
}

void SettingsDialog::updateGpiDevice()
{
    if (!isVisible()) // During construction of dialog we don't want to rewrite
        return;       // values in the database that are already there.

    QString serialPort = lineEditSerialPort->text().trimmed();
    int baudRate = comboBoxGpiBaudRate->currentText().toInt();
    qDebug() << "GPO Device changed -- Serial port: "
             << serialPort << " Baud rate: " << baudRate;

    DatabaseManager::getInstance().updateGpiDevice(
            GpiDeviceModel(serialPort, baudRate));
    GpiManager::getInstance().reinitialize();
}

void SettingsDialog::serialPortChanged()
{
    updateGpiDevice();
}

void SettingsDialog::baudRateChanged(QString baudRate)
{
    updateGpiDevice();
}
