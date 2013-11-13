#include "DatabaseManager.h"

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QMutexLocker>
#include <QtCore/QTime>
#include <QtCore/QVariant>

#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

Q_GLOBAL_STATIC(DatabaseManager, databaseManager)

DatabaseManager::DatabaseManager()
    : mutex(QMutex::Recursive)
{
}

DatabaseManager& DatabaseManager::getInstance()
{
    return *databaseManager();
}

void DatabaseManager::initialize()
{
    QMutexLocker locker(&mutex);

    if (QSqlDatabase::database().tables().count() > 0)
        return;

    // Setup the database.
    QSqlQuery sql;
    sql.exec("CREATE TABLE BlendMode (Id INTEGER PRIMARY KEY, Value TEXT)");
    sql.exec("CREATE TABLE Configuration (Id INTEGER PRIMARY KEY, Name TEXT, Value TEXT)");
    sql.exec("CREATE TABLE Chroma (Id INTEGR PRIMARY KEY, Key TEXT)");
    sql.exec("CREATE TABLE Device (Id INTEGER PRIMARY KEY, Name TEXT, Address TEXT, Port INTEGER, Username TEXT, Password TEXT, Description TEXT, Version TEXT, Shadow TEXT, Channels INTEGER, ChannelFormats TEXT)");
    sql.exec("CREATE TABLE Direction (Id INTEGER PRIMARY KEY, Value TEXT)");
    sql.exec("CREATE TABLE Format (Id INTEGER PRIMARY KEY, Name TEXT, Width INTEGER, Height INTEGER, FramesPerSecond TEXT)");
    sql.exec("CREATE TABLE GpiPort (Id INTEGER PRIMARY KEY, RisingEdge INTEGER, Action TEXT)");
    sql.exec("CREATE TABLE GpoPort (Id INTEGER PRIMARY KEY, RisingEdge INTEGER, PulseLengthMillis INTEGER)");
    sql.exec("CREATE TABLE Library (Id INTEGER PRIMARY KEY, Name TEXT, DeviceId INTEGER, TypeId INTEGER, ThumbnailId INTEGER, Timecode TEXT)");
    sql.exec("CREATE TABLE Preset (Id INTEGER PRIMARY KEY, Name TEXT, Value TEXT)");
    sql.exec("CREATE TABLE Thumbnail (Id INTEGER PRIMARY KEY, Data TEXT, Timestamp TEXT, Size TEXT)");
    sql.exec("CREATE TABLE Transition (Id INTEGER PRIMARY KEY, Value TEXT)");
    sql.exec("CREATE TABLE Tween (Id INTEGER PRIMARY KEY, Value TEXT)");
    sql.exec("CREATE TABLE Type (Id INTEGER PRIMARY KEY, Value TEXT)");

    sql.exec("INSERT INTO BlendMode (Value) VALUES('Normal')");
    sql.exec("INSERT INTO BlendMode (Value) VALUES('Lighten')");
    sql.exec("INSERT INTO BlendMode (Value) VALUES('Darken')");
    sql.exec("INSERT INTO BlendMode (Value) VALUES('Multiply')");
    sql.exec("INSERT INTO BlendMode (Value) VALUES('Average')");
    sql.exec("INSERT INTO BlendMode (Value) VALUES('Add')");
    sql.exec("INSERT INTO BlendMode (Value) VALUES('Subtract')");
    sql.exec("INSERT INTO BlendMode (Value) VALUES('Difference')");
    sql.exec("INSERT INTO BlendMode (Value) VALUES('Negation')");
    sql.exec("INSERT INTO BlendMode (Value) VALUES('Exclusion')");
    sql.exec("INSERT INTO BlendMode (Value) VALUES('Screen')");
    sql.exec("INSERT INTO BlendMode (Value) VALUES('Overlay')");
    sql.exec("INSERT INTO BlendMode (Value) VALUES('Soft_Light')");
    sql.exec("INSERT INTO BlendMode (Value) VALUES('Hard_Light')");
    sql.exec("INSERT INTO BlendMode (Value) VALUES('Color_Dodge ')");
    sql.exec("INSERT INTO BlendMode (Value) VALUES('Color_Burn')");
    sql.exec("INSERT INTO BlendMode (Value) VALUES('Linear_Dodge')");
    sql.exec("INSERT INTO BlendMode (Value) VALUES('Linear_Burn')");
    sql.exec("INSERT INTO BlendMode (Value) VALUES('Linear_Light')");
    sql.exec("INSERT INTO BlendMode (Value) VALUES('Vivid_Light')");
    sql.exec("INSERT INTO BlendMode (Value) VALUES('Pin_Light ')");
    sql.exec("INSERT INTO BlendMode (Value) VALUES('Hard_Mix')");
    sql.exec("INSERT INTO BlendMode (Value) VALUES('Reflect')");
    sql.exec("INSERT INTO BlendMode (Value) VALUES('Glow')");
    sql.exec("INSERT INTO BlendMode (Value) VALUES('Phoenix')");
    sql.exec("INSERT INTO BlendMode (Value) VALUES('Contrast')");
    sql.exec("INSERT INTO BlendMode (Value) VALUES('Saturation')");
    sql.exec("INSERT INTO BlendMode (Value) VALUES('Color')");
    sql.exec("INSERT INTO BlendMode (Value) VALUES('Luminosity')");

    sql.exec("INSERT INTO Configuration (Name, Value) VALUES('StartFullscreen', 'false')");
    sql.exec("INSERT INTO Configuration (Name, Value) VALUES('AutoRefreshLibrary', 'false')");
    sql.exec("INSERT INTO Configuration (Name, Value) VALUES('ShowThumbnailTooltip', 'true')");
    sql.exec("INSERT INTO Configuration (Name, Value) VALUES('ReverseOscTime', 'true')");
    sql.exec("INSERT INTO Configuration (Name, Value) VALUES('DisableVideoProgress', 'false')");
    sql.exec("INSERT INTO Configuration (Name, Value) VALUES('DisableInAndOutPoints', 'true')");
    sql.exec("INSERT INTO Configuration (Name, Value) VALUES('RefreshLibraryInterval', '60')");
    sql.exec("INSERT INTO Configuration (Name, Value) VALUES('GpiSerialPort', 'COM1')");
    sql.exec("INSERT INTO Configuration (Name, Value) VALUES('GpiBaudRate', '115200')");
    sql.exec("INSERT INTO Configuration (Name, Value) VALUES('OscPort', '6250')");
#if defined(Q_OS_WIN)
    sql.exec("INSERT INTO Configuration (Name, Value) VALUES('FontSize', '11')");
#else
    sql.exec("INSERT INTO Configuration (Name, Value) VALUES('FontSize', '12')");
#endif

    sql.exec("INSERT INTO Chroma (Key) VALUES('None')");
    sql.exec("INSERT INTO Chroma (Key) VALUES('Red')");
    sql.exec("INSERT INTO Chroma (Key) VALUES('Yellow')");
    sql.exec("INSERT INTO Chroma (Key) VALUES('Green')");
    sql.exec("INSERT INTO Chroma (Key) VALUES('Torquise')");
    sql.exec("INSERT INTO Chroma (Key) VALUES('Blue')");
    sql.exec("INSERT INTO Chroma (Key) VALUES('Magenta')");

#if defined(Q_OS_WIN)
    sql.exec("INSERT INTO Device (Name, Address, Port, Username, Password, Description, Version, Shadow, Channels, ChannelFormats) VALUES('Local CasparCG', '127.0.0.1', 5250, '', '', '', '', 'No', 0, '')");
#endif

    sql.exec("INSERT INTO Direction (Value) VALUES('RIGHT')");
    sql.exec("INSERT INTO Direction (Value) VALUES('LEFT')");

    sql.exec("INSERT INTO Format (Name, Width, Height, FramesPerSecond) VALUES('PAL', 720, 576, '25')");
    sql.exec("INSERT INTO Format (Name, Width, Height, FramesPerSecond) VALUES('NTSC', 720, 486, '29.97')");
    sql.exec("INSERT INTO Format (Name, Width, Height, FramesPerSecond) VALUES('576p2500', 1024, 576, '25')");
    sql.exec("INSERT INTO Format (Name, Width, Height, FramesPerSecond) VALUES('720p2398', 1280, 720, '23.98')");
    sql.exec("INSERT INTO Format (Name, Width, Height, FramesPerSecond) VALUES('720p2400', 1280, 720, '24')");
    sql.exec("INSERT INTO Format (Name, Width, Height, FramesPerSecond) VALUES('720p2500', 1280, 720, '25')");
    sql.exec("INSERT INTO Format (Name, Width, Height, FramesPerSecond) VALUES('720p5000', 1280, 720, '50')");
    sql.exec("INSERT INTO Format (Name, Width, Height, FramesPerSecond) VALUES('720p2997', 1280, 720, '29.97')");
    sql.exec("INSERT INTO Format (Name, Width, Height, FramesPerSecond) VALUES('720p5994', 1280, 720, '59.94')");
    sql.exec("INSERT INTO Format (Name, Width, Height, FramesPerSecond) VALUES('720p3000', 1280, 720, '30')");
    sql.exec("INSERT INTO Format (Name, Width, Height, FramesPerSecond) VALUES('720p6000', 1280, 720, '60')");
    sql.exec("INSERT INTO Format (Name, Width, Height, FramesPerSecond) VALUES('1080p2398', 1920, 1080, '23.98')");
    sql.exec("INSERT INTO Format (Name, Width, Height, FramesPerSecond) VALUES('1080p2400', 1920, 1080, '24')");
    sql.exec("INSERT INTO Format (Name, Width, Height, FramesPerSecond) VALUES('1080i5000', 1920, 1080, '25')");
    sql.exec("INSERT INTO Format (Name, Width, Height, FramesPerSecond) VALUES('1080i5994', 1920, 1080, '59.94')");
    sql.exec("INSERT INTO Format (Name, Width, Height, FramesPerSecond) VALUES('1080i6000', 1920, 1080, '30')");
    sql.exec("INSERT INTO Format (Name, Width, Height, FramesPerSecond) VALUES('1080p2500', 1920, 1080, '25')");
    sql.exec("INSERT INTO Format (Name, Width, Height, FramesPerSecond) VALUES('1080p2997', 1920, 1080, '29.97')");
    sql.exec("INSERT INTO Format (Name, Width, Height, FramesPerSecond) VALUES('1080p3000', 1920, 1080, '30')");
    sql.exec("INSERT INTO Format (Name, Width, Height, FramesPerSecond) VALUES('1080p5000', 1920, 1080, '50')");
    sql.exec("INSERT INTO Format (Name, Width, Height, FramesPerSecond) VALUES('1080p5994', 1920, 1080, '59.94')");
    sql.exec("INSERT INTO Format (Name, Width, Height, FramesPerSecond) VALUES('1080p6000', 1920, 1080, '60')");
    sql.exec("INSERT INTO Format (Name, Width, Height, FramesPerSecond) VALUES('1556p2398', 2048, 1556, '23.98')");
    sql.exec("INSERT INTO Format (Name, Width, Height, FramesPerSecond) VALUES('1556p2400', 2048, 1556, '24')");
    sql.exec("INSERT INTO Format (Name, Width, Height, FramesPerSecond) VALUES('1556p2500', 2048, 1556, '25')");
    sql.exec("INSERT INTO Format (Name, Width, Height, FramesPerSecond) VALUES('2160p2398', 3840, 2160, '23.98')");
    sql.exec("INSERT INTO Format (Name, Width, Height, FramesPerSecond) VALUES('2160p2400', 3840, 2160, '24')");
    sql.exec("INSERT INTO Format (Name, Width, Height, FramesPerSecond) VALUES('2160p2500', 3840, 2160, '25')");
    sql.exec("INSERT INTO Format (Name, Width, Height, FramesPerSecond) VALUES('2160p2997', 3840, 2160, '29.97')");
    sql.exec("INSERT INTO Format (Name, Width, Height, FramesPerSecond) VALUES('2160p3000', 3840, 2160, '30')");

    sql.exec("INSERT INTO GpiPort (Id, RisingEdge, Action) VALUES(0, 1, 'Stop')");
    sql.exec("INSERT INTO GpiPort (Id, RisingEdge, Action) VALUES(1, 1, 'Play')");
    sql.exec("INSERT INTO GpiPort (Id, RisingEdge, Action) VALUES(2, 1, 'Load')");
    sql.exec("INSERT INTO GpiPort (Id, RisingEdge, Action) VALUES(3, 1, 'Next')");
    sql.exec("INSERT INTO GpiPort (Id, RisingEdge, Action) VALUES(4, 1, 'Update')");
    sql.exec("INSERT INTO GpiPort (Id, RisingEdge, Action) VALUES(5, 1, 'Clear')");
    sql.exec("INSERT INTO GpiPort (Id, RisingEdge, Action) VALUES(6, 1, 'ClearVideolayer')");
    sql.exec("INSERT INTO GpiPort (Id, RisingEdge, Action) VALUES(7, 1, 'ClearChannel')");

    sql.exec("INSERT INTO GpoPort (Id, RisingEdge, PulseLengthMillis) VALUES(0, 1, 100)");
    sql.exec("INSERT INTO GpoPort (Id, RisingEdge, PulseLengthMillis) VALUES(1, 1, 100)");
    sql.exec("INSERT INTO GpoPort (Id, RisingEdge, PulseLengthMillis) VALUES(2, 1, 100)");
    sql.exec("INSERT INTO GpoPort (Id, RisingEdge, PulseLengthMillis) VALUES(3, 1, 100)");
    sql.exec("INSERT INTO GpoPort (Id, RisingEdge, PulseLengthMillis) VALUES(4, 1, 100)");
    sql.exec("INSERT INTO GpoPort (Id, RisingEdge, PulseLengthMillis) VALUES(5, 1, 100)");
    sql.exec("INSERT INTO GpoPort (Id, RisingEdge, PulseLengthMillis) VALUES(6, 1, 100)");
    sql.exec("INSERT INTO GpoPort (Id, RisingEdge, PulseLengthMillis) VALUES(7, 1, 100)");

    sql.exec("INSERT INTO Transition (Value) VALUES('CUT')");
    sql.exec("INSERT INTO Transition (Value) VALUES('MIX')");
    sql.exec("INSERT INTO Transition (Value) VALUES('PUSH')");
    sql.exec("INSERT INTO Transition (Value) VALUES('SLIDE')");
    sql.exec("INSERT INTO Transition (Value) VALUES('WIPE')");

    sql.exec("INSERT INTO Tween (Value) VALUES('Linear')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseNone')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseInQuad')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseOutQuad')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseInOutQuad')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseOutInQuad')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseInCubic')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseOutCubic')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseInOutCubic')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseOutInCubic')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseInQuart')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseOutQuart')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseInOutQuart')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseOutInQuart')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseInQuint')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseOutQuint')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseInOutQuint')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseOutInQuint')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseInSine')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseOutSine')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseInOutSine')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseOutInSine')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseInExpo')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseOutExpo')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseInOutExpo')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseOutInExpo')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseInCirc')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseOutCirc')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseInOutCirc')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseOutInCirc')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseInElastic')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseOutElastic')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseInOutElastic')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseOutInElastic')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseInBack')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseOutBack')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseInOutBack')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseOutInBack')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseOutBounce')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseInBounce')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseInOutBounce')");
    sql.exec("INSERT INTO Tween (Value) VALUES('EaseOutInBounce')");

    sql.exec("INSERT INTO Type (Value) VALUES('AUDIO')");
    sql.exec("INSERT INTO Type (Value) VALUES('DATA')");
    sql.exec("INSERT INTO Type (Value) VALUES('MOVIE')");
    sql.exec("INSERT INTO Type (Value) VALUES('STILL')");
    sql.exec("INSERT INTO Type (Value) VALUES('TEMPLATE')");
}

