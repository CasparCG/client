#include "MainWindow.h"

#include "Global.h"

#include "Events/CompactViewEvent.h"
#include "Events/OpenRundownEvent.h"
#include "Events/SaveRundownEvent.h"
#include "Events/StatusbarEvent.h"

#include "AboutDialog.h"
#include "HelpDialog.h"
#include "SettingsDialog.h"

#include <QtCore/QDebug>
#include <QtCore/QEvent>
#include <QtCore/QString>
#include <QtCore/QUrl>

#include <QtGui/QApplication>
#include <QtGui/QDesktopServices>
#include <QtGui/QFileDialog>
#include <QtGui/QIcon>
#include <QtGui/QKeyEvent>
#include <QtGui/QMouseEvent>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent),
    currentFilename("")
{
    setupUi(this);
    setupUiMenu();
    setWindowIcon(QIcon(":/Graphics/Images/CasparCG.png"));

    this->applicationTitle = this->windowTitle();
    setWindowTitle(QString("%1 %2.%3 %4").arg(this->applicationTitle).arg(MAJOR_VERSION).arg(MINOR_VERSION).arg(TAG_VERSION));

    this->widgetOnAirNow->setVisible(false);
    this->widgetClock->setVisible(false);
    this->widgetOnAirNext->setVisible(false);
    this->widgetTimeline->setVisible(false);
    this->widgetPreview->setVisible(false);
    this->splitterHorizontal->setSizes(QList<int>() << 0 << 1);

    qApp->installEventFilter(this);
}

void MainWindow::setupUiMenu()
{
    this->fileMenu = new QMenu(this);
    this->fileMenu->addAction("Open", this, SLOT(openRundown()), QKeySequence::fromString("Ctrl+O"));
    this->fileMenu->addSeparator();
    this->fileMenu->addAction("Save", this, SLOT(saveRundown()), QKeySequence::fromString("Ctrl+S"));
    this->fileMenu->addAction("Save As...", this, SLOT(saveAsRundown()));
    //this->fileMenu->addSeparator();
    //this->fileMenu->addAction("Import");//, this, SLOT(showAboutDialog()), QKeySequence::fromString("Ctrl+I"));
    //this->fileMenu->addAction("Export");//, this, SLOT(showAboutDialog()), QKeySequence::fromString("Ctrl+E"));
    this->fileMenu->addSeparator();
    this->fileMenu->addAction("Quit", this, SLOT(close()));
    //this->fileMenu->actions().at(3)->setEnabled(false);
    //this->fileMenu->actions().at(4)->setEnabled(false);

    this->editMenu = new QMenu(this);
    this->editMenu->addAction("Settings", this, SLOT(showSettingsDialog()));

    this->viewMenu = new QMenu(this);
    this->viewMenu->addAction("Toggle Fullscreen", this, SLOT(toggleFullscreen()), QKeySequence::fromString("Ctrl+F"));
    this->viewMenu->addAction("Toggle Compact View", this, SLOT(toggleCompactView()), QKeySequence::fromString("Ctrl+W"));

    this->playoutMenu = new QMenu(this);
    this->playoutMenu->addAction("Stop", this, SLOT(executeStop()), QKeySequence::fromString("F1"));
    this->playoutMenu->addAction("Play", this, SLOT(executePlay()), QKeySequence::fromString("F2"));
    this->playoutMenu->addAction("Load", this, SLOT(executeLoad()), QKeySequence::fromString("F3"));
    this->playoutMenu->addAction("Pause", this, SLOT(executePause()), QKeySequence::fromString("F4"));
    this->playoutMenu->addSeparator();
    this->playoutMenu->addAction("Next", this, SLOT(executeNext()), QKeySequence::fromString("F5"));
    this->playoutMenu->addAction("Update", this, SLOT(executeUpdate()), QKeySequence::fromString("F6"));
    this->playoutMenu->addAction("Invoke", this, SLOT(executeInvoke()), QKeySequence::fromString("F7"));
    this->playoutMenu->addSeparator();
    this->playoutMenu->addAction("Clear", this, SLOT(executeClear()), QKeySequence::fromString("F10"));
    this->playoutMenu->addAction("Clear Video Layer", this, SLOT(executeClearVideolayer()), QKeySequence::fromString("F11"));
    this->playoutMenu->addAction("Clear Channel", this, SLOT(executeClearChannel()), QKeySequence::fromString("F12"));

    this->helpMenu = new QMenu(this);
    this->helpMenu->addAction("View Help", this, SLOT(showHelpDialog()), QKeySequence::fromString("Ctrl+H"));
    this->helpMenu->addSeparator();
    this->helpMenu->addAction("About CasparCG Rundown");//, this, SLOT(showAboutDialog()));
    this->helpMenu->actions().at(2)->setEnabled(false);

    this->menuBar = new QMenuBar(this);
    this->menuBar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->menuBar->addMenu(this->fileMenu)->setText("File");
    this->menuBar->addMenu(this->editMenu)->setText("Edit");
    this->menuBar->addMenu(this->viewMenu)->setText("View");
    this->menuBar->addMenu(this->playoutMenu)->setText("Playout");
    this->menuBar->addMenu(this->helpMenu)->setText("Help");

    this->setMenuBar(this->menuBar);
}

