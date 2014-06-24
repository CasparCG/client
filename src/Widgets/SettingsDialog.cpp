#include "SettingsDialog.h"
#include "DeviceDialog.h"
#include "AtemDeviceDialog.h"
#include "TriCasterDeviceDialog.h"
#include "OscOutputDialog.h"
#include "ImportDeviceDialog.h"
#include "ImportAtemDeviceDialog.h"
#include "ImportTriCasterDeviceDialog.h"

#include "DatabaseManager.h"
#include "GpiManager.h"
#include "EventManager.h"
#include "Events/OscOutputChangedEvent.h"
#include "Events/Atem/AtemDeviceChangedEvent.h"
#include "Events/Library/RefreshLibraryEvent.h"
#include "Events/Library/AutoRefreshLibraryEvent.h"
#include "Events/Rundown/SaveRundownEvent.h"
#include "Events/TriCaster/TriCasterDeviceChangedEvent.h"
#include "Models/ConfigurationModel.h"
#include "Models/DeviceModel.h"
#include "Models/GpiModel.h"
#include "Models/TriCaster/TriCasterProductModel.h"
#include "Models/TriCaster/TriCasterDeviceModel.h"
#include "Models/DeviceModel.h"
#include "Models/OscOutputModel.h"

#include <QtCore/QTimer>
#include <QtCore/QRegExp>

#include <QtGui/QComboBox>
#include <QtGui/QFileDialog>
#include <QtGui/QIcon>
#include <QtGui/QMessageBox>

SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent)
{
    setupUi(this);

    this->stylesheet = qApp->styleSheet();

    bool startFullscreen = (DatabaseManager::getInstance().getConfigurationByName("StartFullscreen").getValue() == "true") ? true : false;
    this->checkBoxFullscreen->setChecked(startFullscreen);

    this->comboBoxTheme->setCurrentIndex(this->comboBoxTheme->findText(DatabaseManager::getInstance().getConfigurationByName("Theme").getValue()));
    this->spinBoxFontSize->setValue(DatabaseManager::getInstance().getConfigurationByName("FontSize").getValue().toInt());

    bool autoRefreshLibrary = (DatabaseManager::getInstance().getConfigurationByName("AutoRefreshLibrary").getValue() == "true") ? true : false;
    this->checkBoxAutoRefresh->setChecked(autoRefreshLibrary);
    this->labelInterval->setEnabled(autoRefreshLibrary);
    this->spinBoxRefreshInterval->setEnabled(autoRefreshLibrary);
    this->labelSeconds->setEnabled(autoRefreshLibrary);

    this->spinBoxRefreshInterval->setValue(DatabaseManager::getInstance().getConfigurationByName("RefreshLibraryInterval").getValue().toInt());

    bool showThumbnailTooltip = (DatabaseManager::getInstance().getConfigurationByName("ShowThumbnailTooltip").getValue() == "true") ? true : false;
    this->checkBoxShowThumbnailTooltip->setChecked(showThumbnailTooltip);

    bool reverseOscTime = (DatabaseManager::getInstance().getConfigurationByName("ReverseOscTime").getValue() == "true") ? true : false;
    this->checkBoxReverseOscTime->setChecked(reverseOscTime);

    bool enableOscInput = (DatabaseManager::getInstance().getConfigurationByName("EnableOscInput").getValue() == "true") ? true : false;
    this->checkBoxEnableOscInput->setChecked(enableOscInput);
    this->labelOscPort->setEnabled(enableOscInput);
    this->lineEditOscInputPort->setEnabled(enableOscInput);

    bool disableInAndOutPoints = (DatabaseManager::getInstance().getConfigurationByName("DisableInAndOutPoints").getValue() == "true") ? true : false;
    this->checkBoxDisableInAndOutPoints->setChecked(disableInAndOutPoints);
    this->lineEditRundownRepository->setText(DatabaseManager::getInstance().getConfigurationByName("RundownRepository").getValue());
    bool previewOnAutoStep = (DatabaseManager::getInstance().getConfigurationByName("PreviewOnAutoStep").getValue() == "true") ? true : false;
    this->checkBoxPreviewOnAutoStep->setChecked(previewOnAutoStep);
    bool clearDelayedCommandsOnAutoStep = (DatabaseManager::getInstance().getConfigurationByName("ClearDelayedCommandsOnAutoStep").getValue() == "true") ? true : false;
    this->checkBoxClearDelayedCommandsOnAutoStep->setChecked(clearDelayedCommandsOnAutoStep);
    bool markUsedItems = (DatabaseManager::getInstance().getConfigurationByName("MarkUsedItems").getValue() == "true") ? true : false;
    this->checkBoxMarkUsedItems->setChecked(markUsedItems);

    bool showPreviewPanel = (DatabaseManager::getInstance().getConfigurationByName("ShowPreviewPanel").getValue() == "true") ? true : false;
    this->checkBoxShowPreview->setChecked(showPreviewPanel);
    bool showLivePanel = (DatabaseManager::getInstance().getConfigurationByName("ShowLivePanel").getValue() == "true") ? true : false;
    this->checkBoxShowLive->setChecked(showLivePanel);
    bool showAudioLevelsPanel = (DatabaseManager::getInstance().getConfigurationByName("ShowAudioLevelsPanel").getValue() == "true") ? true : false;
    this->checkBoxShowAudioLevels->setChecked(showAudioLevelsPanel);
    this->comboBoxDelayType->setCurrentIndex(this->comboBoxDelayType->findText(DatabaseManager::getInstance().getConfigurationByName("DelayType").getValue()));

    bool storeThumbnailsInDatabase = (DatabaseManager::getInstance().getConfigurationByName("StoreThumbnailsInDatabase").getValue() == "true") ? true : false;
    this->checkBoxStoreThumbnailsInDatabase->setChecked(storeThumbnailsInDatabase);

    this->lineEditOscInputPort->setPlaceholderText(QString("%1").arg(Osc::DEFAULT_PORT));
    QString oscPort = DatabaseManager::getInstance().getConfigurationByName("OscPort").getValue();
    if (!oscPort.isEmpty())
        this->lineEditOscInputPort->setText(oscPort);

    loadDevice();
    loadTriCasterDevice();
    loadAtemDevice();
    loadGpi();
    loadOscOutput();
    loadTriCasterProduct();

    this->comboBoxTriCasterProduct->setCurrentIndex(this->comboBoxTriCasterProduct->findText(DatabaseManager::getInstance().getConfigurationByName("TriCasterProduct").getValue()));
}

