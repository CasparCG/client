#pragma once

#include "../Shared.h"
#include "ui_InspectorTemplateWidget.h"

#include "Commands/TemplateCommand.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QTreeWidgetItem>
#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorTemplateWidget : public QWidget, Ui::InspectorTemplateWidget
{
    Q_OBJECT

    public:
        explicit InspectorTemplateWidget(QWidget* parent = 0);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        int fieldCounter;
        LibraryModel* model;
        TemplateCommand* command;

        void updateDataTemplateModels();
        void blockAllSignals(bool block);

        Q_SLOT bool addRow();
        Q_SLOT bool editRow();
        Q_SLOT bool removeRow();
        Q_SLOT bool duplicateSelectedItem();
        Q_SLOT bool copySelectedItem();
        Q_SLOT bool pasteSelectedItem();
        Q_SLOT void invokeChanged(QString);
        Q_SLOT void flashlayerChanged(int);
        Q_SLOT void useStoredDataChanged(int);
        Q_SLOT void useUppercaseDataChanged(int);
        Q_SLOT void currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*);
        Q_SLOT void itemDoubleClicked(QTreeWidgetItem*, int);
};
