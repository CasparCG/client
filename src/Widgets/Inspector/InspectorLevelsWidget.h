#pragma once

#include "../Shared.h"
#include "ui_InspectorLevelsWidget.h"

#include "Commands/LevelsCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorLevelsWidget : public QWidget, Ui::InspectorLevelsWidget
{
    Q_OBJECT

    public:
        explicit InspectorLevelsWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        LevelsCommand* command;

        void loadTween();
        void blockAllSignals(bool block);

        Q_SLOT void sliderMinInChanged(int);
        Q_SLOT void sliderMinOutChanged(int);
        Q_SLOT void sliderMaxInChanged(int);
        Q_SLOT void sliderMaxOutChanged(int);
        Q_SLOT void sliderGammaChanged(int);
        Q_SLOT void spinBoxMinInChanged(int);
        Q_SLOT void spinBoxMinOutChanged(int);
        Q_SLOT void spinBoxMaxInChanged(int);
        Q_SLOT void spinBoxMaxOutChanged(int);
        Q_SLOT void spinBoxGammaChanged(int);
        Q_SLOT void transitionDurationChanged(int);
        Q_SLOT void tweenChanged(QString);
        Q_SLOT void deferChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