void SettingsDialog::blockAllSignals(bool block)
{
    this->comboBoxTriCasterProduct->blockSignals(block);
}

void SettingsDialog::loadDevice()
{
    this->treeWidgetDevice->clear();
    this->treeWidgetDevice->headerItem()->setText(1, "");
    this->treeWidgetDevice->setColumnHidden(0, true);
    this->treeWidgetDevice->setColumnWidth(1, 25);
    this->treeWidgetDevice->setColumnWidth(4, 50);

    QList<DeviceModel> models = DatabaseManager::getInstance().getDevice();
    foreach (DeviceModel model, models)
    {
        QTreeWidgetItem* treeItem = new QTreeWidgetItem(this->treeWidgetDevice);
        treeItem->setText(0, QString("%1").arg(model.getId()));
        treeItem->setIcon(1, QIcon(":/Graphics/Images/ServerSmall.png"));
        treeItem->setText(2, model.getName());
        treeItem->setText(3, model.getAddress());
        treeItem->setText(4, QString("%1").arg(model.getPort()));
        treeItem->setText(5, model.getDescription());
        treeItem->setText(6, model.getUsername());

        QString password = model.getPassword();
        treeItem->setText(7, password.replace(QRegExp("."), "*"));

        treeItem->setText(8, model.getVersion());
        treeItem->setText(9, model.getShadow());

        if (model.getChannels() > 0)
            treeItem->setText(10, QString("%1").arg(model.getChannels()));

        treeItem->setText(11, model.getChannelFormats());

        if (model.getPreviewChannel() > 0)
            treeItem->setText(12, QString("%1").arg(model.getPreviewChannel()));
    }

    checkEmptyDeviceList();
}

