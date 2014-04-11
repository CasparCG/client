#include "ImportAtemDeviceDialog.h"

#include <iostream>

#include <QtCore/QTextCodec>
#include <QtCore/QTextStream>

#include <QtGui/QAbstractButton>
#include <QtGui/QCloseEvent>
#include <QtGui/QMessageBox>

ImportAtemDeviceDialog::ImportAtemDeviceDialog(QWidget* parent)
    : QDialog(parent)
{
    setupUi(this);

    this->treeWidgetAtemDevice->headerItem()->setText(0, "");
    this->treeWidgetAtemDevice->setColumnWidth(0, 25);
}

void ImportAtemDeviceDialog::setImportFile(const QString& path)
{
    QFile file(path);
    if (file.open(QFile::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream.setCodec(QTextCodec::codecForName("UTF-8"));

        std::wstringstream wstringstream;
        wstringstream << stream.readAll().toStdWString();

        boost::property_tree::wptree pt;
        boost::property_tree::xml_parser::read_xml(wstringstream, pt);

        BOOST_FOREACH(boost::property_tree::wptree::value_type& parentValue, pt.get_child(L"mixers"))
        {
            this->models.push_back(parseData(parentValue.second));
        }

        foreach (AtemDeviceModel model, this->models)
        {
            QTreeWidgetItem* treeItem = new QTreeWidgetItem(this->treeWidgetAtemDevice);
            treeItem->setIcon(0, QIcon(":/Graphics/Images/ServerSmall.png"));
            treeItem->setText(1, model.getName());
            treeItem->setText(2, model.getDescription());
        }
    }
}

AtemDeviceModel ImportAtemDeviceDialog::parseData(boost::property_tree::wptree& pt)
{
    QString name = QString::fromStdWString(pt.get(L"name", L""));
    QString address = QString::fromStdWString(pt.get(L"address", L""));
    QString description = QString::fromStdWString(pt.get(L"description", L""));

    return AtemDeviceModel(0, name, address, description);
}

const QList<AtemDeviceModel> ImportAtemDeviceDialog::getDevice() const
{
    QList<AtemDeviceModel> models;
    for (int i = 0; i < this->treeWidgetAtemDevice->invisibleRootItem()->childCount(); i++)
    {
        QTreeWidgetItem* item = this->treeWidgetAtemDevice->invisibleRootItem()->child(i);
        if (item->isSelected())
            models.push_back(this->models.at(i));
    }

    return models;
}

void ImportAtemDeviceDialog::accept()
{
    QDialog::accept();
}
