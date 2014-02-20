#pragma once

#include "../../Shared.h"
#include "ui_InspectorInputWidget.h"

#include "Commands/TriCaster/InputCommand.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorInputWidget : public QWidget, Ui::InspectorInputWidget
{
    Q_OBJECT

    public:
        explicit InspectorInputWidget(QWidget* parent = 0);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        LibraryModel* model;
        InputCommand* command;

        void loadTriCasterSwitcher();
        void loadTriCasterInput();
        void blockAllSignals(bool block);

        Q_SLOT void switcherChanged(int);
        Q_SLOT void inputChanged(int);
        Q_SLOT void triggerOnNextChanged(int);
};
