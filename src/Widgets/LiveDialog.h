#pragma once

#include "Shared.h"
#include "ui_LiveDialog.h"

#include "Events/ToggleFullscreenEvent.h"

#include <QtWidgets/QDialog>
#include <QtWidgets/QWidget>

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
