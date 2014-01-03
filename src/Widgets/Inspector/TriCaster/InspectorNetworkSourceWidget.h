#pragma once

#include "../../Shared.h"
#include "ui_InspectorNetworkSourceWidget.h"

#include "Commands/TriCaster/NetworkSourceCommand.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorNetworkSourceWidget : public QWidget, Ui::InspectorNetworkSourceWidget
{
    Q_OBJECT

    public:
        explicit InspectorNetworkSourceWidget(QWidget* parent = 0);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        LibraryModel* model;
        NetworkSourceCommand* command;

        void loadTriCasterNetworkTarget();;
        void blockAllSignals(bool block);

        Q_SLOT void targetChanged(int);
        Q_SLOT void sourceChanged(QString);
};
