#pragma once

#include "../../Shared.h"
#include "ui_InspectorAtemMacroWidget.h"

#include "AtemDevice.h"

#include "Commands/Atem/AtemMacroCommand.h"
#include "Events/Atem/AtemDeviceChangedEvent.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include "qatemtypes.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtWidgets/QWidget>

class WIDGETS_EXPORT InspectorAtemMacroWidget : public QWidget, Ui::InspectorAtemMacroWidget
{
    Q_OBJECT

    public:
        explicit InspectorAtemMacroWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        AtemMacroCommand* command;
        QVector<QAtem::MacroInfo> macros;

        void checkEmptyMacro();
        void loadAtemMacro();
        void blockAllSignals(bool block);

        Q_SLOT void macroChanged(int);
        Q_SLOT void triggerOnNextChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
        Q_SLOT void atemDeviceChanged(const AtemDeviceChangedEvent&);
};
