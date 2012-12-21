#pragma once

#include "../Shared.h"
#include "ui_InspectorBrightnessWidget.h"

#include "Commands/BrightnessCommand.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorBrightnessWidget : public QWidget, Ui::InspectorBrightnessWidget
{
    Q_OBJECT

    public:
        explicit InspectorBrightnessWidget(QWidget* parent = 0);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        bool preview;
        LibraryModel* model;
        BrightnessCommand* command;

        void loadTween();

        Q_SLOT void sliderBrightnessChanged(int);
        Q_SLOT void spinBoxBrightnessChanged(int);
        Q_SLOT void durationChanged(int);
        Q_SLOT void tweenChanged(QString);
        Q_SLOT void resetBrightness(QString);
        Q_SLOT void resetDuration(QString);
        Q_SLOT void resetTween(QString);
        Q_SLOT void deferChanged(int);
        Q_SLOT void resetDefer(QString);
};
