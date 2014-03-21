#pragma once

#include "../Shared.h"
#include "ui_InspectorChromaWidget.h"

#include "Commands/ChromaCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorChromaWidget : public QWidget, Ui::InspectorChromaWidget
{
    Q_OBJECT

    public:
        explicit InspectorChromaWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        ChromaCommand* command;

        void loadChroma();
        void blockAllSignals(bool block);

        Q_SLOT void blurChanged(int);
        Q_SLOT void keyChanged(QString);
        Q_SLOT void showMaskChanged(bool);
        Q_SLOT void softnessChanged(int);
        Q_SLOT void spillChanged(int);
        Q_SLOT void thresholdChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
