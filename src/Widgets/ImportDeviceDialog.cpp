#include "ImportDeviceDialog.h"

#include <iostream>

#include <QtCore5Compat/QTextCodec>
#include <QtCore/QTextStream>
#include <QtCore/QFile>

#include <QtGui/QCloseEvent>

#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QMessageBox>

ImportDeviceDialog::ImportDeviceDialog(QWidget* parent)
    : QDialog(parent)
{
    setupUi(this);

    this->treeWidgetDevice->headerItem()->setText(0, "");
    this->treeWidgetDevice->setColumnWidth(0, 25);
}

void ImportDeviceDialog::setImportFile(const QString& path)
{
    QFile file(path);
    if (file.open(QFile::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream.setEncoding(QStringConverter::Utf8);

        std::wstringstream wstringstream;
        wstringstream << stream.readAll().toStdWString();

        boost::property_tree::wptree pt;
        boost::property_tree::xml_parser::read_xml(wstringstream, pt);

        for (boost::property_tree::wptree::value_type &parentValue : pt.get_child(L"servers"))
        {
            this->models.push_back(parseData(parentValue.second));
        }

        foreach (DeviceModel model, this->models)
        {
            QTreeWidgetItem* treeItem = new QTreeWidgetItem(this->treeWidgetDevice);
            treeItem->setIcon(0, QIcon(":/Graphics/Images/ServerSmall.png"));
            treeItem->setText(1, model.getName());
            treeItem->setText(2, model.getDescription());
        }
    }
}

DeviceModel ImportDeviceDialog::parseData(boost::property_tree::wptree& pt)
{
    QString name = QString::fromStdWString(pt.get(L"name", L""));
    QString address = QString::fromStdWString(pt.get(L"address", L""));
    QString port = QString::fromStdWString(pt.get(L"port", L"5250"));
    QString username = QString::fromStdWString(pt.get(L"username", L""));
    QString password = QString::fromStdWString(pt.get(L"password", L""));
    QString description = QString::fromStdWString(pt.get(L"description", L""));
    QString shadow = QString::fromStdWString(pt.get(L"shadow", L"No"));
    QString previewChannel = QString::fromStdWString(pt.get(L"previewchannel", L"0"));
    QString lockedChannel = QString::fromStdWString(pt.get(L"lockedchannel", L"0"));

    return DeviceModel(0, name, address, port.toInt(), username, password, description, "", shadow, 0, "", previewChannel.toInt(), lockedChannel.toInt());
}

const QList<DeviceModel> ImportDeviceDialog::getDevice() const
{
    QList<DeviceModel> models;
    for (int i = 0; i < this->treeWidgetDevice->invisibleRootItem()->childCount(); i++)
    {
        QTreeWidgetItem* item = this->treeWidgetDevice->invisibleRootItem()->child(i);
        if (item->isSelected())
            models.push_back(this->models.at(i));
    }

    return models;
}

void ImportDeviceDialog::accept()
{
    QDialog::accept();
}
