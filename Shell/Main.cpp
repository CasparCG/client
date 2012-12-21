#include "Global.h"

#include "DatabaseManager.h"
#include "DeviceManager.h"
#include "GpiManager.h"
#include "SynchronizeManager.h"

#include "MainWindow.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QTextStream>

#include <QtGui/QApplication>
#include <QtGui/QFontDatabase>

#include <QtSql/QSqlDatabase>

void loadDatabase(QApplication& application)
{
    QString path = QString("%1/.casparcg/client").arg(QDir::homePath());

    QDir directory;
    if (!directory.exists(path))
        directory.mkpath(path);

    QString databaseLocation = QString("%1/%2.s3db").arg(path).arg(DATABASE_VERSION);
    if (application.arguments().contains("-database"))
        databaseLocation = QString("%1/%2%3.s3db").arg(path).arg(application.arguments().at(application.arguments().indexOf(QRegExp("-database")) + 1)).arg(DATABASE_VERSION);

    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(databaseLocation);
    if (!database.open())
        qCritical() << "Unable to open database";

    DatabaseManager::getInstance().initialize();
}

void loadStyleSheets(QApplication& application)
{
    QString stylesheet;

    // Load default stylesheet..
    QFile defaultStylesheet(":/Appearances/Stylesheets/Default.css");
    if (defaultStylesheet.open(QFile::ReadOnly))
    {
        QTextStream stream(&defaultStylesheet);
        stylesheet = stream.readAll();
        defaultStylesheet.close();

        application.setStyleSheet(stylesheet);

        defaultStylesheet.close();
    }

    // Load extended stylesheet.
    QFile extendedStylesheet(":/Appearances/Stylesheets/Extended.css");
    if (extendedStylesheet.open(QFile::ReadOnly))
    {
        QTextStream stream(&extendedStylesheet);
        stylesheet += stream.readAll();
        extendedStylesheet.close();

        application.setStyleSheet(stylesheet);

        extendedStylesheet.close();
    }

    // Load platform stylesheet.
#if defined(Q_OS_UNIX)
    QFile platformStylesheet(":/Appearances/Stylesheets/Unix.css");
#elif defined(Q_OS_WIN32)
    QFile platformStylesheet(":/Appearances/Stylesheets/Windows.css");
#endif
    if (platformStylesheet.open(QFile::ReadOnly))
    {
        QTextStream stream(&platformStylesheet);
        stylesheet += stream.readAll();
        platformStylesheet.close();

        application.setStyleSheet(stylesheet);

        platformStylesheet.close();
    }
}

void loadFonts(QApplication& application)
{
    QFontDatabase::addApplicationFont(":/Appearances/Fonts/OpenSans-Bold.ttf");
    QFontDatabase::addApplicationFont(":/Appearances/Fonts/OpenSans-BoldItalic.ttf");
    QFontDatabase::addApplicationFont(":/Appearances/Fonts/OpenSans-ExtraBold.ttf");
    QFontDatabase::addApplicationFont(":/Appearances/Fonts/OpenSans-ExtraBoldItalic.ttf");
    QFontDatabase::addApplicationFont(":/Appearances/Fonts/OpenSans-Italic.ttf");
    QFontDatabase::addApplicationFont(":/Appearances/Fonts/OpenSans-Light.ttf");
    QFontDatabase::addApplicationFont(":/Appearances/Fonts/OpenSans-LightItalic.ttf");
    QFontDatabase::addApplicationFont(":/Appearances/Fonts/OpenSans-Regular.ttf");
    QFontDatabase::addApplicationFont(":/Appearances/Fonts/OpenSans-Semibold.ttf");
    QFontDatabase::addApplicationFont(":/Appearances/Fonts/OpenSans-SemiboldItalic.ttf");

    application.setFont(QFont("Open Sans Semibold"));
}

void loadConfiguration(QApplication& application, QMainWindow& window)
{
    QString stylesheet = QString("QWidget { font-size: %1px; }").arg(DatabaseManager::getInstance().getConfigurationByName("FontSize").getValue().toInt());
    application.setStyleSheet(application.styleSheet() + stylesheet);

    // Check command line arguments followed by the configuration.
    if (application.arguments().contains("-fullscreen") || DatabaseManager::getInstance().getConfigurationByName("StartFullscreen").getValue() == "true")
         window.showFullScreen();
}

int main(int argc, char* argv[])
{
    QApplication::setGraphicsSystem("raster");
    QApplication application(argc, argv);
    application.setStyle("plastique");

    loadDatabase(application);
    loadStyleSheets(application);
    loadFonts(application);
    GpiManager::getInstance().initialize();

    MainWindow window;

    loadConfiguration(application, window);

    window.show();

    SynchronizeManager::getInstance().initialize();
    DeviceManager::getInstance().initialize();

    int returnValue = application.exec();

    GpiManager::getInstance().uninitialize();
    DeviceManager::getInstance().uninitialize();
    SynchronizeManager::getInstance().uninitialize();

    return returnValue;
}
