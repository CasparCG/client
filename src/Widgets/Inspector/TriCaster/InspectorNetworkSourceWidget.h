#pragma once

#include "../../Shared.h"
#include "ui_InspectorNetworkSourceWidget.h"

#include "Commands/TriCaster/NetworkSourceCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorNetworkSourceWidget : public QWidget, Ui::InspectorNetworkSourceWidget
{
    Q_OBJECT

    public:
        explicit InspectorNetworkSourceWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        NetworkSourceCommand* command;

        void loadTriCasterNetworkTarget();
        void blockAllSignals(bool block);

        Q_SLOT void targetChanged(int);
        Q_SLOT void sourceChanged(QString);
        Q_SLOT void triggerOnNextChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
