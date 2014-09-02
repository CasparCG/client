#pragma once

#include "../../Shared.h"
#include "ui_InspectorAtemKeyerStateWidget.h"

#include "Commands/Atem/AtemKeyerStateCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

class WIDGETS_EXPORT InspectorAtemKeyerStateWidget : public QWidget, Ui::InspectorAtemKeyerStateWidget
{
    Q_OBJECT

    public:
        explicit InspectorAtemKeyerStateWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        AtemKeyerStateCommand* command;

        void loadAtemKeyer();
        void blockAllSignals(bool block);

        Q_SLOT void keyerChanged(int);
        Q_SLOT void stateChanged(int);
        Q_SLOT void triggerOnNextChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