bool MainWindow::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::StatusbarMessage))
    {
        StatusbarEvent* statusbarEvent = dynamic_cast<StatusbarEvent*>(event);
        if (statusbarEvent->getTimeout() == 0)
            statusBar()->showMessage(statusbarEvent->getMessage());
        else
            statusBar()->showMessage(statusbarEvent->getMessage(), statusbarEvent->getTimeout());
    }

    return QObject::eventFilter(target, event);
}

void MainWindow::openRundown()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open Rundown", "", "Rundown (*.xml)");
    if (!filename.isEmpty())
    {
        qApp->postEvent(qApp, new StatusbarEvent("Opening rundown..."));
        qApp->postEvent(qApp, new OpenRundownEvent(filename));

        this->currentFilename = filename;
        setWindowTitle(QString("%1 %2.%3 %4 - %5").arg(this->applicationTitle)
                       .arg(MAJOR_VERSION).arg(MINOR_VERSION).arg(TAG_VERSION).arg(this->currentFilename));
    }
}

void MainWindow::saveRundown()
{
    QString filename = (!this->currentFilename.isEmpty()) ? this->currentFilename : QFileDialog::getSaveFileName(this, "Save Rundown", "", "Rundown (*.xml)");
    if (!filename.isEmpty())
    {
        qApp->postEvent(qApp, new StatusbarEvent("Saving rundown..."));
        qApp->postEvent(qApp, new SaveRundownEvent(filename));

        this->currentFilename = filename;
        setWindowTitle(QString("%1 %2.%3 %4 - %5").arg(this->applicationTitle)
                       .arg(MAJOR_VERSION).arg(MINOR_VERSION).arg(TAG_VERSION).arg(this->currentFilename));
    }
}

void MainWindow::saveAsRundown()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save Rundown", "", "Rundown (*.xml)");
    if (!filename.isEmpty())
    {
        qApp->postEvent(qApp, new StatusbarEvent("Saving rundown..."));
        qApp->postEvent(qApp, new SaveRundownEvent(filename));

        this->currentFilename = filename;
        setWindowTitle(QString("%1 %2.%3 %4 - %5").arg(this->applicationTitle)
                       .arg(MAJOR_VERSION).arg(MINOR_VERSION).arg(TAG_VERSION).arg(this->currentFilename));
    }
}

void MainWindow::executeStop()
{
    qApp->postEvent(qApp, new QKeyEvent(QEvent::KeyPress, Qt::Key_F1, Qt::NoModifier));
}

void MainWindow::executePlay()
{
    qApp->postEvent(qApp, new QKeyEvent(QEvent::KeyPress, Qt::Key_F2, Qt::NoModifier));
}

void MainWindow::executePause()
{
    qApp->postEvent(qApp, new QKeyEvent(QEvent::KeyPress, Qt::Key_F4, Qt::NoModifier));
}

void MainWindow::executeLoad()
{
    qApp->postEvent(qApp, new QKeyEvent(QEvent::KeyPress, Qt::Key_F3, Qt::NoModifier));
}

void MainWindow::executeNext()
{
    qApp->postEvent(qApp, new QKeyEvent(QEvent::KeyPress, Qt::Key_F6, Qt::NoModifier));
}

void MainWindow::executeUpdate()
{
    qApp->postEvent(qApp, new QKeyEvent(QEvent::KeyPress, Qt::Key_F7, Qt::NoModifier));
}

void MainWindow::executeInvoke()
{
    qApp->postEvent(qApp, new QKeyEvent(QEvent::KeyPress, Qt::Key_F8, Qt::NoModifier));
}

void MainWindow::executeClear()
{
    qApp->postEvent(qApp, new QKeyEvent(QEvent::KeyPress, Qt::Key_F10, Qt::NoModifier));
}

void MainWindow::executeClearVideolayer()
{
    qApp->postEvent(qApp, new QKeyEvent(QEvent::KeyPress, Qt::Key_F11, Qt::NoModifier));
}

void MainWindow::executeClearChannel()
{
    qApp->postEvent(qApp, new QKeyEvent(QEvent::KeyPress, Qt::Key_F12, Qt::NoModifier));
}

void MainWindow::toggleCompactView()
{
    qApp->postEvent(qApp, new CompactViewEvent());
}

void MainWindow::showAboutDialog()
{
    AboutDialog* dialog = new AboutDialog(this);
    dialog->exec();
}

void MainWindow::showHelpDialog()
{
    HelpDialog* dialog = new HelpDialog(this);
    dialog->exec();
}

void MainWindow::showSettingsDialog()
{
    SettingsDialog* dialog = new SettingsDialog(this);

    QObject::connect(dialog, SIGNAL(gpiBindingChanged(int, Playout::PlayoutType::Type)),
                     widgetRundown, SLOT(gpiBindingChanged(int, Playout::PlayoutType::Type)));

    dialog->exec();
}

void MainWindow::toggleFullscreen()
{
    isFullScreen() ? showNormal() : showFullScreen();
}