void DatabaseManager::uninitialize()
{
}

void DatabaseManager::updateConfiguration(const ConfigurationModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString query = QString("UPDATE Configuration SET Value = '%1' "
                            "WHERE Name = '%2'").arg(model.getValue()).arg(model.getName());

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QSqlDatabase::database().commit();
}

ConfigurationModel DatabaseManager::getConfigurationByName(const QString& name)
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT c.Id, c.Name, c.Value FROM Configuration c "
                            "WHERE c.Name = '%1'").arg(name);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    sql.first();

    return ConfigurationModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString());
}

QList<FormatModel> DatabaseManager::getFormat()
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT f.Id, f.Name, f.Width, f.Height, f.FramesPerSecond FROM Format f");

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QList<FormatModel> models;
    while (sql.next())
        models.push_back(FormatModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toInt(), sql.value(3).toInt(), sql.value(4).toString()));

    return models;
}

FormatModel DatabaseManager::getFormat(const QString& name)
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT f.Id, f.Name, f.Width, f.Height, f.FramesPerSecond FROM Format f "
                            "WHERE f.Name = '%1'").arg(name);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    sql.first();

    return FormatModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toInt(), sql.value(3).toInt(), sql.value(4).toString());
}

QList<PresetModel> DatabaseManager::getPreset()
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT p.Id, p.Name, p.Value FROM Preset p");

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QList<PresetModel> models;
    while (sql.next())
        models.push_back(PresetModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString()));

    return models;
}

