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
#include <QtCore/QCommandLineParser>

#include <QtGui/QPixmap>
#include <QtGui/QFontDatabase>

#include <QtWidgets/QApplication>
#include <QtWidgets/QSplashScreen>
#include <QtWidgets/QStyleFactory>

#include <QtSql/QSqlDatabase>

void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& message)
{
    Q_UNUSED(context);

    QString logMessage;
    QString threadId = QString::number((long long)QThread::currentThreadId(), 16);
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");

    switch (type)
    {
        case QtDebugMsg:
            logMessage = QString("[%1] [%2] [D] %3").arg(timestamp).arg(threadId).arg(message);
            break;
        case QtWarningMsg:
            logMessage = QString("\033[33m[%1] [%2] [W] %3\033[0m").arg(timestamp).arg(threadId).arg(message);
            break;
        case QtCriticalMsg:
            logMessage = QString("\033[31m[%1] [%2] [C] %3\033[0m").arg(timestamp).arg(threadId).arg(message);
            break;
        case QtFatalMsg:
            logMessage = QString("\033[31m[%1] [%2] [F] %3\033[0m").arg(timestamp).arg(threadId).arg(message);
    }

    fprintf(stderr, "%s\n", qPrintable(logMessage));

    QString path = QString("%1/.CasparCG/Client/Logs").arg(QDir::homePath());

    QDir directory;
    if (!directory.exists(path))
        directory.mkpath(path);

    QFile logFile(QString("%1/Client_%2.log").arg(path).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")));
    logFile.open(QIODevice::WriteOnly | QIODevice::Append);

    QTextStream logStream(&logFile);
    logStream << logMessage << endl;

    logFile.close();

    if (type == QtFatalMsg)
       abort();
}

void loadDatabase(const QCommandLineParser& parser)
{
    QString path = QString("%1/.CasparCG/Client").arg(QDir::homePath());

    QDir directory;
    if (!directory.exists(path))
        directory.mkpath(path);

    QString databaseLocation = QString("%1/Database.s3db").arg(path);
    if (parser.isSet("database"))
        databaseLocation = parser.value("database");

    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    if (parser.isSet("dbmemory"))
        database.setDatabaseName(":memory:");
    else
        database.setDatabaseName(databaseLocation);

    if (!database.open())
        qCritical("Unable to open database");
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

void loadConfiguration(QApplication& application, QMainWindow& window, const QCommandLineParser& parser)
{
    QString stylesheet = QString("QWidget { font-size: %1px; }").arg(DatabaseManager::getInstance().getConfigurationByName("FontSize").getValue().toInt());
    application.setStyleSheet(application.styleSheet() + stylesheet);

    // Check command line arguments followed by the configuration.
    if (parser.isSet("fullscreen") || DatabaseManager::getInstance().getConfigurationByName("StartFullscreen").getValue() == "true")
         window.showFullScreen();

    if (parser.isSet("rundown"))
        EventManager::getInstance().fireOpenRundownEvent(OpenRundownEvent(parser.value("rundown")));
}

int main(int argc, char* argv[])
{
    qInstallMessageHandler(messageHandler);

    Application application(argc, argv);
    application.setApplicationName("CasparCG Client");
    application.setApplicationVersion(QString("%1.%2.%3.%4").arg(MAJOR_VERSION).arg(MINOR_VERSION).arg(REVISION_VERSION).arg(BUILD_VERSION));

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption rundownOption("rundown", "Open specified rundown");
    parser.addOption(rundownOption);

    QCommandLineOption fullscreenOption("fullscreen", "Start the application in fullscreen");
    parser.addOption(fullscreenOption);

    QCommandLineOption databaseOption("database", "Open specified database");
    parser.addOption(databaseOption);

    QCommandLineOption dbmemoryOption("dbmemory", "Use in memory database");
    parser.addOption(dbmemoryOption);

    parser.process(application);

    QSplashScreen splashScreen(QPixmap(":/Graphics/Images/SplashScreen.png"));
    splashScreen.show();

    application.setStyle(QStyleFactory::create("plastique"));

    loadDatabase(parser);
    DatabaseManager::getInstance().initialize();

    qDebug("Starting %s %s", qPrintable(application.applicationName()), qPrintable(application.applicationVersion()));

    loadStyleSheets(application);
    loadFonts(application);

    EventManager::getInstance().initialize();
    GpiManager::getInstance().initialize();

    MainWindow window;
    splashScreen.finish(&window);

    loadConfiguration(application, window, parser);

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
