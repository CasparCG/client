#pragma once

#include "../Shared.h"
#include "ui_InspectorDeckLinkInputWidget.h"

#include "Commands/DeckLinkInputCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>
#include <QtCore/QString>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorDeckLinkInputWidget : public QWidget, Ui::InspectorDeckLinkInputWidget
{
    Q_OBJECT

    public:
        explicit InspectorDeckLinkInputWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        DeckLinkInputCommand* command;

        void loadDirection();
        void loadFormat();
        void loadTransition();
        void loadTween();
        void blockAllSignals(bool block);

        Q_SLOT void transitionChanged(QString);
        Q_SLOT void transitionDurationChanged(int);
        Q_SLOT void directionChanged(QString);
        Q_SLOT void tweenChanged(QString);
        Q_SLOT void deviceChanged(int);
        Q_SLOT void formatChanged(QString);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
