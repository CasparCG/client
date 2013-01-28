#pragma once

#include "../Shared.h"
#include "ui_InspectorKeyerWidget.h"

#include "Commands/KeyerCommand.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorKeyerWidget : public QWidget, Ui::InspectorKeyerWidget
{
    Q_OBJECT

    public:
        explicit InspectorKeyerWidget(QWidget* parent = 0);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        bool preview;
        LibraryModel* model;
        KeyerCommand* command;

        Q_SLOT void deferChanged(int);
        Q_SLOT void resetDefer(QString);
};
