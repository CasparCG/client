#pragma once

#include "../Shared.h"
#include "ui_InspectorFileRecorderWidget.h"

#include "Commands/FileRecorderCommand.h"
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

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        bool preview;
        LibraryModel* model;
        FileRecorderCommand* command;

        Q_SLOT void filenameChanged(QString);
        Q_SLOT void containerChanged(QString);
        Q_SLOT void codecChanged(QString);
        Q_SLOT void presetChanged(QString);
        Q_SLOT void tuneChanged(QString);
        Q_SLOT void resetFilename(QString);
        Q_SLOT void resetContainer(QString);
        Q_SLOT void resetCodec(QString);
        Q_SLOT void resetPreset(QString);
        Q_SLOT void resetTune(QString);
};