void SettingsDialog::loadTriCasterDevice()
{
    this->treeWidgetTriCasterDevice->clear();
    this->treeWidgetTriCasterDevice->headerItem()->setText(1, "");
    this->treeWidgetTriCasterDevice->setColumnHidden(0, true);
    this->treeWidgetTriCasterDevice->setColumnWidth(1, 25);
    this->treeWidgetTriCasterDevice->setColumnWidth(4, 50);

    QList<TriCasterDeviceModel> models = DatabaseManager::getInstance().getTriCasterDevice();
    foreach (TriCasterDeviceModel model, models)
    {
        QTreeWidgetItem* treeItem = new QTreeWidgetItem(this->treeWidgetTriCasterDevice);
        treeItem->setText(0, QString("%1").arg(model.getId()));
        treeItem->setIcon(1, QIcon(":/Graphics/Images/ServerSmall.png"));
        treeItem->setText(2, model.getName());
        treeItem->setText(3, model.getAddress());
        treeItem->setText(4, QString("%1").arg(model.getPort()));
        treeItem->setText(5, model.getDescription());
    }

    checkEmptyTriCasterDeviceList();
}

void SettingsDialog::loadTriCasterProduct()
{
    blockAllSignals(true);

    this->comboBoxTriCasterProduct->clear();

    QList<TriCasterProductModel> models = DatabaseManager::getInstance().getTriCasterProduct();
    foreach (TriCasterProductModel model, models)
        this->comboBoxTriCasterProduct->addItem(model.getName());

    blockAllSignals(false);
}

void SettingsDialog::loadAtemDevice()
{
    this->treeWidgetAtemDevice->clear();
    this->treeWidgetAtemDevice->headerItem()->setText(1, "");
    this->treeWidgetAtemDevice->setColumnHidden(0, true);
    this->treeWidgetAtemDevice->setColumnWidth(1, 25);
    this->treeWidgetAtemDevice->setColumnWidth(4, 50);

    QList<AtemDeviceModel> models = DatabaseManager::getInstance().getAtemDevice();
    foreach (AtemDeviceModel model, models)
    {
        QTreeWidgetItem* treeItem = new QTreeWidgetItem(this->treeWidgetAtemDevice);
        treeItem->setText(0, QString("%1").arg(model.getId()));
        treeItem->setIcon(1, QIcon(":/Graphics/Images/ServerSmall.png"));
        treeItem->setText(2, model.getName());
        treeItem->setText(3, model.getAddress());
        treeItem->setText(4, model.getDescription());
    }

    checkEmptyAtemDeviceList();
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

    QString serialPort = DatabaseManager::getInstance().getConfigurationByName("GpiSerialPort").getValue();
    int baudRate = DatabaseManager::getInstance().getConfigurationByName("GpiBaudRate").getValue().toInt();

    this->lineEditSerialPort->setText(serialPort);
    this->comboBoxGpiBaudRate->setCurrentIndex(comboBoxGpiBaudRate->findText(QString("%1").arg(baudRate)));
}

void SettingsDialog::loadOscOutput()
{
    this->treeWidgetOscOutput->clear();
    this->treeWidgetOscOutput->headerItem()->setText(1, "");
    this->treeWidgetOscOutput->setColumnHidden(0, true);
    this->treeWidgetOscOutput->setColumnWidth(1, 25);
    this->treeWidgetOscOutput->setColumnWidth(4, 50);

    QList<OscOutputModel> models = DatabaseManager::getInstance().getOscOutput();
    foreach (OscOutputModel model, models)
    {
        QTreeWidgetItem* treeItem = new QTreeWidgetItem(this->treeWidgetOscOutput);
        treeItem->setText(0, QString("%1").arg(model.getId()));
        treeItem->setIcon(1, QIcon(":/Graphics/Images/ServerSmall.png"));
        treeItem->setText(2, model.getName());
        treeItem->setText(3, model.getAddress());
        treeItem->setText(4, QString("%1").arg(model.getPort()));
        treeItem->setText(5, model.getDescription());
    }

    checkEmptyOscOutputList();
}

