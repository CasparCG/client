#pragma once

#include "../Shared.h"
#include "ui_InspectorContrastWidget.h"

#include "Commands/ContrastCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorContrastWidget : public QWidget, Ui::InspectorContrastWidget
{
    Q_OBJECT

    public:
        explicit InspectorContrastWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        ContrastCommand* command;

        void loadTween();
        void blockAllSignals(bool block);

        Q_SLOT void sliderContrastChanged(int);
        Q_SLOT void spinBoxContrastChanged(int);
        Q_SLOT void transitionDurationChanged(int);
        Q_SLOT void tweenChanged(QString tween);
        Q_SLOT void deferChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
