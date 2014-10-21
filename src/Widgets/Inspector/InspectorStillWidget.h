#pragma once

#include "../Shared.h"
#include "ui_InspectorStillWidget.h"

#include "Commands/StillCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>
#include <QtCore/QString>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorStillWidget : public QWidget, Ui::InspectorStillWidget
{
    Q_OBJECT

    public:
        explicit InspectorStillWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        StillCommand* command;

        void loadDirection();
        void loadTransition();
        void loadTween();
        void blockAllSignals(bool block);

        Q_SLOT void transitionChanged(QString);
        Q_SLOT void transitionDurationChanged(int);
        Q_SLOT void directionChanged(QString);
        Q_SLOT void tweenChanged(QString);
        Q_SLOT void useAutoChanged(int);
        Q_SLOT void triggerOnNextChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
