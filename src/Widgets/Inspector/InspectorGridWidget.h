#pragma once

#include "../Shared.h"
#include "ui_InspectorGridWidget.h"

#include "Commands/GridCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorGridWidget : public QWidget, Ui::InspectorGridWidget
{
    Q_OBJECT

    public:
        explicit InspectorGridWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        GridCommand* command;

        void loadTween();
        void blockAllSignals(bool block);

        Q_SLOT void gridChanged(int);
        Q_SLOT void transitionDurationChanged(int);
        Q_SLOT void tweenChanged(QString);
        Q_SLOT void deferChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
