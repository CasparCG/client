#pragma once

#include "../../Shared.h"
#include "ui_InspectorMacroWidget.h"

#include "Commands/TriCaster/MacroCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorMacroWidget : public QWidget, Ui::InspectorMacroWidget
{
    Q_OBJECT

    public:
        explicit InspectorMacroWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        MacroCommand* command;

        void blockAllSignals(bool block);

        Q_SLOT void macroChanged(QString);
        Q_SLOT void triggerOnNextChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
