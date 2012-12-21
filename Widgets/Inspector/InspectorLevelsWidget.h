#pragma once

#include "../Shared.h"
#include "ui_InspectorLevelsWidget.h"

#include "Commands/LevelsCommand.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorLevelsWidget : public QWidget, Ui::InspectorLevelsWidget
{
    Q_OBJECT

    public:
        explicit InspectorLevelsWidget(QWidget* parent = 0);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        bool preview;
        LibraryModel* model;
        LevelsCommand* command;

        void loadTween();

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
        Q_SLOT void durationChanged(int);
        Q_SLOT void tweenChanged(QString);
        Q_SLOT void resetMinIn(QString);
        Q_SLOT void resetMinOut(QString);
        Q_SLOT void resetMaxIn(QString);
        Q_SLOT void resetMaxOut(QString);
        Q_SLOT void resetGamma(QString);
        Q_SLOT void resetDuration(QString);
        Q_SLOT void resetTween(QString);
        Q_SLOT void deferChanged(int);
        Q_SLOT void resetDefer(QString);
};
