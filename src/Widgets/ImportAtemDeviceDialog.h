#pragma once

#include "Shared.h"
#include "ui_ImportAtemDeviceDialog.h"

#include "Models/Atem/AtemDeviceModel.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QList>

#include <QtGui/QDialog>
#include <QtGui/QWidget>

class WIDGETS_EXPORT ImportAtemDeviceDialog : public QDialog, Ui::ImportAtemDeviceDialog
{
    Q_OBJECT

    public:
        explicit ImportAtemDeviceDialog(QWidget* parent = 0);

        void setImportFile(const QString& path);

        const QList<AtemDeviceModel> getDevice() const;

    protected:
        void accept();

    private:
        QList<AtemDeviceModel> models;

        AtemDeviceModel parseData(boost::property_tree::wptree& pt);
};
