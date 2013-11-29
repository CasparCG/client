#pragma once

#include "../../Shared.h"
#include "ui_InspectorAutoWidget.h"

#include "Commands/TriCaster/AutoCommand.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorAutoWidget : public QWidget, Ui::InspectorAutoWidget
{
    Q_OBJECT

    public:
        explicit InspectorAutoWidget(QWidget* parent = 0);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        LibraryModel* model;
        AutoCommand* command;

        void loadTriCasterStep();
        void blockAllSignals(bool block);

        Q_SLOT void stepChanged(int);
};
