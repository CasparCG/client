#pragma once

#include "../Shared.h"
#include "ui_InspectorVideoWidget.h"

#include "Commands/VideoCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>
#include <QtCore/QString>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

class WIDGETS_EXPORT InspectorVideoWidget : public QWidget, Ui::InspectorVideoWidget
{
    Q_OBJECT

    public:
        explicit InspectorVideoWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        VideoCommand* command;
        bool enableOscInput;

        void loadDirection();
        void loadTransition();
        void loadTween();
        void blockAllSignals(bool block);

        Q_SLOT void transitionChanged(QString);
        Q_SLOT void transitionDurationChanged(int);
        Q_SLOT void directionChanged(QString);
        Q_SLOT void tweenChanged(QString);
        Q_SLOT void loopChanged(int);
        Q_SLOT void freezeOnLoadChanged(int);
        Q_SLOT void triggerOnNextChanged(int);
        Q_SLOT void seekChanged(int);
        Q_SLOT void lengthChanged(int);
        Q_SLOT void autoPlayChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
