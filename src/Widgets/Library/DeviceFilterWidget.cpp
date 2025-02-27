#include "DeviceFilterWidget.h"

#include "Global.h"

#include "DeviceManager.h"
#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/DataChangedEvent.h"
#include "Events/MediaChangedEvent.h"
#include "Events/PresetChangedEvent.h"
#include "Events/Inspector/TemplateChangedEvent.h"

#include <QtCore/QEvent>
#include <QtCore/QStringList>

DeviceFilterWidget::DeviceFilterWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUi(this);

    this->lineEditDeviceFilter->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    QListWidgetItem* item = new QListWidgetItem("All");
    item->setCheckState(Qt::Checked);
    this->listWidget.addItem(item);
    this->lineEditDeviceFilter->setText(item->text());

    this->comboBoxDeviceFilter->setModel(this->listWidget.model());
    this->comboBoxDeviceFilter->setView(&this->listWidget);


    QObject::connect(&this->listWidget, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(itemPressed(QListWidgetItem*)));
    QObject::connect(&DeviceManager::getInstance(), SIGNAL(deviceRemoved()), this, SLOT(deviceRemoved()));
    QObject::connect(&DeviceManager::getInstance(), SIGNAL(deviceAdded(CasparDevice&)), this, SLOT(deviceAdded(CasparDevice&)));

    qApp->installEventFilter(this);
}

bool DeviceFilterWidget::eventFilter(QObject* target, QEvent* event)
{
    if (target == this->lineEditDeviceFilter && event->type() == QEvent::MouseButtonPress)
    {
        this->comboBoxDeviceFilter->showPopup();
        return true;
    }

    return QObject::eventFilter(target, event);
}

void DeviceFilterWidget::blockAllSignals(bool block)
{
    this->comboBoxDeviceFilter->blockSignals(block);
    this->comboBoxDeviceFilter->model()->blockSignals(block);
}

QList<QString> DeviceFilterWidget::getDeviceFilter()
{
    QList<QString> devices;

    if (this->listWidget.count() == 0)
        return devices;

    for (int i = 1; i < this->listWidget.count(); i++)
    {
        if (this->listWidget.item(i)->checkState() == Qt::Checked)
        {
            QString name = this->listWidget.item(i)->text();
            devices.push_back(DeviceManager::getInstance().getDeviceModelByName(name)->getAddress());
        }
    }

    return devices;
}

void DeviceFilterWidget::itemPressed(QListWidgetItem* item)
{
    // The 'All' item is special.
    if (this->listWidget.item(0) == item)
    {
        if (this->listWidget.count() > 1)
        {
            item->setCheckState(Qt::Checked);
            for (int i = 1; i < this->listWidget.count(); i++)
                this->listWidget.item(i)->setCheckState(Qt::Unchecked);
        }

        this->comboBoxDeviceFilter->setStyleSheet("");
    }
    else
    {
        // Deselect 'All' if something else is selected.
        this->listWidget.item(0)->setCheckState(Qt::Unchecked);

        if (item->checkState() == Qt::Checked)
            item->setCheckState(Qt::Unchecked);
        else
            item->setCheckState(Qt::Checked);

        this->comboBoxDeviceFilter->setStyleSheet("border-color: darkorange;");
    }

    // Select 'All' if no one is selected.
    bool found = false;
    for (int i = 1; i < this->listWidget.count(); i++)
    {
        if (this->listWidget.item(i)->checkState() == Qt::Checked)
            found = true;
    }

    if (!found)
    {
        this->listWidget.item(0)->setCheckState(Qt::Checked);
        this->comboBoxDeviceFilter->setStyleSheet("");
    }

    QString devices;
    for (int i = 0; i < this->listWidget.count(); i++)
    {
        if (this->listWidget.item(i)->checkState() == Qt::Checked)
            devices += this->listWidget.item(i)->text() + ',';
    }

    if (devices.endsWith(','))
        devices.remove(devices.length() - 1, 1);

    this->lineEditDeviceFilter->setText(devices);

    EventManager::getInstance().fireMediaChangedEvent(MediaChangedEvent());
    EventManager::getInstance().fireTemplateChangedEvent(TemplateChangedEvent());
    EventManager::getInstance().fireDataChangedEvent(DataChangedEvent());
    EventManager::getInstance().firePresetChangedEvent(PresetChangedEvent());
}

void DeviceFilterWidget::deviceRemoved()
{
    blockAllSignals(true);

    this->listWidget.clear();
    this->lineEditDeviceFilter->clear();

    QListWidgetItem* item = new QListWidgetItem("All");
    item->setCheckState(Qt::Checked);
    this->listWidget.addItem(item);

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        QListWidgetItem* item = new QListWidgetItem(QString("%1").arg(model.getName()));
        item->setCheckState(Qt::Checked);
        this->listWidget.addItem(item);
    }

    blockAllSignals(false);
}

void DeviceFilterWidget::deviceAdded(CasparDevice& device)
{
    const QSharedPointer<DeviceModel> deviceModel = DeviceManager::getInstance().getDeviceModelByAddress(device.getAddress());
    if (deviceModel == NULL || deviceModel->getShadow() == "Yes")
        return; // Don't add shadow systems.

    QListWidgetItem* item = new QListWidgetItem(QString("%1").arg(deviceModel->getName()));
    item->setCheckState(Qt::Unchecked);
    this->listWidget.addItem(item);
}
