#pragma once

#include "../Shared.h"
#include "ui_InspectorImageScrollerWidget.h"

#include "Commands/ImageScrollerCommand.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>
#include <QtCore/QString>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorImageScrollerWidget : public QWidget, Ui::InspectorImageScrollerWidget
{
    Q_OBJECT

    public:
        explicit InspectorImageScrollerWidget(QWidget* parent = 0);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        bool preview;
        LibraryModel* model;
        ImageScrollerCommand* command;

        Q_SLOT void blurChanged(int);
        Q_SLOT void speedChanged(int);
        Q_SLOT void premultiplyChanged(int);
        Q_SLOT void progressiveChanged(int);
        Q_SLOT void resetSpeed(QString);
        Q_SLOT void resetBlur(QString);
        Q_SLOT void resetPremultiply(QString);
        Q_SLOT void resetProgressive(QString);
};
