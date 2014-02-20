#pragma once

#include "../../Shared.h"
#include "ui_InspectorPresetWidget.h"

#include "Commands/TriCaster/PresetCommand.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorPresetWidget : public QWidget, Ui::InspectorPresetWidget
{
    Q_OBJECT

    public:
        explicit InspectorPresetWidget(QWidget* parent = 0);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        LibraryModel* model;
        PresetCommand* command;

        void loadTriCasterSource();
        void loadTriCasterPreset();
        void blockAllSignals(bool block);

        Q_SLOT void sourceChanged(int);
        Q_SLOT void presetChanged(int);
        Q_SLOT void triggerOnNextChanged(int);
};