PresetModel DatabaseManager::getPreset(const QString& name)
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT p.Id, p.Name, p.Value FROM Preset p "
                            "WHERE p.Name = '%1'").arg(name);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    sql.first();

    return PresetModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString());
}

QList<PresetModel> DatabaseManager::getPresetByFilter(const QString& filter)
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT p.Id, p.Name, p.Value "
                            "WHERE p.Name LIKE '%%1%' OR p.Value LIKE '%%1%' "
                            "ORDER BY p.Name, p.Id").arg(filter);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QList<PresetModel> models;
    while (sql.next())
        models.push_back(PresetModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString()));

    return models;
}

void DatabaseManager::insertPreset(const PresetModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString query = QString("INSERT INTO Preset (Name, Value) "
                            "VALUES('%1', '%2')").arg(model.getName()).arg(model.getValue());

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QSqlDatabase::database().commit();
}

void DatabaseManager::deletePreset(int id)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString query = QString("DELETE FROM Preset WHERE Id = %1").arg(id);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QSqlDatabase::database().commit();
}

QList<BlendModeModel> DatabaseManager::getBlendMode()
{
    QMutexLocker locker(&mutex);

    QString query("SELECT b.Id, b.Value FROM BlendMode b");

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QList<BlendModeModel> models;
    while (sql.next())
        models.push_back(BlendModeModel(sql.value(0).toInt(), sql.value(1).toString()));

    return models;
}

