#pragma once

#include "../Shared.h"
#include "ui_InspectorTemplateWidget.h"

#include "Commands/TemplateCommand.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorTemplateWidget : public QWidget, Ui::InspectorTemplateWidget
{
    Q_OBJECT

    public:
        explicit InspectorTemplateWidget(QWidget* parent = 0);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        bool preview;
        int rowIndex;
        LibraryModel* model;
        TemplateCommand* command;

        void updateDataTemplateModels();

        Q_SLOT void addRow();
        Q_SLOT void removeRow();
        Q_SLOT void invokeChanged(QString);
        Q_SLOT void flashlayerChanged(int);
        Q_SLOT void useStoredDataChanged(int);
        Q_SLOT void itemChanged(QTableWidgetItem*);
        Q_SLOT void resetFlashlayer(QString);
        Q_SLOT void resetUseStoredData(QString);
        Q_SLOT void resetInvoke(QString);
};