void SettingsDialog::checkEmptyDeviceList()
{
    if (this->treeWidgetDevice->invisibleRootItem()->childCount() == 0)
    {
        this->tabWidgetSettings->setCurrentIndex(1);
        this->treeWidgetDevice->setStyleSheet("border-color: firebrick;");
    }
    else
        this->treeWidgetDevice->setStyleSheet("");
}

void SettingsDialog::checkEmptyTriCasterDeviceList()
{
    if (this->treeWidgetTriCasterDevice->invisibleRootItem()->childCount() == 0)
        this->treeWidgetTriCasterDevice->setStyleSheet("border-color: firebrick;");
    else
        this->treeWidgetTriCasterDevice->setStyleSheet("");
}

void SettingsDialog::checkEmptyAtemDeviceList()
{
    if (this->treeWidgetAtemDevice->invisibleRootItem()->childCount() == 0)
        this->treeWidgetAtemDevice->setStyleSheet("border-color: firebrick;");
    else
        this->treeWidgetAtemDevice->setStyleSheet("");
}

void SettingsDialog::checkEmptyOscOutputList()
{
    if (this->treeWidgetOscOutput->invisibleRootItem()->childCount() == 0)
        this->treeWidgetOscOutput->setStyleSheet("border-color: firebrick;");
    else
        this->treeWidgetOscOutput->setStyleSheet("");
}

void SettingsDialog::showImportDeviceDialog()
{
    QString path("./CasparCG.xml");

    QFile file(path);
    if (!file.exists())
        path = QFileDialog::getOpenFileName(this, "Import CasparCG Servers", "", "CasparCG (*.xml)");

    if (!path.isEmpty())
    {
        ImportDeviceDialog* dialog = new ImportDeviceDialog(this);
        dialog->setImportFile(path);
        if (dialog->exec() == QDialog::Accepted)
        {
            QList<DeviceModel> models = dialog->getDevice();
            foreach (DeviceModel model, models)
            {
                DatabaseManager::getInstance().insertDevice(DeviceModel(0, model.getName(), model.getAddress(),
                                                                        model.getPort(), model.getUsername(),
                                                                        model.getPassword(), model.getDescription(),
                                                                        "", model.getShadow(), 0, "", model.getPreviewChannel()));
            }

            loadDevice();

            EventManager::getInstance().fireRefreshLibraryEvent(RefreshLibraryEvent());
        }
    }
}

void SettingsDialog::showImportTriCasterDeviceDialog()
{
    QString path("./TriCaster.xml");

    QFile file(path);
    if (!file.exists())
        path = QFileDialog::getOpenFileName(this, "Import TriCaster Mixers", "", "TriCaster (*.xml)");

    if (!path.isEmpty())
    {
        ImportTriCasterDeviceDialog* dialog = new ImportTriCasterDeviceDialog(this);
        dialog->setImportFile(path);
        if (dialog->exec() == QDialog::Accepted)
        {
            QList<TriCasterDeviceModel> models = dialog->getDevice();
            foreach (TriCasterDeviceModel model, models)
            {
                DatabaseManager::getInstance().insertTriCasterDevice(TriCasterDeviceModel(0, model.getName(), model.getAddress(),
                                                                                          model.getPort(), model.getDescription()));
            }

            loadTriCasterDevice();

            EventManager::getInstance().fireRefreshLibraryEvent(RefreshLibraryEvent());
        }
    }
}

void SettingsDialog::showImportAtemDeviceDialog()
{
    QString path("./ATEM.xml");

    QFile file(path);
    if (!file.exists())
        path = QFileDialog::getOpenFileName(this, "Import ATEM Mixers", "", "ATEM (*.xml)");

    if (!path.isEmpty())
    {
        ImportAtemDeviceDialog* dialog = new ImportAtemDeviceDialog(this);
        dialog->setImportFile(path);
        if (dialog->exec() == QDialog::Accepted)
        {
            QList<AtemDeviceModel> models = dialog->getDevice();
            foreach (AtemDeviceModel model, models)
            {
                DatabaseManager::getInstance().insertAtemDevice(AtemDeviceModel(0, model.getName(), model.getAddress(),
                                                                                model.getDescription()));
            }

            loadAtemDevice();

            EventManager::getInstance().fireRefreshLibraryEvent(RefreshLibraryEvent());
        }
    }
}