QList<ChromaModel> DatabaseManager::getChroma()
{
    QMutexLocker locker(&mutex);

    QString query("SELECT c.Id, c.Key FROM Chroma c");

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QList<ChromaModel> models;
    while (sql.next())
        models.push_back(ChromaModel(sql.value(0).toInt(), sql.value(1).toString()));

    return models;
}

QList<DirectionModel> DatabaseManager::getDirection()
{
    QMutexLocker locker(&mutex);

    QString query("SELECT d.Id, d.Value FROM Direction d");

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QList<DirectionModel> models;
    while (sql.next())
        models.push_back(DirectionModel(sql.value(0).toInt(), sql.value(1).toString()));

    return models;
}

QList<TransitionModel> DatabaseManager::getTransition()
{
    QMutexLocker locker(&mutex);

    QString query("SELECT t.Id, t.Value FROM Transition t");

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QList<TransitionModel> models;
    while (sql.next())
        models.push_back(TransitionModel(sql.value(0).toInt(), sql.value(1).toString()));

    return models;
}

QList<TweenModel> DatabaseManager::getTween()
{
    QMutexLocker locker(&mutex);

    QString query("SELECT t.Id, t.Value FROM Tween t");

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QList<TweenModel> models;
    while (sql.next())
        models.push_back(TweenModel(sql.value(0).toInt(), sql.value(1).toString()));

    return models;
}

QList<GpiPortModel> DatabaseManager::getGpiPorts()
{
    QMutexLocker locker(&mutex);

    QString query("SELECT Id, RisingEdge, Action FROM GpiPort ORDER BY Id");

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QList<GpiPortModel> models;
    while (sql.next())
        models.push_back(GpiPortModel(sql.value(0).toInt(), sql.value(1).toInt() == 1, Playout::fromString(sql.value(2).toString())));

    return models;
}

void DatabaseManager::updateGpiPort(const GpiPortModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString query = QString("UPDATE GpiPort SET Action = '%1', RisingEdge = %2 "
                            "WHERE Id = %3").arg(model.getAction()).arg(model.isRisingEdge() ? "1" : "0").arg(model.getPort());

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QSqlDatabase::database().commit();
}

QList<GpoPortModel> DatabaseManager::getGpoPorts()
{
    QMutexLocker locker(&mutex);

    QString query("SELECT Id, RisingEdge, PulseLengthMillis "
                  "FROM GpoPort ORDER BY Id");

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QList<GpoPortModel> models;
    while (sql.next())
        models.push_back(GpoPortModel(sql.value(0).toInt(), sql.value(1).toInt() == 1, sql.value(2).toInt()));

    return models;
}

void DatabaseManager::updateGpoPort(const GpoPortModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString query = QString("UPDATE GpoPort SET PulseLengthMillis = %1, RisingEdge = %2 "
                            "WHERE Id = %3").arg(model.getPulseLengthMillis()).arg(model.isRisingEdge() ? "1" : "0").arg(model.getPort());

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QSqlDatabase::database().commit();
}

QList<TypeModel> DatabaseManager::getType()
{
    QMutexLocker locker(&mutex);

    QString query("SELECT t.Id, t.Value FROM Type t");

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QList<TypeModel> models;
    while (sql.next())
        models.push_back(TypeModel(sql.value(0).toInt(), sql.value(1).toString()));

    return models;
}

