#pragma once

#include "../Shared.h"
#include "ui_InspectorBlendModeWidget.h"

#include "Commands/BlendModeCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorBlendModeWidget : public QWidget, Ui::InspectorBlendModeWidget
{
    Q_OBJECT

    public:
        explicit InspectorBlendModeWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        BlendModeCommand* command;

        void loadBlendMode();
        void blockAllSignals(bool block);

        Q_SLOT void blendModeChanged(QString);
        Q_SLOT void resetBlendMode(QString);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