void SettingsDialog::showAddDeviceDialog()
{
    DeviceDialog* dialog = new DeviceDialog(this);
    if (dialog->exec() == QDialog::Accepted)
    {
        DatabaseManager::getInstance().insertDevice(DeviceModel(0, dialog->getName(), dialog->getAddress(),
                                                                dialog->getPort().toInt(), dialog->getUsername(),
                                                                dialog->getPassword(), dialog->getDescription(),
                                                                "", dialog->getShadow(), 0, "", dialog->getPreviewChannel()));

        loadDevice();

        EventManager::getInstance().fireRefreshLibraryEvent(RefreshLibraryEvent());
    }
}

void SettingsDialog::showAddTriCasterDeviceDialog()
{
    TriCasterDeviceDialog* dialog = new TriCasterDeviceDialog(this);
    if (dialog->exec() == QDialog::Accepted)
    {
        DatabaseManager::getInstance().insertTriCasterDevice(TriCasterDeviceModel(0, dialog->getName(), dialog->getAddress(),
                                                             dialog->getPort().toInt(), dialog->getDescription()));

        loadTriCasterDevice();

        EventManager::getInstance().fireRefreshLibraryEvent(RefreshLibraryEvent());
    }
}

void SettingsDialog::showAddAtemDeviceDialog()
{
    AtemDeviceDialog* dialog = new AtemDeviceDialog(this);
    if (dialog->exec() == QDialog::Accepted)
    {
        DatabaseManager::getInstance().insertAtemDevice(AtemDeviceModel(0, dialog->getName(), dialog->getAddress(),
                                                        dialog->getDescription()));

        loadAtemDevice();

        EventManager::getInstance().fireRefreshLibraryEvent(RefreshLibraryEvent());
    }
}

void SettingsDialog::showAddOscOutputDialog()
{
    OscOutputDialog* dialog = new OscOutputDialog(this);
    if (dialog->exec() == QDialog::Accepted)
    {
        DatabaseManager::getInstance().insertOscOutput(OscOutputModel(0, dialog->getName(), dialog->getAddress(),
                                                       dialog->getPort().toInt(), dialog->getDescription()));

        loadOscOutput();

        EventManager::getInstance().fireOscOutputChangedEvent(OscOutputChangedEvent());
    }
}

void SettingsDialog::removeDevice()
{
    if (this->treeWidgetDevice->selectedItems().count() == 0)
        return;

    DatabaseManager::getInstance().deleteDevice(this->treeWidgetDevice->currentItem()->text(0).toInt());
    delete this->treeWidgetDevice->currentItem();

    loadDevice();

    EventManager::getInstance().fireRefreshLibraryEvent(RefreshLibraryEvent());
}

void SettingsDialog::removeTriCasterDevice()
{
    if (this->treeWidgetTriCasterDevice->selectedItems().count() == 0)
        return;

    DatabaseManager::getInstance().deleteTriCasterDevice(this->treeWidgetTriCasterDevice->currentItem()->text(0).toInt());
    delete this->treeWidgetTriCasterDevice->currentItem();

    loadTriCasterDevice();

    EventManager::getInstance().fireRefreshLibraryEvent(RefreshLibraryEvent());
}

void SettingsDialog::removeAtemDevice()
{
    if (this->treeWidgetAtemDevice->selectedItems().count() == 0)
        return;

    DatabaseManager::getInstance().deleteAtemDevice(this->treeWidgetAtemDevice->currentItem()->text(0).toInt());
    delete this->treeWidgetAtemDevice->currentItem();

    loadAtemDevice();

    EventManager::getInstance().fireRefreshLibraryEvent(RefreshLibraryEvent());
}

void SettingsDialog::removeOscOutput()
{
    if (this->treeWidgetOscOutput->selectedItems().count() == 0)
        return;

    DatabaseManager::getInstance().deleteOscOutput(this->treeWidgetOscOutput->currentItem()->text(0).toInt());
    delete this->treeWidgetOscOutput->currentItem();

    loadOscOutput();

    EventManager::getInstance().fireOscOutputChangedEvent(OscOutputChangedEvent());
}

