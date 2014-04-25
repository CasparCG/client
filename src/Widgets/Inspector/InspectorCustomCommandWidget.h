#pragma once

#include "../Shared.h"
#include "ui_InspectorCustomCommandWidget.h"

#include "Commands/CustomCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>
#include <QtCore/QString>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorCustomCommandWidget : public QWidget, Ui::InspectorCustomCommandWidget
{
    Q_OBJECT

    public:
        explicit InspectorCustomCommandWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        CustomCommand* command;

        void blockAllSignals(bool block);

        Q_SLOT void stopCommandChanged(QString);
        Q_SLOT void playCommandChanged(QString);
        Q_SLOT void loadCommandChanged(QString);
        Q_SLOT void pauseCommandChanged(QString);
        Q_SLOT void nextCommandChanged(QString);
        Q_SLOT void updateCommandChanged(QString);
        Q_SLOT void invokeCommandChanged(QString);
        Q_SLOT void previewCommandChanged(QString);
        Q_SLOT void clearCommandChanged(QString);
        Q_SLOT void clearVideolayerCommandChanged(QString);
        Q_SLOT void clearChannelCommandChanged(QString);
        Q_SLOT void triggerOnNextChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
