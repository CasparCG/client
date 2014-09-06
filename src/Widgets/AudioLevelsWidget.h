#pragma once

#include "Shared.h"
#include "ui_AudioLevelsWidget.h"

#if QT_VERSION >= 0x050000
#include <QImage>
#include <QtWidgets/QMenu>
#include <QtWidgets/QWidget>
#else
#include <QtGui/QImage>
#include <QtGui/QMenu>
#include <QtGui/QWidget>
#endif

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