void SettingsDialog::deviceItemDoubleClicked(QTreeWidgetItem* current, int index)
{
    DeviceModel model = DatabaseManager::getInstance().getDeviceByAddress(current->text(3));

    DeviceDialog* dialog = new DeviceDialog(this);
    dialog->setDeviceModel(model);
    if (dialog->exec() == QDialog::Accepted)
    {
        DatabaseManager::getInstance().updateDevice(DeviceModel(model.getId(), dialog->getName(), dialog->getAddress(),
                                                                dialog->getPort().toInt(), dialog->getUsername(),
                                                                dialog->getPassword(), dialog->getDescription(),
                                                                model.getVersion(), dialog->getShadow(),
                                                                model.getChannels(), model.getChannelFormats(),
                                                                dialog->getPreviewChannel()));

        loadDevice();

        EventManager::getInstance().fireRefreshLibraryEvent(RefreshLibraryEvent());
    }
}

void SettingsDialog::tricasterDeviceItemDoubleClicked(QTreeWidgetItem* current, int index)
{
    TriCasterDeviceModel model = DatabaseManager::getInstance().getTriCasterDeviceByAddress(current->text(3));

    TriCasterDeviceDialog* dialog = new TriCasterDeviceDialog(this);
    dialog->setDeviceModel(model);
    if (dialog->exec() == QDialog::Accepted)
    {
        DatabaseManager::getInstance().updateTriCasterDevice(TriCasterDeviceModel(model.getId(), dialog->getName(), dialog->getAddress(),
                                                                                  dialog->getPort().toInt(), dialog->getDescription()));

        loadTriCasterDevice();

        EventManager::getInstance().fireRefreshLibraryEvent(RefreshLibraryEvent());
    }
}

void SettingsDialog::atemDeviceItemDoubleClicked(QTreeWidgetItem* current, int index)
{
    AtemDeviceModel model = DatabaseManager::getInstance().getAtemDeviceByAddress(current->text(3));

    AtemDeviceDialog* dialog = new AtemDeviceDialog(this);
    dialog->setDeviceModel(model);
    if (dialog->exec() == QDialog::Accepted)
    {
        DatabaseManager::getInstance().updateAtemDevice(AtemDeviceModel(model.getId(), dialog->getName(), dialog->getAddress(),
                                                                        dialog->getDescription()));

        loadAtemDevice();

        EventManager::getInstance().fireRefreshLibraryEvent(RefreshLibraryEvent());
    }
}

void SettingsDialog::oscOutputItemDoubleClicked(QTreeWidgetItem* current, int index)
{
    OscOutputModel model = DatabaseManager::getInstance().getOscOutputByAddress(current->text(3));

    OscOutputDialog* dialog = new OscOutputDialog(this);
    dialog->setDeviceModel(model);
    if (dialog->exec() == QDialog::Accepted)
    {
        DatabaseManager::getInstance().updateOscOutput(OscOutputModel(model.getId(), dialog->getName(), dialog->getAddress(),
                                                                      dialog->getPort().toInt(), dialog->getDescription()));

        loadOscOutput();

        EventManager::getInstance().fireOscOutputChangedEvent(OscOutputChangedEvent());
    }
}

void SettingsDialog::startFullscreenChanged(int state)
{
    QString isFullscreen = (state == Qt::Checked) ? "true" : "false";
    DatabaseManager::getInstance().updateConfiguration(ConfigurationModel(0, "StartFullscreen", isFullscreen));
}

void SettingsDialog::fontSizeChanged(int size)
{
    qApp->setStyleSheet(this->stylesheet + QString("QWidget { font-size: %1px; }").arg(size));
    DatabaseManager::getInstance().updateConfiguration(ConfigurationModel(0, "FontSize", QString("%1").arg(size)));
}

