#pragma once

#include "../Shared.h"
#include "ui_InspectorHttpGetWidget.h"

#include "Commands/HttpGetCommand.h"
#include "Events/Inspector/ShowAddHttpGetDataDialogEvent.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QTreeWidgetItem>
#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorHttpGetWidget : public QWidget, Ui::InspectorHttpGetWidget
{
    Q_OBJECT

    public:
        explicit InspectorHttpGetWidget(QWidget* parent = 0);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        int fieldCounter;
        LibraryModel* model;
        HttpGetCommand* command;

        void checkEmptyUrl();
        void updateHttpDataModels();
        void blockAllSignals(bool block);

        Q_SLOT bool addRow();
        Q_SLOT bool editRow();
        Q_SLOT bool removeRow();
        Q_SLOT bool duplicateSelectedItem();
        Q_SLOT bool copySelectedItem();
        Q_SLOT bool pasteSelectedItem();
        Q_SLOT void urlChanged(QString);
        Q_SLOT void currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*);
        Q_SLOT void itemDoubleClicked(QTreeWidgetItem*, int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
        Q_SLOT void showAddHttpGetDataDialog(const ShowAddHttpGetDataDialogEvent&);
        Q_SLOT void triggerOnNextChanged(int);
};
