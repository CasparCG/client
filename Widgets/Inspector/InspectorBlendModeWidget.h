#pragma once

#include "../Shared.h"
#include "ui_InspectorBlendModeWidget.h"

#include "Commands/BlendModeCommand.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorBlendModeWidget : public QWidget, Ui::InspectorBlendModeWidget
{
    Q_OBJECT

    public:
        explicit InspectorBlendModeWidget(QWidget* parent = 0);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        bool preview;
        LibraryModel* model;
        BlendModeCommand* command;

        void loadBlendMode();

        Q_SLOT void blendModeChanged(QString);
        Q_SLOT void resetBlendMode(QString);
};