void SettingsDialog::autoSynchronizeChanged(int state)
{
    QString isAutoSynchronize = (state == Qt::Checked) ? "true" : "false";
    DatabaseManager::getInstance().updateConfiguration(ConfigurationModel(0, "AutoRefreshLibrary", isAutoSynchronize));

    this->labelInterval->setEnabled((isAutoSynchronize == "true") ? true : false);
    this->spinBoxRefreshInterval->setEnabled((isAutoSynchronize == "true") ? true : false);
    this->labelSeconds->setEnabled((isAutoSynchronize == "true") ? true : false);

    EventManager::getInstance().fireAutoRefreshLibraryEvent(AutoRefreshLibraryEvent((isAutoSynchronize == "true") ? true : false,
                                                                                    this->spinBoxRefreshInterval->value() * 1000));
}

void SettingsDialog::showThumbnailTooltipChanged(int state)
{
    QString showThumbnailTooltip = (state == Qt::Checked) ? "true" : "false";
    DatabaseManager::getInstance().updateConfiguration(ConfigurationModel(0, "ShowThumbnailTooltip", showThumbnailTooltip));
}

void SettingsDialog::reverseOscTimeChanged(int state)
{
    QString reverseOscTime = (state == Qt::Checked) ? "true" : "false";
    DatabaseManager::getInstance().updateConfiguration(ConfigurationModel(0, "ReverseOscTime", reverseOscTime));
}

void SettingsDialog::enableOscInputChanged(int state)
{
    QString enableOscInput = (state == Qt::Checked) ? "true" : "false";
    DatabaseManager::getInstance().updateConfiguration(ConfigurationModel(0, "EnableOscInput", enableOscInput));

    this->labelOscPort->setEnabled((state == Qt::Checked) ? true : false);
    this->lineEditOscInputPort->setEnabled((state == Qt::Checked) ? true : false);
}

void SettingsDialog::disableInAndOutPointsChanged(int state)
{
    QString disableInAndOutPoints = (state == Qt::Checked) ? "true" : "false";
    DatabaseManager::getInstance().updateConfiguration(ConfigurationModel(0, "DisableInAndOutPoints", disableInAndOutPoints));
}

void SettingsDialog::synchronizeIntervalChanged(int interval)
{
    DatabaseManager::getInstance().updateConfiguration(ConfigurationModel(0, "RefreshLibraryInterval", QString("%1").arg(interval)));

    EventManager::getInstance().fireAutoRefreshLibraryEvent(AutoRefreshLibraryEvent(this->checkBoxAutoRefresh->checkState(), interval * 1000));
}

