#pragma once

#include "../../Shared.h"
#include "ui_InspectorTakeWidget.h"

#include "Commands/TriCaster/TakeCommand.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorTakeWidget : public QWidget, Ui::InspectorTakeWidget
{
    Q_OBJECT

    public:
        explicit InspectorTakeWidget(QWidget* parent = 0);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        LibraryModel* model;
        TakeCommand* command;

        void checkEmptyStep();
        void loadTriCasterStep();
        void blockAllSignals(bool block);

        Q_SLOT void stepChanged(int);
};
