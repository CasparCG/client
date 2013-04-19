#pragma once

#include "../Shared.h"
#include "ui_InspectorSolidColorWidget.h"

#include "Commands/SolidColorCommand.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>
#include <QtCore/QString>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorSolidColorWidget : public QWidget, Ui::InspectorSolidColorWidget
{
    Q_OBJECT

    public:
        explicit InspectorSolidColorWidget(QWidget* parent = 0);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        LibraryModel* model;
        SolidColorCommand* command;

        void loadDirection();
        void loadTransition();
        void loadTween();
        void blockAllSignals(bool block);

        Q_SLOT void colorDialogClicked();
        Q_SLOT void colorChanged(QString);
        Q_SLOT void transitionChanged(QString);
        Q_SLOT void durationChanged(int);
        Q_SLOT void directionChanged(QString);
        Q_SLOT void tweenChanged(QString);
        Q_SLOT void useAutoChanged(int);
        Q_SLOT void resetTransition(QString);
        Q_SLOT void resetDuration(QString);
        Q_SLOT void resetDirection(QString);
        Q_SLOT void resetTween(QString);
        Q_SLOT void resetUseAuto(QString);
        Q_SLOT void resetColor(QString);
};
