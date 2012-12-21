#pragma once

#include "../Shared.h"
#include "ui_InspectorGridWidget.h"

#include "Commands/GridCommand.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorGridWidget : public QWidget, Ui::InspectorGridWidget
{
    Q_OBJECT

    public:
        explicit InspectorGridWidget(QWidget* parent = 0);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        bool preview;
        LibraryModel* model;
        GridCommand* command;

        void loadTween();

        Q_SLOT void gridChanged(int);
        Q_SLOT void durationChanged(int);
        Q_SLOT void tweenChanged(QString);
        Q_SLOT void resetGrid(QString);
        Q_SLOT void resetDuration(QString);
        Q_SLOT void resetTween(QString);
        Q_SLOT void deferChanged(int);
        Q_SLOT void resetDefer(QString);
};
