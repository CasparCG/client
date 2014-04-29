#pragma once

#include "../Shared.h"
#include "ui_InspectorCropWidget.h"

#include "Commands/CropCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorCropWidget : public QWidget, Ui::InspectorCropWidget
{
    Q_OBJECT

    public:
        explicit InspectorCropWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        CropCommand* command;

        void loadTween();
        void blockAllSignals(bool block);

        Q_SLOT void sliderCropLeftChanged(int);
        Q_SLOT void sliderCropRightChanged(int);
        Q_SLOT void sliderCropTopChanged(int);
        Q_SLOT void sliderCropBottomChanged(int);
        Q_SLOT void spinBoxCropLeftChanged(int);
        Q_SLOT void spinBoxCropRightChanged(int);
        Q_SLOT void spinBoxCropTopChanged(int);
        Q_SLOT void spinBoxCropBottomChanged(int);
        Q_SLOT void transitionDurationChanged(int);
        Q_SLOT void tweenChanged(QString);
        Q_SLOT void deferChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
