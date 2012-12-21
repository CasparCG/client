#pragma once

#include "../Shared.h"
#include "ui_InspectorMediaWidget.h"

#include "Commands/MediaCommand.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>
#include <QtCore/QString>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorMediaWidget : public QWidget, Ui::InspectorMediaWidget
{
    Q_OBJECT

    public:
        explicit InspectorMediaWidget(QWidget* parent = 0);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        bool preview;
        LibraryModel* model;
        MediaCommand* command;

        void loadDirection();
        void loadTransition();
        void loadTween();

        Q_SLOT void transitionChanged(QString);
        Q_SLOT void durationChanged(int);
        Q_SLOT void directionChanged(QString);
        Q_SLOT void tweenChanged(QString);
        Q_SLOT void loopChanged(int);
        Q_SLOT void seekChanged(int);
        Q_SLOT void lengthChanged(int);
        Q_SLOT void resetTransition(QString);
        Q_SLOT void resetDuration(QString);
        Q_SLOT void resetDirection(QString);
        Q_SLOT void resetTween(QString);
        Q_SLOT void resetSeek(QString);
        Q_SLOT void resetLength(QString);
        Q_SLOT void resetLoop(QString);
};
