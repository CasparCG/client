#pragma once

#include "../Shared.h"
#include "ui_InspectorHttpPostWidget.h"

#include "Commands/HttpPostCommand.h"
#include "Events/Inspector/ShowAddHttpPostDataDialogEvent.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QTreeWidgetItem>
#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorHttpPostWidget : public QWidget, Ui::InspectorHttpPostWidget
{
    Q_OBJECT

    public:
        explicit InspectorHttpPostWidget(QWidget* parent = 0);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        int fieldCounter;
        LibraryModel* model;
        HttpPostCommand* command;

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
        Q_SLOT void showAddHttpPostDataDialog(const ShowAddHttpPostDataDialogEvent&);
        Q_SLOT void triggerOnNextChanged(int);
};
