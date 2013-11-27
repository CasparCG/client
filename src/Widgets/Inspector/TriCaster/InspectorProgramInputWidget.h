#pragma once

#include "../../Shared.h"
#include "ui_InspectorProgramInputWidget.h"

#include "Commands/TriCaster/ProgramInputCommand.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorProgramInputWidget : public QWidget, Ui::InspectorProgramInputWidget
{
    Q_OBJECT

    public:
        explicit InspectorProgramInputWidget(QWidget* parent = 0);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        LibraryModel* model;
        ProgramInputCommand* command;

        void loadTriCasterInput();
        void blockAllSignals(bool block);

        Q_SLOT void inputChanged(int);
};
