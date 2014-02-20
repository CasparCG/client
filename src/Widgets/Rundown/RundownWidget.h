#pragma once

#include "../Shared.h"
#include "ui_RundownWidget.h"

#include "Global.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QKeyEvent>
#include <QtGui/QWidget>

class WIDGETS_EXPORT RundownWidget : public QWidget, Ui::RundownWidget
{
    Q_OBJECT

    public:
        explicit RundownWidget(QWidget* parent = 0);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        QMenu* contextMenuRundownDropdown;

        QAction* newRundownAction;
        QAction* allowRemoteTriggeringAction;

        void setupMenus();

        Q_SLOT void newRundown();
        Q_SLOT void openRundown();
        Q_SLOT void saveRundown();
        Q_SLOT void saveAsRundown();
        Q_SLOT void toggleCompactView();
        Q_SLOT void closeRundown();
        Q_SLOT bool selectTab(int index);
        Q_SLOT void currentChanged(int);
        Q_SLOT void tabCloseRequested(int);
        Q_SLOT void gpiBindingChanged(int, Playout::PlayoutType::Type);
        Q_SLOT void allowRemoteTriggering(bool enabled);
};
