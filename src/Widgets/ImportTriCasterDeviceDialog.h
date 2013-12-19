#pragma once

#include "Shared.h"
#include "ui_ImportTriCasterDeviceDialog.h"

#include "Models/TriCaster/TriCasterDeviceModel.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QList>

#include <QtGui/QDialog>
#include <QtGui/QWidget>

class WIDGETS_EXPORT ImportTriCasterDeviceDialog : public QDialog, Ui::ImportTriCasterDeviceDialog
{
    Q_OBJECT

    public:
        explicit ImportTriCasterDeviceDialog(QWidget* parent = 0);

        void setImportFile(const QString& path);

        const QList<TriCasterDeviceModel> getDevice() const;

    protected:
        void accept();

    private:
        QList<TriCasterDeviceModel> models;

        TriCasterDeviceModel parseData(boost::property_tree::wptree& pt);
};
