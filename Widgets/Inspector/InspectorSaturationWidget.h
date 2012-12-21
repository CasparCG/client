#pragma once

#include "../Shared.h"
#include "ui_InspectorSaturationWidget.h"

#include "Commands/SaturationCommand.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorSaturationWidget : public QWidget, Ui::InspectorSaturationWidget
{
    Q_OBJECT

    public:
        explicit InspectorSaturationWidget(QWidget* parent = 0);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        bool preview;
        LibraryModel* model;
        SaturationCommand* command;

        void loadTween();

        Q_SLOT void sliderSaturationChanged(int);
        Q_SLOT void spinBoxSaturationChanged(int);
        Q_SLOT void durationChanged(int);
        Q_SLOT void tweenChanged(QString);
        Q_SLOT void resetSaturation(QString);
        Q_SLOT void resetDuration(QString);
        Q_SLOT void resetTween(QString);
        Q_SLOT void deferChanged(int);
        Q_SLOT void resetDefer(QString);
};