TypeModel DatabaseManager::getTypeByValue(const QString& value)
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT t.Id FROM Type t "
                            "WHERE t.Value = '%1'").arg(value);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    sql.first();

    return TypeModel(sql.value(0).toInt(), sql.value(1).toString());
}

QList<DeviceModel> DatabaseManager::getDevice()
{
    QString query("SELECT d.Id, d.Name, d.Address, d.Port, d.Username, d.Password, d.Description, d.Version, d.Shadow, d.Channels, d.ChannelFormats FROM Device d "
                  "ORDER BY d.Name");

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QList<DeviceModel> models;
    while (sql.next())
        models.push_back(DeviceModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toInt(),
                                     sql.value(4).toString(), sql.value(5).toString(), sql.value(6).toString(), sql.value(7).toString(),
                                     sql.value(8).toString(), sql.value(9).toInt(), sql.value(10).toString()));

    return models;
}

DeviceModel DatabaseManager::getDeviceByName(const QString& name)
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT d.Id, d.Name, d.Address, d.Port, d.Username, d.Password, d.Description, d.Version, d.Shadow, d.Channels, d.ChannelFormats FROM Device d "
                            "WHERE d.Name = '%1'").arg(name);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    sql.first();

    return DeviceModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toInt(),
                       sql.value(4).toString(), sql.value(5).toString(), sql.value(6).toString(), sql.value(6).toString(),
                       sql.value(8).toString(), sql.value(9).toInt(), sql.value(10).toString());
}

DeviceModel DatabaseManager::getDeviceByAddress(const QString& address)
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT d.Id, d.Name, d.Address, d.Port, d.Username, d.Password, d.Description, d.Version, d.Shadow, d.Channels, d.ChannelFormats FROM Device d "
                            "WHERE d.Address = '%1'").arg(address);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    sql.first();

    return DeviceModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toInt(),
                       sql.value(4).toString(), sql.value(5).toString(), sql.value(6).toString(), sql.value(6).toString(),
                       sql.value(8).toString(), sql.value(9).toInt(), sql.value(10).toString());
}

void DatabaseManager::insertDevice(const DeviceModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString query = QString("INSERT INTO Device (Name, Address, Port, Username, Password, Description, Version, Shadow, Channels, ChannelFormats) "
                            "VALUES('%1', '%2', %3, '%4', '%5', '%6', '%7', '%8', %9, '%10')")
                            .arg(model.getName()).arg(model.getAddress()).arg(model.getPort()).arg(model.getUsername())
                            .arg(model.getPassword()).arg(model.getDescription()).arg(model.getVersion()).arg(model.getShadow())
                            .arg(model.getChannels()).arg(model.getChannelFormats());

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QSqlDatabase::database().commit();
}

void DatabaseManager::updateDevice(const DeviceModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString query = QString("UPDATE Device SET Name = '%1', Address = '%2', Port = %3, Username = '%4', Password = '%5', Description = '%6', Shadow = '%7' WHERE Id = %8")
                            .arg(model.getName()).arg(model.getAddress()).arg(model.getPort()).arg(model.getUsername()).arg(model.getPassword()).arg(model.getDescription()).arg(model.getShadow()).arg(model.getId());

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QSqlDatabase::database().commit();
}

void DatabaseManager::updateDeviceVersion(const DeviceModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString query = QString("UPDATE Device SET Version = '%1' "
                            "WHERE Address = '%2'").arg(model.getVersion()).arg(model.getAddress());

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QSqlDatabase::database().commit();
}

void DatabaseManager::updateDeviceChannels(const DeviceModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString query = QString("UPDATE Device SET Channels = %1 "
                            "WHERE Address = '%2'").arg(model.getChannels()).arg(model.getAddress());

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QSqlDatabase::database().commit();
}

void DatabaseManager::updateDeviceChannelFormats(const DeviceModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString query = QString("UPDATE Device SET ChannelFormats = '%1' "
                            "WHERE Address = '%2'").arg(model.getChannelFormats()).arg(model.getAddress());

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QSqlDatabase::database().commit();
}

void DatabaseManager::deleteDevice(int id)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString query = QString("DELETE FROM Device WHERE Id = %1").arg(id);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    query = QString("DELETE FROM Thumbnail WHERE Id IN (SELECT l.ThumbnailId FROM Library l WHERE DeviceId = %1)").arg(id);

    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    query = QString("DELETE FROM Library WHERE DeviceId = %1").arg(id);

    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QSqlDatabase::database().commit();
}

QList<LibraryModel> DatabaseManager::getLibraryMedia()
{
    QMutexLocker locker(&mutex);

    QString query("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                  "WHERE  l.DeviceId = d.Id AND l.TypeId = t.Id AND (l.TypeId = 1 OR l.TypeId = 3 OR l.TypeId = 4) "
                  "ORDER BY l.Name, l.DeviceId");

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QList<LibraryModel> models;
    while (sql.next())
        models.push_back(LibraryModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(1).toString(),
                                      sql.value(2).toString(), sql.value(3).toString(), sql.value(4).toInt(),
                                      sql.value(5).toString()));

    return models;
}

QList<LibraryModel> DatabaseManager::getLibraryTemplate()
{
    QMutexLocker locker(&mutex);

    QString query("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                  "WHERE  l.DeviceId = d.Id AND l.TypeId = t.Id AND l.TypeId = 5 "
                  "ORDER BY l.Name, l.DeviceId");

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QList<LibraryModel> models;
    while (sql.next())
        models.push_back(LibraryModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(1).toString(),
                                      sql.value(2).toString(), sql.value(3).toString(), sql.value(4).toInt(),
                                      sql.value(5).toString()));

    return models;
}

