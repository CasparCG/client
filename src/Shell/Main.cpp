#include "Version.h"
#include "Global.h"

#include "Application.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "GpiManager.h"
#include "LibraryManager.h"
#include "DeviceManager.h"
#include "OscDeviceManager.h"
#include "AtemDeviceManager.h"
#include "TriCasterDeviceManager.h"
#include "Events/Rundown/OpenRundownEvent.h"

#include "MainWindow.h"

#include <QtCore/QRegExp>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QTextStream>

#include <QtGui/QApplication>
#include <QtGui/QFontDatabase>
#include <QtGui/QDesktopServices>

#include <QtSql/QSqlDatabase>

void loadDatabase(QApplication& application)
{
    //QString path = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    QString path = QString("%1/.CasparCG/Client").arg(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));

    QDir directory;
    if (!directory.exists(path))
        directory.mkpath(path);

    QString databaseLocation = QString("%1/%2.s3db").arg(path).arg(DATABASE_VERSION);
    if (application.arguments().contains("-database"))
        databaseLocation = QString("%1/%2%3.s3db").arg(path).arg(application.arguments().at(application.arguments().indexOf(QRegExp("-database")) + 1)).arg(DATABASE_VERSION);

    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    if (application.arguments().contains("-dbmemory"))
        database.setDatabaseName(":memory:");
    else
        database.setDatabaseName(databaseLocation);

    if (!database.open())
        qCritical() << "Unable to open database";
}

void loadStyleSheets(QApplication& application)
{
    QString stylesheet;
    QString theme = DatabaseManager::getInstance().getConfigurationByName("Theme").getValue();

    // Load default stylesheet..
    QFile defaultStylesheet(QString(":/Appearances/Stylesheets/%1/Default.css").arg(theme));
    if (defaultStylesheet.open(QFile::ReadOnly))
    {
        QTextStream stream(&defaultStylesheet);
        stylesheet = stream.readAll();
        defaultStylesheet.close();

        application.setStyleSheet(stylesheet);

        defaultStylesheet.close();
    }

    // Load extended stylesheet.
    QFile extendedStylesheet(QString(":/Appearances/Stylesheets/%1/Extended.css").arg(theme));
    if (extendedStylesheet.open(QFile::ReadOnly))
    {
        QTextStream stream(&extendedStylesheet);
        stylesheet += stream.readAll();
        extendedStylesheet.close();

        application.setStyleSheet(stylesheet);

        extendedStylesheet.close();
    }

    // Load platform stylesheet.
#if defined(Q_OS_WIN)
    QFile platformStylesheet(QString(":/Appearances/Stylesheets/%1/Windows.css").arg(theme));
#elif defined(Q_OS_MAC)
    QFile platformStylesheet(QString(":/Appearances/Stylesheets/%1/Mac.css").arg(theme));
#elif defined(Q_OS_LINUX)
    QFile platformStylesheet(QString(":/Appearances/Stylesheets/%1/Linux.css").arg(theme));
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

#if defined(Q_OS_UNIX)
    application.setFont(QFont("Open Sans"));
#elif defined(Q_OS_WIN)
   application.setFont(QFont("Open Sans Semibold"));
#endif
}

void loadConfiguration(QApplication& application, QMainWindow& window)
{
    QString stylesheet = QString("QWidget { font-size: %1px; }").arg(DatabaseManager::getInstance().getConfigurationByName("FontSize").getValue().toInt());
    application.setStyleSheet(application.styleSheet() + stylesheet);

    // Check command line arguments followed by the configuration.
    if (application.arguments().contains("-fullscreen") || DatabaseManager::getInstance().getConfigurationByName("StartFullscreen").getValue() == "true")
         window.showFullScreen();

    if (application.arguments().contains("-rundown"))
    {
        int i = application.arguments().indexOf("-rundown");
        if (application.arguments().count() > i + 1)
        {
            QString path = application.arguments().at(i + 1);

            EventManager::getInstance().fireOpenRundownEvent(OpenRundownEvent(path));
        }
    }
}

int main(int argc, char* argv[])
{
#if defined(Q_OS_MAC)
    if (QSysInfo::MacintoshVersion > QSysInfo::MV_10_8)
        QFont::insertSubstitution(".Lucida Grande UI", "Lucida Grande"); // FIX: Mac OSX 10.9 (Mavericks) font issue: https://bugreports.qt-project.org/browse/QTBUG-32789
#endif

    //Application::setOrganizationName("CasparCG");
    //Application::setApplicationName("CasparCG Client");
    Application::setGraphicsSystem("raster");
    Application application(argc, argv);
    application.setStyle("plastique");

    loadDatabase(application);
    DatabaseManager::getInstance().initialize();

    loadStyleSheets(application);
    loadFonts(application);

    EventManager::getInstance().initialize();
    GpiManager::getInstance().initialize();

    MainWindow window;

    loadConfiguration(application, window);

    window.show();

    LibraryManager::getInstance().initialize();
    DeviceManager::getInstance().initialize();
    AtemDeviceManager::getInstance().initialize();
    TriCasterDeviceManager::getInstance().initialize();
    OscDeviceManager::getInstance().initialize();

    int returnValue = application.exec();

    EventManager::getInstance().uninitialize();
    DatabaseManager::getInstance().uninitialize();
    GpiManager::getInstance().uninitialize();
    OscDeviceManager::getInstance().uninitialize();
    TriCasterDeviceManager::getInstance().uninitialize();
    AtemDeviceManager::getInstance().uninitialize();
    DeviceManager::getInstance().uninitialize();
    LibraryManager::getInstance().uninitialize();

    return returnValue;
}
