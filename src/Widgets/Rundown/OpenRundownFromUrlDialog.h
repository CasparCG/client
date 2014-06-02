#pragma once

#include "../Shared.h"
#include "ui_OpenRundownFromUrlDialog.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QEvent>
#include <QtCore/QList>

#include <QtGui/QDialog>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QWidget>

#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>

class WIDGETS_EXPORT OpenRundownFromUrlDialog : public QDialog, Ui::OpenRundownFromUrlDialog
{
    Q_OBJECT

    public:
        explicit OpenRundownFromUrlDialog(QWidget* parent = 0);

        QString getPath() const;

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);
        void accept();

    private:
        QString repositoryUrl;

        QNetworkAccessManager* networkManager;

        Q_SLOT void finished(QNetworkReply*);
        Q_SLOT void itemDoubleClicked(QTreeWidgetItem*, int);
};