QList<LibraryModel> DatabaseManager::getLibraryData()
{
    QMutexLocker locker(&mutex);

    QString query("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                  "WHERE  l.DeviceId = d.Id AND l.TypeId = t.Id AND l.TypeId = 2 "
                  "ORDER BY l.Name, l.DeviceId");

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QList<LibraryModel> models;
    while (sql.next())
        models.push_back(LibraryModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(1).toString(),
                                      sql.value(2).toString(), sql.value(3).toString(), sql.value(4).toInt(),
                                      sql.value(5).toString()));

    return models;
}

QList<LibraryModel> DatabaseManager::getLibraryMediaByFilter(const QString& filter, QList<QString> devices)
{
    QMutexLocker locker(&mutex);

    QString query;

    if (!filter.isEmpty() && devices.isEmpty()) // Filter on all devices.
    {
        query = QString("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                        "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND (l.TypeId = 1 OR l.TypeId = 3 OR l.TypeId = 4) AND l.Name LIKE '%%1%' "
                        "ORDER BY l.Name, l.DeviceId").arg(filter);
    }
    else if (!filter.isEmpty() && !devices.isEmpty()) // Filter specific devices.
    {
        QString address;
        foreach (QString device, devices)
            address += QString("d.Address LIKE '%%1%' OR ").arg(device);

        address = address.mid(0, address.length() - 4); // Remove the last OR.

        query = QString("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                        "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND (l.TypeId = 1 OR l.TypeId = 3 OR l.TypeId = 4) AND l.Name LIKE '%%1%' AND (%2) "
                        "ORDER BY l.Name, l.DeviceId").arg(filter).arg(address);
    }
    else if (filter.isEmpty() && !devices.isEmpty()) // All on specific devices.
    {
        QString address;
        foreach (QString device, devices)
            address += QString("d.Address LIKE '%%1%' OR ").arg(device);

        address = address.mid(0, address.length() - 4); // Remove the last OR.

        query = QString("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                        "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND (l.TypeId = 1 OR l.TypeId = 3 OR l.TypeId = 4) AND (%1) "
                        "ORDER BY l.Name, l.DeviceId").arg(address);
    }

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QList<LibraryModel> models;
    while (sql.next())
        models.push_back(LibraryModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(1).toString(),
                                      sql.value(2).toString(), sql.value(3).toString(), sql.value(4).toInt(),
                                      sql.value(5).toString()));

    return models;
}

QList<LibraryModel> DatabaseManager::getLibraryTemplateByFilter(const QString& filter, QList<QString> devices)
{
    QMutexLocker locker(&mutex);

    QString query;

    if (!filter.isEmpty() && devices.isEmpty()) // Filter on all devices.
    {
        query = QString("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                        "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND l.TypeId = 5 AND (l.Name LIKE '%%1%' OR d.Name LIKE '%%1%' OR d.Address LIKE '%%1%') "
                        "ORDER BY l.Name, l.DeviceId").arg(filter);
    }
    else if (!filter.isEmpty() && !devices.isEmpty()) // Filter specific devices.
    {
        QString address;
        foreach (QString device, devices)
            address += QString("d.Address LIKE '%%1%' OR ").arg(device);

        address = address.mid(0, address.length() - 4); // Remove the last OR.

        query = QString("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                        "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND l.TypeId = 5 AND l.Name LIKE '%%1%' AND (%2) "
                        "ORDER BY l.Name, l.DeviceId").arg(filter).arg(address);
    }
    else if (filter.isEmpty() && !devices.isEmpty()) // All on specific devices.
    {
        QString address;
        foreach (QString device, devices)
            address += QString("d.Address LIKE '%%1%' OR ").arg(device);

        address = address.mid(0, address.length() - 4); // Remove the last OR.

        query = QString("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                        "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND l.TypeId = 5 AND (%1) "
                        "ORDER BY l.Name, l.DeviceId").arg(address);
    }

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QList<LibraryModel> models;
    while (sql.next())
        models.push_back(LibraryModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(1).toString(),
                                      sql.value(2).toString(), sql.value(3).toString(), sql.value(4).toInt(),
                                      sql.value(5).toString()));

    return models;
}

QList<LibraryModel> DatabaseManager::getLibraryDataByFilter(const QString& filter, QList<QString> devices)
{
    QMutexLocker locker(&mutex);

    QString query;

    if (!filter.isEmpty() && devices.isEmpty()) // Filter on all devices.
    {
        query = QString("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                        "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND l.TypeId = 2 AND (l.Name LIKE '%%1%' OR d.Name LIKE '%%1%' OR d.Address LIKE '%%1%') "
                        "ORDER BY l.Name, l.DeviceId").arg(filter);
    }
    else if (!filter.isEmpty() && !devices.isEmpty()) // Filter specific devices.
    {
        QString address;
        foreach (QString device, devices)
            address += QString("d.Address LIKE '%%1%' OR ").arg(device);

        address = address.mid(0, address.length() - 4); // Remove the last OR.

        query = QString("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                        "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND l.TypeId = 2 AND l.Name LIKE '%%1%' AND (%2) "
                        "ORDER BY l.Name, l.DeviceId").arg(filter).arg(address);
    }
    else if (filter.isEmpty() && !devices.isEmpty()) // All on specific devices.
    {
        QString address;
        foreach (QString device, devices)
            address += QString("d.Address LIKE '%%1%' OR ").arg(device);

        address = address.mid(0, address.length() - 4); // Remove the last OR.

        query = QString("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                        "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND l.TypeId = 2 AND (%1) "
                        "ORDER BY l.Name, l.DeviceId").arg(address);
    }

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QList<LibraryModel> models;
    while (sql.next())
        models.push_back(LibraryModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(1).toString(),
                                      sql.value(2).toString(), sql.value(3).toString(), sql.value(4).toInt(),
                                      sql.value(5).toString()));

    return models;
}