void SettingsDialog::updateGpi(int gpi, const QComboBox* voltage, const QComboBox* action)
{
    if (!isVisible()) // During construction of dialog we don't want to rewrite
        return;       // values in the database that are already there.

    bool risingEdge = voltage->currentIndex() == 0;
    Playout::PlayoutType::Type playoutType = Playout::enumConstants().at(action->currentIndex());

    qDebug() << "GPI " << gpi
             << " changed -- rising edge: " << risingEdge
             << " action: " << Playout::toString(playoutType);

    DatabaseManager::getInstance().updateGpiPort(GpiPortModel(gpi, risingEdge, playoutType));

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

void SettingsDialog::updateGpo(int gpo, const QComboBox* voltage, const QSpinBox* pulseLength)
{
    if (!isVisible()) // During construction of dialog we don't want to rewrite
        return;       // values in the database that are already there.

    bool risingEdge = voltage->currentIndex() == 0;
    int pulseLengthMillis = pulseLength->value();

    qDebug() << "GPO " << gpo
             << " changed -- rising edge: " << risingEdge
             << " pulse length: " << pulseLengthMillis << "ms";

    DatabaseManager::getInstance().updateGpoPort(GpoPortModel(gpo, risingEdge, pulseLengthMillis));

    GpiManager::getInstance().getGpiDevice()->setupGpoPort(gpo, pulseLengthMillis, risingEdge);
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

    QString serialPort = this->lineEditSerialPort->text().trimmed();
    int baudRate = this->comboBoxGpiBaudRate->currentText().toInt();

    qDebug() << "GPO Device changed -- Serial port: "
             << serialPort << " Baud rate: " << baudRate;

    DatabaseManager::getInstance().updateConfiguration(ConfigurationModel(0, "GpiSerialPort", serialPort));
    DatabaseManager::getInstance().updateConfiguration(ConfigurationModel(0, "GpiBaudRate", QString("%1").arg(baudRate)));

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

void SettingsDialog::oscPortChanged()
{
    QString oscPort = this->lineEditOscInputPort->text().trimmed();
    if (oscPort.isEmpty())
        oscPort = Osc::DEFAULT_PORT;

    qDebug() << "OSC port: " << oscPort;

    DatabaseManager::getInstance().updateConfiguration(ConfigurationModel(0, "OscPort", oscPort));
}

void SettingsDialog::delayTypeChanged(QString delayType)
{
    DatabaseManager::getInstance().updateConfiguration(ConfigurationModel(0, "DelayType", this->comboBoxDelayType->currentText()));
}

void SettingsDialog::tricasterProductChanged(QString product)
{
    DatabaseManager::getInstance().updateConfiguration(ConfigurationModel(0, "TriCasterProduct", this->comboBoxTriCasterProduct->currentText()));
}

void SettingsDialog::themeChanged(QString theme)
{
    DatabaseManager::getInstance().updateConfiguration(ConfigurationModel(0, "Theme", this->comboBoxTheme->currentText()));
}

void SettingsDialog::rundownRepositoryChanged()
{
    DatabaseManager::getInstance().updateConfiguration(ConfigurationModel(0, "RundownRepository", this->lineEditRundownRepository->text()));
}

void SettingsDialog::previewOnAutoStepChanged(int state)
{
    QString previewOnAutoStep = (state == Qt::Checked) ? "true" : "false";
    DatabaseManager::getInstance().updateConfiguration(ConfigurationModel(0, "PreviewOnAutoStep", previewOnAutoStep));
}

void SettingsDialog::clearDelayedCommandsOnAutoStepChanged(int state)
{
    QString clearDelayedCommandsOnAutoStep = (state == Qt::Checked) ? "true" : "false";
    DatabaseManager::getInstance().updateConfiguration(ConfigurationModel(0, "ClearDelayedCommandsOnAutoStep", clearDelayedCommandsOnAutoStep));
}

void SettingsDialog::storeThumbnailsInDatabaseChanged(int state)
{
    QString storeThumbnailsInDatabase = (state == Qt::Checked) ? "true" : "false";
    DatabaseManager::getInstance().updateConfiguration(ConfigurationModel(0, "StoreThumbnailsInDatabase", storeThumbnailsInDatabase));
}

void SettingsDialog::deleteThumbnails()
{
    EventManager::getInstance().fireStatusbarEvent(StatusbarEvent("Deleting thumbnails..."));

    DatabaseManager::getInstance().deleteThumbnails();

    EventManager::getInstance().fireStatusbarEvent(StatusbarEvent(""));

    QMessageBox box(this);
    box.setWindowTitle("Database");
    box.setText(QString("Successfully deleted all thumbnails from the database."));
    box.setIconPixmap(QPixmap(":/Graphics/Images/Information.png"));
    box.setStandardButtons(QMessageBox::Ok);
    box.buttons().at(0)->setFocusPolicy(Qt::NoFocus);
    box.exec();
}

void SettingsDialog::markUsedItemsChanged(int state)
{
    QString markUsedItems = (state == Qt::Checked) ? "true" : "false";
    DatabaseManager::getInstance().updateConfiguration(ConfigurationModel(0, "MarkUsedItems", markUsedItems));
}

void SettingsDialog::showAudioLevelsChanged(int state)
{
    QString showAudioLevelsPanel = (state == Qt::Checked) ? "true" : "false";
    DatabaseManager::getInstance().updateConfiguration(ConfigurationModel(0, "ShowAudioLevelsPanel", showAudioLevelsPanel));
}

void SettingsDialog::showPreviewChanged(int state)
{
    QString showPreviewPanel = (state == Qt::Checked) ? "true" : "false";
    DatabaseManager::getInstance().updateConfiguration(ConfigurationModel(0, "ShowPreviewPanel", showPreviewPanel));
}

void SettingsDialog::showLiveChanged(int state)
{
    QString showLivePanel = (state == Qt::Checked) ? "true" : "false";
    DatabaseManager::getInstance().updateConfiguration(ConfigurationModel(0, "ShowLivePanel", showLivePanel));
}
