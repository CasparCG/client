#pragma once

#include "Shared.h"
#include "ui_AudioLevelsWidget.h"

#include <QtGui/QImage>
#include <QtGui/QMenu>
#include <QtGui/QWidget>

class WIDGETS_EXPORT AudioLevelsWidget : public QWidget, Ui::AudioLevelsWidget
{
    Q_OBJECT

    public:
        explicit AudioLevelsWidget(QWidget* parent = 0);

    private:
        bool collapsed;

        QAction* expandCollapseAction;

        QMenu* contextMenuAudioLevelsDropdown;

        void setupMenus();

        Q_SLOT void toggleExpandCollapse();
};