QList<LibraryModel> DatabaseManager::getLibraryByDeviceId(int deviceId)
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                            "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND d.Id = %1 "
                            "ORDER BY l.Name, l.DeviceId").arg(deviceId);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QList<LibraryModel> models;
    while (sql.next())
        models.push_back(LibraryModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(1).toString(),
                                      sql.value(2).toString(), sql.value(3).toString(), sql.value(4).toInt(),
                                      sql.value(5).toString()));

    return models;
}

QList<LibraryModel> DatabaseManager::getLibraryMediaByDeviceAddress(const QString& address)
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                            "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND (l.TypeId = 1 OR l.TypeId = 3 OR l.TypeId = 4) AND d.Address = '%1' "
                            "ORDER BY l.Id, l.DeviceId").arg(address);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QList<LibraryModel> models;
    while (sql.next())
        models.push_back(LibraryModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(1).toString(),
                                      sql.value(2).toString(), sql.value(3).toString(), sql.value(4).toInt(),
                                      sql.value(5).toString()));

    return models;
}

QList<LibraryModel> DatabaseManager::getLibraryTemplateByDeviceAddress(const QString& address)
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                            "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND l.TypeId = 5 AND d.Address = '%1' "
                            "ORDER BY l.Id, l.DeviceId").arg(address);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QList<LibraryModel> models;
    while (sql.next())
        models.push_back(LibraryModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(1).toString(),
                                      sql.value(2).toString(), sql.value(3).toString(), sql.value(4).toInt(),
                                      sql.value(5).toString()));

    return models;
}

QList<LibraryModel> DatabaseManager::getLibraryDataByDeviceAddress(const QString& address)
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                            "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND l.TypeId = 2 AND d.Address = '%1' "
                            "ORDER BY l.Id, l.DeviceId").arg(address);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QList<LibraryModel> models;
    while (sql.next())
        models.push_back(LibraryModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(1).toString(),
                                      sql.value(2).toString(), sql.value(3).toString(), sql.value(4).toInt(),
                                      sql.value(5).toString()));

    return models;
}

QList<LibraryModel> DatabaseManager::getLibraryByNameAndDeviceId(const QString& name, int deviceId)
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                            "WHERE  l.Name = '%1' AND l.DeviceId = %2 AND l.DeviceId = d.Id AND l.TypeId = t.Id")
                            .arg(name).arg(deviceId);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QList<LibraryModel> models;
    while (sql.next())
        models.push_back(LibraryModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(1).toString(),
                                      sql.value(2).toString(), sql.value(3).toString(), sql.value(4).toInt(),
                                      sql.value(5).toString()));

    return models;
}

void DatabaseManager::updateLibraryMedia(const QString& address, const QList<LibraryModel>& deleteModels, const QList<LibraryModel>& insertModels)
{
    QMutexLocker locker(&mutex);

    int deviceId = getDeviceByAddress(address).getId();
    QList<TypeModel> typeModels = getType();

    QSqlDatabase::database().transaction();

    QSqlQuery sql;
    if (deleteModels.count() > 0)
    {
        for (int i = 0; i < deleteModels.count(); i++)
        {
            QString query = QString("DELETE FROM Library WHERE Id = %1").arg(deleteModels.at(i).getId());

            if (!sql.exec(query))
               qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());
        }
    }

    if (insertModels.count() > 0)
    {
        int typeId;
        for (int i = 0; i < insertModels.count(); i++)
        {
            if (insertModels.at(i).getType() == "AUDIO")
                typeId = std::find_if(typeModels.begin(), typeModels.end(), TypeModel::ByName("AUDIO"))->getId();
            else if (insertModels.at(i).getType() == "MOVIE")
                typeId = std::find_if(typeModels.begin(), typeModels.end(), TypeModel::ByName("MOVIE"))->getId();
            else if (insertModels.at(i).getType() == "STILL")
                typeId = std::find_if(typeModels.begin(), typeModels.end(), TypeModel::ByName("STILL"))->getId();

            QString query = QString("INSERT INTO Library (Name, DeviceId, TypeId, ThumbnailId, Timecode) VALUES('%1', %2, %3, %4, '%5')")
                                    .arg(insertModels.at(i).getName()).arg(deviceId).arg(typeId).arg(insertModels.at(i).getThumbnailId()).arg(insertModels.at(i).getTimecode());

            if (!sql.exec(query))
               qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());
        }
    }

    QSqlDatabase::database().commit();
}

void DatabaseManager::updateLibraryTemplate(const QString& address, const QList<LibraryModel>& deleteModels, const QList<LibraryModel>& insertModels)
{
    QMutexLocker locker(&mutex);

    int deviceId = getDeviceByAddress(address).getId();
    QList<TypeModel> typeModels = getType();
    int typeId = std::find_if(typeModels.begin(), typeModels.end(), TypeModel::ByName("TEMPLATE"))->getId();

    QSqlDatabase::database().transaction();

    QSqlQuery sql;
    if (deleteModels.count() > 0)
    {
        for (int i = 0; i < deleteModels.count(); i++)
        {
            QString query = QString("DELETE FROM Library WHERE Id = %1").arg(deleteModels.at(i).getId());

            if (!sql.exec(query))
               qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());
        }
    }

    if (insertModels.count() > 0)
    {
        for (int i = 0; i < insertModels.count(); i++)
        {
            QString query = QString("INSERT INTO Library (Name, DeviceId, TypeId, ThumbnailId, Timecode) VALUES('%1', %2, %3, %4, '%5')")
                                    .arg(insertModels.at(i).getName()).arg(deviceId).arg(typeId).arg(insertModels.at(i).getThumbnailId()).arg(insertModels.at(i).getTimecode());

            if (!sql.exec(query))
               qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());
        }
    }

    QSqlDatabase::database().commit();
}

