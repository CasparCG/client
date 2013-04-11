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
        int tabCount;

        QMenu* contextMenuColor;
        QMenu* contextMenuRundownDropdown;

        void setupMenus();

        Q_SLOT void addRundown();
        Q_SLOT void renameRundown();
        Q_SLOT bool selectTab(int index);
        Q_SLOT void currentChanged(int);
        Q_SLOT void tabCloseRequested(int);
};
