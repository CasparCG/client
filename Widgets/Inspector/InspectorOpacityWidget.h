#pragma once

#include "../Shared.h"
#include "ui_InspectorOpacityWidget.h"

#include "Commands/OpacityCommand.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorOpacityWidget : public QWidget, Ui::InspectorOpacityWidget
{
    Q_OBJECT

    public:
        explicit InspectorOpacityWidget(QWidget* parent = 0);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        bool preview;
        LibraryModel* model;
        OpacityCommand* command;

        void loadTween();

        Q_SLOT void sliderOpacityChanged(int);
        Q_SLOT void spinBoxOpacityChanged(int);
        Q_SLOT void durationChanged(int);
        Q_SLOT void tweenChanged(QString);
        Q_SLOT void resetOpacity(QString);
        Q_SLOT void resetDuration(QString);
        Q_SLOT void resetTween(QString);
        Q_SLOT void deferChanged(int);
        Q_SLOT void resetDefer(QString);
};
