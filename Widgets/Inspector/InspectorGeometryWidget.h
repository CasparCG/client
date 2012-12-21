#pragma once

#include "../Shared.h"
#include "ui_InspectorGeometryWidget.h"

#include "Commands/GeometryCommand.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorGeometryWidget : public QWidget, Ui::InspectorGeometryWidget
{
    Q_OBJECT

    public:
        explicit InspectorGeometryWidget(QWidget* parent = 0);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        bool preview;
        LibraryModel* model;
        GeometryCommand* command;
        int resolutionWidth;
        int resolutionHeight;

        void loadTween();

        Q_SLOT void sliderPositionXChanged(int);
        Q_SLOT void spinBoxPositionXChanged(int);
        Q_SLOT void sliderPositionYChanged(int);
        Q_SLOT void spinBoxPositionYChanged(int);
        Q_SLOT void sliderScaleXChanged(int);
        Q_SLOT void spinBoxScaleXChanged(int);
        Q_SLOT void sliderScaleYChanged(int);
        Q_SLOT void spinBoxScaleYChanged(int);
        Q_SLOT void durationChanged(int);
        Q_SLOT void tweenChanged(QString);
        Q_SLOT void resetPositionX(QString);
        Q_SLOT void resetPositionY(QString);
        Q_SLOT void resetScaleX(QString);
        Q_SLOT void resetScaleY(QString);
        Q_SLOT void resetDuration(QString);
        Q_SLOT void resetTween(QString);
        Q_SLOT void deferChanged(int);
        Q_SLOT void resetDefer(QString);
};
