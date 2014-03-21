#pragma once

#include "../Shared.h"
#include "ui_InspectorFileRecorderWidget.h"

#include "Commands/FileRecorderCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>
#include <QtCore/QString>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorFileRecorderWidget : public QWidget, Ui::InspectorFileRecorderWidget
{
    Q_OBJECT

    public:
        explicit InspectorFileRecorderWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        FileRecorderCommand* command;

        void blockAllSignals(bool block);

        Q_SLOT void outputChanged(QString);
        Q_SLOT void codecChanged(QString);
        Q_SLOT void presetChanged(QString);
        Q_SLOT void tuneChanged(QString);
        Q_SLOT void withAlphaChanged(int);
        Q_SLOT void resetOutput(QString);
        Q_SLOT void resetCodec(QString);
        Q_SLOT void resetPreset(QString);
        Q_SLOT void resetTune(QString);
        Q_SLOT void resetWithAlpha(QString);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
