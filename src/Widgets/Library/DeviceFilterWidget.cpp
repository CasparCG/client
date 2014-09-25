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

#include <QtGui/QStandardItemModel>

DeviceFilterWidget::DeviceFilterWidget(QWidget* parent)
    : QWidget(parent), sendEvents(false)
{
    setupUi(this);

    this->lineEditDeviceFilter->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    this->comboBoxDeviceFilter->addItem("All");

    QObject::connect(this->comboBoxDeviceFilter->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(dataChanged(const QModelIndex&, const QModelIndex&)));
    dynamic_cast<QStandardItemModel*>(this->comboBoxDeviceFilter->model())->item(0, 0)->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    dynamic_cast<QStandardItemModel*>(this->comboBoxDeviceFilter->model())->item(0, 0)->setData(Qt::Checked, Qt::CheckStateRole);

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

    if (this->comboBoxDeviceFilter->count() == 0)
        return devices;

    if (dynamic_cast<QStandardItemModel*>(this->comboBoxDeviceFilter->model())->item(0, 0)->checkState() == Qt::Checked)
        return devices;

    for (int i = 1; i < this->comboBoxDeviceFilter->count(); i++)
    {
        if (dynamic_cast<QStandardItemModel*>(this->comboBoxDeviceFilter->model())->item(i, 0)->checkState() == Qt::Checked)
        {
            QString name = dynamic_cast<QStandardItemModel*>(this->comboBoxDeviceFilter->model())->item(i, 0)->text();
            devices.push_back(DeviceManager::getInstance().getDeviceModelByName(name)->getAddress());
        }
    }

    return devices;
}

void DeviceFilterWidget::deviceRemoved()
{
    blockAllSignals(true);

    this->lineEditDeviceFilter->clear();
    this->comboBoxDeviceFilter->clear();

    blockAllSignals(false);

    this->comboBoxDeviceFilter->addItem("All");
    QObject::connect(this->comboBoxDeviceFilter->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(dataChanged(const QModelIndex&, const QModelIndex&)));
    dynamic_cast<QStandardItemModel*>(this->comboBoxDeviceFilter->model())->item(0, 0)->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    dynamic_cast<QStandardItemModel*>(this->comboBoxDeviceFilter->model())->item(0, 0)->setData(Qt::Checked, Qt::CheckStateRole);

    blockAllSignals(true);

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        this->comboBoxDeviceFilter->addItem(QString("%1").arg(model.getName()));

        dynamic_cast<QStandardItemModel*>(this->comboBoxDeviceFilter->model())->item(this->comboBoxDeviceFilter->count() - 1, 0)->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        dynamic_cast<QStandardItemModel*>(this->comboBoxDeviceFilter->model())->item(this->comboBoxDeviceFilter->count() - 1, 0)->setData(Qt::Unchecked, Qt::CheckStateRole);
    }

    blockAllSignals(false);
}

void DeviceFilterWidget::deviceAdded(CasparDevice& device)
{
    this->sendEvents = false;

    const QSharedPointer<DeviceModel> model = DeviceManager::getInstance().getDeviceModelByAddress(device.getAddress());
    if (model == NULL || model->getShadow() == "Yes")
        return; // Don't add shadow systems.

    this->comboBoxDeviceFilter->addItem(QString("%1").arg(model->getName()));

    dynamic_cast<QStandardItemModel*>(this->comboBoxDeviceFilter->model())->item(this->comboBoxDeviceFilter->count() - 1, 0)->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    dynamic_cast<QStandardItemModel*>(this->comboBoxDeviceFilter->model())->item(this->comboBoxDeviceFilter->count() - 1, 0)->setData(Qt::Unchecked, Qt::CheckStateRole);

    this->sendEvents = true;
}

void DeviceFilterWidget::dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    blockAllSignals(true);

    // "All" item is selected, deselect others.
    if (topLeft.row() == 0 && dynamic_cast<QStandardItemModel*>(this->comboBoxDeviceFilter->model())->item(topLeft.row(), 0)->checkState() == Qt::Checked)
    {
        for (int i = 1; i < this->comboBoxDeviceFilter->count(); i++)
            dynamic_cast<QStandardItemModel*>(this->comboBoxDeviceFilter->model())->item(i, 0)->setData(Qt::Unchecked, Qt::CheckStateRole);
    }
    else
    {
        // Deselect the "All" item.
        dynamic_cast<QStandardItemModel*>(this->comboBoxDeviceFilter->model())->item(0, 0)->setData(Qt::Unchecked, Qt::CheckStateRole);
    }

    bool found = false;
    for (int i = 0; i < this->comboBoxDeviceFilter->count(); i++)
    {
        if (dynamic_cast<QStandardItemModel*>(this->comboBoxDeviceFilter->model())->item(i, 0)->checkState() == Qt::Checked)
            found = true;

        if (found)
            break;
    }

    if (!found)
        dynamic_cast<QStandardItemModel*>(this->comboBoxDeviceFilter->model())->item(0, 0)->setData(Qt::Checked, Qt::CheckStateRole);

    QString devices;
    for (int i = 0; i < this->comboBoxDeviceFilter->count(); i++)
    {
        if (dynamic_cast<QStandardItemModel*>(this->comboBoxDeviceFilter->model())->item(i, 0)->checkState() == Qt::Checked)
            devices += QString("%1,").arg(dynamic_cast<QStandardItemModel*>(this->comboBoxDeviceFilter->model())->item(i, 0)->text().trimmed());
    }

    if (devices.endsWith(','))
        devices.remove(devices.length() - 1, 1);

    this->lineEditDeviceFilter->setText(devices);

    if (this->sendEvents)
    {
        EventManager::getInstance().fireMediaChangedEvent(MediaChangedEvent());
        EventManager::getInstance().fireTemplateChangedEvent(TemplateChangedEvent());
        EventManager::getInstance().fireDataChangedEvent(DataChangedEvent());
        EventManager::getInstance().firePresetChangedEvent(PresetChangedEvent());
    }

    blockAllSignals(false);
}
