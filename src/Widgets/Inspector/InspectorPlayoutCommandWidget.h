#pragma once

#include "../Shared.h"
#include "ui_InspectorPlayoutCommandWidget.h"

#include "Commands/PlayoutCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

class WIDGETS_EXPORT InspectorPlayoutCommandWidget : public QWidget, Ui::InspectorPlayoutCommandWidget
{
    Q_OBJECT

    public:
        explicit InspectorPlayoutCommandWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        PlayoutCommand* command;

        void blockAllSignals(bool block);

        Q_SLOT void playoutCommandChanged(QString);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
