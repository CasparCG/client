#pragma once

#include "../Shared.h"
#include "ui_InspectorVolumeWidget.h"

#include "Commands/VolumeCommand.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorVolumeWidget : public QWidget, Ui::InspectorVolumeWidget
{
    Q_OBJECT

    public:
        explicit InspectorVolumeWidget(QWidget* parent = 0);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        bool preview;
        LibraryModel* model;
        VolumeCommand* command;

        void loadTween();

        Q_SLOT void sliderVolumeChanged(int);
        Q_SLOT void spinBoxVolumeChanged(int);
        Q_SLOT void durationChanged(int);
        Q_SLOT void tweenChanged(QString);
        Q_SLOT void resetVolume(QString);
        Q_SLOT void resetDuration(QString);
        Q_SLOT void resetTween(QString);
        Q_SLOT void deferChanged(int);
        Q_SLOT void resetDefer(QString);
};
