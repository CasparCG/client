#pragma once

#include "../Shared.h"
#include "ui_InspectorDeckLinkInputWidget.h"

#include "Commands/DeckLinkInputCommand.h"
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

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        bool preview;
        LibraryModel* model;
        DeckLinkInputCommand* command;

        void loadDirection();
        void loadFormat();
        void loadTransition();
        void loadTween();

        Q_SLOT void transitionChanged(QString);
        Q_SLOT void durationChanged(int);
        Q_SLOT void directionChanged(QString);
        Q_SLOT void tweenChanged(QString);
        Q_SLOT void deviceChanged(int);
        Q_SLOT void formatChanged(QString);
        Q_SLOT void resetDevice(QString);
        Q_SLOT void resetFormat(QString);
        Q_SLOT void resetTransition(QString);
        Q_SLOT void resetDuration(QString);
        Q_SLOT void resetDirection(QString);
        Q_SLOT void resetTween(QString);
};
