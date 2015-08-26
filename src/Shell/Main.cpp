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
#include <QtCore/QCommandLineOption>

#include <QtGui/QPixmap>
#include <QtGui/QFontDatabase>

#include <QtWidgets/QApplication>
#include <QtWidgets/QSplashScreen>
#include <QtWidgets/QStyleFactory>

#include <QtSql/QSqlDatabase>

struct CommandLineArgs
{
    QString rundown;

    QString mysqlhost;
    QString mysqluser;
    QString mysqlpass;
    QString mysqldb;
    QString sqlitepath;

    bool dbmemory = false;
    bool fullscreen = false;
};

enum CommandLineParseResult
{
    CommandLineOk,
    CommandLineError,
    CommandLineVersionRequested,
    CommandLineHelpRequested
};

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
            logMessage = QString("[%1] [%2] [W] %3").arg(timestamp).arg(threadId).arg(message);
            break;
        case QtCriticalMsg:
            logMessage = QString("[%1] [%2] [C] %3").arg(timestamp).arg(threadId).arg(message);
            break;
        case QtFatalMsg:
            logMessage = QString("[%1] [%2] [F] %3").arg(timestamp).arg(threadId).arg(message);
    }

    fprintf(stderr, "%s\n", qPrintable(logMessage));

    QString path = QString("%1/.CasparCG/Client/Logs").arg(QDir::homePath());

    QDir directory(path);
    if (!directory.exists())
        directory.mkpath(".");

    QFile logFile(QString("%1/Client_%2.log").arg(path).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")));
    logFile.open(QIODevice::WriteOnly | QIODevice::Append);

    QTextStream logStream(&logFile);
    logStream << logMessage << endl;

    logFile.close();

    if (type == QtFatalMsg)
       abort();
}

void loadDatabase(CommandLineArgs* args)
{
    QString path = QString("%1/.CasparCG/Client").arg(QDir::homePath());

    QDir directory(path);
    if (!directory.exists())
        directory.mkpath(".");

    QSqlDatabase database;
    if (args->dbmemory)
    {
        qDebug("Using SQLite in memory database");

        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(":memory:");
    }
    else
    {
        if (!args->mysqlhost.isEmpty() && !args->mysqluser.isEmpty() && !args->mysqlpass.isEmpty() && !args->mysqldb.isEmpty())
        {
            qDebug("Using MySQL database");

            database = QSqlDatabase::addDatabase("QMYSQL");
            database.setHostName(args->mysqlhost);
            database.setDatabaseName(args->mysqldb);
            database.setUserName(args->mysqluser);
            database.setPassword(args->mysqlpass);
        }
        else
        {
            qDebug("Using SQLite database");

            database = QSqlDatabase::addDatabase("QSQLITE");
            QString databaseLocation = QString("%1/Database.s3db").arg(path);
            if (!args->sqlitepath.isEmpty())
                databaseLocation = args->sqlitepath;

            database.setDatabaseName(databaseLocation);
        }
    }

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

void loadConfiguration(QApplication& application, QMainWindow& window, CommandLineArgs* args)
{
    QString stylesheet = QString("QWidget { font-size: %1px; }").arg(DatabaseManager::getInstance().getConfigurationByName("FontSize").getValue().toInt());
    application.setStyleSheet(application.styleSheet() + stylesheet);

    // Check command line arguments followed by the configuration.
    if (args->fullscreen || DatabaseManager::getInstance().getConfigurationByName("StartFullscreen").getValue() == "true")
         window.showFullScreen();

    if (!args->rundown.isEmpty())
        EventManager::getInstance().fireOpenRundownEvent(OpenRundownEvent(args->rundown));
}

CommandLineParseResult parseCommandLine(QCommandLineParser& parser, CommandLineArgs* args)
{
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addOption({{"f", "fullscreen"}, "Start application in fullscreen."});
    parser.addOption({{"m", "dbmemory"}, "Use SQLite in memory database."});
    parser.addOption({{"r", "rundown"}, "The rundown path.", "rundown"});
    parser.addOption({{"t", "sqlitepath"}, "The SQLite database path.", "sqlitepath"});
    parser.addOption({{"a", "mysqlhost"}, "MySQL database host.", "mysqlhost"});
    parser.addOption({{"u", "mysqluser"}, "MySQL database user.", "mysqluser"});
    parser.addOption({{"p", "mysqlpass"}, "MySQL database password.", "mysqlpass"});
    parser.addOption({{"n", "mysqldb"}, "MySQL database name.", "mysqldb"});

    if (!parser.parse(QApplication::arguments()))
        return CommandLineError;

    if (parser.isSet("version"))
        return CommandLineVersionRequested;

    if (parser.isSet("help"))
        return CommandLineHelpRequested;

    if (parser.isSet("rundown"))
        args->rundown = parser.value("rundown");

    if (parser.isSet("fullscreen"))
        args->fullscreen = true;

    if (parser.isSet("dbmemory"))
        args->dbmemory = true;

    if (parser.isSet("mysqlhost"))
        args->mysqlhost = parser.value("mysqlhost");

    if (parser.isSet("mysqluser"))
        args->mysqluser = parser.value("mysqluser");

    if (parser.isSet("mysqlpass"))
        args->mysqlpass = parser.value("mysqlpass");

    if (parser.isSet("mysqldb"))
        args->mysqldb = parser.value("mysqldb");

    if (parser.isSet("sqlitepath"))
        args->sqlitepath = parser.value("sqlitepath");

    return CommandLineOk;
}

int main(int argc, char* argv[])
{
    qInstallMessageHandler(messageHandler);

    Application application(argc, argv);
    application.setApplicationName("CasparCG Client");
    application.setApplicationVersion(QString("%1.%2.%3.%4").arg(MAJOR_VERSION).arg(MINOR_VERSION).arg(REVISION_VERSION).arg(BUILD_VERSION));

    qDebug("Starting %s %s", qPrintable(application.applicationName()), qPrintable(application.applicationVersion()));

    CommandLineArgs args;
    QCommandLineParser parser;
    switch (parseCommandLine(parser, &args))
    {
        case CommandLineOk:
            break;
        case CommandLineError:
            qCritical("Unable to parse command line: %s", qPrintable(parser.errorText()));
            parser.showHelp();
            return 0;
        case CommandLineVersionRequested:
            parser.showVersion();
            return 0;
        case CommandLineHelpRequested:
            parser.showHelp();
            return 0;
    }

    QSplashScreen splashScreen(QPixmap(":/Graphics/Images/SplashScreen.png"));
    splashScreen.show();

    loadDatabase(&args);
    DatabaseManager::getInstance().initialize();

    loadStyleSheets(application);
    loadFonts(application);

    EventManager::getInstance().initialize();
    GpiManager::getInstance().initialize();

    MainWindow window;
    splashScreen.finish(&window);

    loadConfiguration(application, window, &args);

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
