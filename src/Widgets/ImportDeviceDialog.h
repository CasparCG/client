#pragma once

#include "Shared.h"
#include "ui_ImportDeviceDialog.h"

#include "Models/DeviceModel.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QList>

#include <QtGui/QDialog>
#include <QtGui/QWidget>

class WIDGETS_EXPORT ImportDeviceDialog : public QDialog, Ui::ImportDeviceDialog
{
    Q_OBJECT

    public:
        explicit ImportDeviceDialog(QWidget* parent = 0);

        void setImportFile(const QString& path);

        const QList<DeviceModel> getDevice() const;

    protected:
        void accept();

    private:
        QList<DeviceModel> models;

        DeviceModel parseData(boost::property_tree::wptree& pt);
};
