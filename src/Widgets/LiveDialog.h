#pragma once

#include "Shared.h"
#include "ui_LiveDialog.h"

#include "Events/ToggleFullscreenEvent.h"

#if QT_VERSION >= 0x050000
#include <QtWidgets/QDialog>
#include <QtWidgets/QWidget>
#else
#include <QtGui/QDialog>
#include <QtGui/QWidget>
#endif

class WIDGETS_EXPORT LiveDialog : public QDialog, Ui::LiveDialog
{
    Q_OBJECT

    public:
        explicit LiveDialog(QWidget* parent = 0);

        void visible();

        QWidget* getRenderTarget();

        Q_SLOT void toggleFullscreen(const ToggleFullscreenEvent&);

    private:
        bool applicationFullscreen;
};
