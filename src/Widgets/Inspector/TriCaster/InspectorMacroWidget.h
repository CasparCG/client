#pragma once

#include "../../Shared.h"
#include "ui_InspectorMacroWidget.h"

#include "Commands/TriCaster/MacroCommand.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorMacroWidget : public QWidget, Ui::InspectorMacroWidget
{
    Q_OBJECT

    public:
        explicit InspectorMacroWidget(QWidget* parent = 0);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        LibraryModel* model;
        MacroCommand* command;

        void blockAllSignals(bool block);

        Q_SLOT void nameChanged(QString);
};