void DatabaseManager::updateLibraryData(const QString& address, const QList<LibraryModel>& deleteModels, const QList<LibraryModel>& insertModels)
{
    QMutexLocker locker(&mutex);

    int deviceId = getDeviceByAddress(address).getId();
    QList<TypeModel> typeModels = getType();

    QSqlDatabase::database().transaction();

    QSqlQuery sql;
    if (deleteModels.count() > 0)
    {
        for (int i = 0; i < deleteModels.count(); i++)
        {
            QString query = QString("DELETE FROM Thumbnail WHERE Id = %1").arg(deleteModels.at(i).getThumbnailId());

            if (!sql.exec(query))
               qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

            query = QString("DELETE FROM Library WHERE Id = %1 AND TypeId = 2").arg(deleteModels.at(i).getId());

            if (!sql.exec(query))
               qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());
        }
    }

    if (insertModels.count() > 0)
    {
        int typeId;
        for (int i = 0; i < insertModels.count(); i++)
        {
            if (insertModels.at(i).getType() == "DATA")
                typeId = std::find_if(typeModels.begin(), typeModels.end(), TypeModel::ByName("DATA"))->getId();

            QString query = QString("INSERT INTO Library (Name, DeviceId, TypeId, ThumbnailId, Timecode) VALUES('%1', %2, %3, %4, '%5')")
                                    .arg(insertModels.at(i).getName()).arg(deviceId).arg(typeId).arg(insertModels.at(i).getThumbnailId()).arg(insertModels.at(i).getTimecode());

            if (!sql.exec(query))
               qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());
        }
    }

    QSqlDatabase::database().commit();
}

void DatabaseManager::deleteLibrary(int deviceId)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString query = QString("DELETE FROM Library WHERE DeviceId = %1").arg(deviceId);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QSqlDatabase::database().commit();
}

ThumbnailModel DatabaseManager::getThumbnailById(int id)
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT t.Id, t.Data, t.Timestamp, t.Size, l.Name, d.Address FROM Thumbnail t, Library l, Device d "
                            "WHERE t.Id = %1 AND l.DeviceId = d.Id AND l.ThumbnailId = t.Id").arg(id);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    sql.first();

    return ThumbnailModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(),
                          sql.value(3).toString(), sql.value(4).toString(), sql.value(5).toString());
}

QList<ThumbnailModel> DatabaseManager::getThumbnailByDeviceAddress(const QString& address)
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT t.Id, t.Data, t.Timestamp, t.Size, l.Name, d.Address FROM Thumbnail t, Library l, Device d "
                            "WHERE d.Address = '%1' AND l.DeviceId = d.Id AND l.ThumbnailId = t.Id").arg(address);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    QList<ThumbnailModel> models;
    while (sql.next())
        models.push_back(ThumbnailModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(),
                                        sql.value(3).toString(), sql.value(4).toString(), sql.value(5).toString()));

    return models;
}

ThumbnailModel DatabaseManager::getThumbnailByNameAndDeviceName(const QString& name, const QString& deviceName)
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT t.Id, t.Data, t.Timestamp, t.Size, l.Name, d.Name, d.Address FROM Thumbnail t, Library l, Device d "
                            "WHERE l.Name = '%1' AND d.Name = '%2' AND l.ThumbnailId = t.Id").arg(name).arg(deviceName);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

    sql.first();

    return ThumbnailModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(),
                          sql.value(3).toString(), sql.value(4).toString(), sql.value(5).toString());
}

void DatabaseManager::updateThumbnail(const ThumbnailModel& model)
{
    QMutexLocker locker(&mutex);

    int deviceId = getDeviceByAddress(model.getAddress()).getId();
    const QList<LibraryModel>& libraryModels = getLibraryByNameAndDeviceId(model.getName(), deviceId);

    QSqlDatabase::database().transaction();

    QSqlQuery sql;
    if (libraryModels.count() > 0)
    {
        for (int i = 0; i < libraryModels.count(); i++)
        {
            const LibraryModel& libraryModel = libraryModels.at(i);
            if (libraryModel.getThumbnailId() > 0)
            {
                QString query = QString("UPDATE Thumbnail SET Data = '%1', Timestamp = '%2', Size = '%3' WHERE Id = %4")
                                        .arg(model.getData()).arg(model.getTimestamp()).arg(model.getSize()).arg(libraryModel.getThumbnailId());

                if (!sql.exec(query))
                   qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());
            }
            else
            {
                QString query = QString("INSERT INTO Thumbnail (Data, Timestamp, Size) VALUES('%1', '%2', '%3')").arg(model.getData()).arg(model.getTimestamp()).arg(model.getSize());

                if (!sql.exec(query))
                   qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());

                query = QString("UPDATE Library SET ThumbnailId = %1 WHERE Id = %2").arg(sql.lastInsertId().toInt()).arg(libraryModel.getId());

                if (!sql.exec(query))
                   qCritical() << QString("Failed to execute: %1, Error: %2").arg(query).arg(sql.lastError().text());
            }
        }
    }

    QSqlDatabase::database().commit();
}
