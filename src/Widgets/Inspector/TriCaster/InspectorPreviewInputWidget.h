#pragma once

#include "../../Shared.h"
#include "ui_InspectorPreviewInputWidget.h"

#include "Commands/TriCaster/PreviewInputCommand.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorPreviewInputWidget : public QWidget, Ui::InspectorPreviewInputWidget
{
    Q_OBJECT

    public:
        explicit InspectorPreviewInputWidget(QWidget* parent = 0);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        LibraryModel* model;
        PreviewInputCommand* command;

        void loadTriCasterInput();
        void blockAllSignals(bool block);

        Q_SLOT void inputChanged(int);
};
