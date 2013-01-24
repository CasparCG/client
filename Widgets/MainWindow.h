#pragma once

#include "Shared.h"
#include "ui_MainWindow.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QWidget>

class WIDGETS_EXPORT MainWindow : public QMainWindow, Ui::MainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget* parent = 0);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        QString currentFilename;
        QString applicationTitle;

        QMenu* fileMenu;
        QMenu* editMenu;
        QMenu* viewMenu;
        QMenu* playoutMenu;
        QMenu* helpMenu;
        QMenuBar* menuBar;

        void setupUiMenu();

        Q_SLOT void openRundown();
        Q_SLOT void saveRundown();
        Q_SLOT void saveAsRundown();
        Q_SLOT void executeStop();
        Q_SLOT void executePlay();
        Q_SLOT void executePause();
        Q_SLOT void executeLoad();
        Q_SLOT void executeNext();
        Q_SLOT void executeUpdate();
        Q_SLOT void executeInvoke();
        Q_SLOT void executeClear();
        Q_SLOT void executeClearVideolayer();
        Q_SLOT void executeClearChannel();
        Q_SLOT void showAboutDialog();
        Q_SLOT void showHelpDialog();
        Q_SLOT void showSettingsDialog();
        Q_SLOT void toggleFullscreen();
        Q_SLOT void toggleCompactView();
};
