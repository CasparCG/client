#include "DatabaseManager.h"

#include <algorithm>
#include <stdexcept>

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QMutexLocker>
#include <QtCore/QTime>
#include <QtCore/QVariant>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

void executeUpdate(const QString& query)
{
    QSqlQuery sql;

    if (!sql.exec(query))
        throw std::runtime_error("Failed to execute " + query.toStdString() + ": " + sql.lastError().text().toStdString());
}

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
    sql.exec("CREATE TABLE Device (Id INTEGER PRIMARY KEY, Name TEXT, Address TEXT, Port INTEGER, Username TEXT, Password TEXT, Description TEXT, Version TEXT, Shadow TEXT)");
    sql.exec("CREATE TABLE Direction (Id INTEGER PRIMARY KEY, Value TEXT)");
    sql.exec("CREATE TABLE Format (Id INTEGER PRIMARY KEY, Value TEXT)");
    sql.exec("CREATE TABLE GpiPort (Id INTEGER PRIMARY KEY, RisingEdge INTEGER, Action TEXT)");
    sql.exec("CREATE TABLE GpoPort (Id INTEGER PRIMARY KEY, RisingEdge INTEGER, PulseLengthMillis INTEGER)");
    sql.exec("CREATE TABLE Library (Id INTEGER PRIMARY KEY, Name TEXT, DeviceId INTEGER, TypeId INTEGER)");
    sql.exec("CREATE TABLE Rundown (Id INTEGER PRIMARY KEY, Page TEXT, Name TEXT, Device TEXT, TypeId INTEGER)");
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
    sql.exec("INSERT INTO Configuration (Name, Value) VALUES('FontSize', '11')");
    sql.exec("INSERT INTO Configuration (Name, Value) VALUES('AutoSynchronize', 'false')");
    sql.exec("INSERT INTO Configuration (Name, Value) VALUES('SynchronizeInterval', '60')");
    sql.exec("INSERT INTO Configuration (Name, Value) VALUES('ResolutionWidth', '1920')");
    sql.exec("INSERT INTO Configuration (Name, Value) VALUES('ResolutionHeight', '1080')");
    sql.exec("INSERT INTO Configuration (Name, Value) VALUES('GpiSerialPort', 'COM1')");
    sql.exec("INSERT INTO Configuration (Name, Value) VALUES('GpiBaudRate', '115200')");

    sql.exec("INSERT INTO Direction (Value) VALUES('RIGHT')");
    sql.exec("INSERT INTO Direction (Value) VALUES('LEFT')");

    sql.exec("INSERT INTO Format (Value) VALUES('PAL')");
    sql.exec("INSERT INTO Format (Value) VALUES('NTSC')");
    sql.exec("INSERT INTO Format (Value) VALUES('576p2500')");
    sql.exec("INSERT INTO Format (Value) VALUES('720p2398')");
    sql.exec("INSERT INTO Format (Value) VALUES('720p2400')");
    sql.exec("INSERT INTO Format (Value) VALUES('720p2500')");
    sql.exec("INSERT INTO Format (Value) VALUES('720p5000')");
    sql.exec("INSERT INTO Format (Value) VALUES('720p2997')");
    sql.exec("INSERT INTO Format (Value) VALUES('720p5994')");
    sql.exec("INSERT INTO Format (Value) VALUES('720p3000')");
    sql.exec("INSERT INTO Format (Value) VALUES('720p6000')");
    sql.exec("INSERT INTO Format (Value) VALUES('1080p2398')");
    sql.exec("INSERT INTO Format (Value) VALUES('1080p2400')");
    sql.exec("INSERT INTO Format (Value) VALUES('1080i5000')");
    sql.exec("INSERT INTO Format (Value) VALUES('1080i5994')");
    sql.exec("INSERT INTO Format (Value) VALUES('1080i6000')");
    sql.exec("INSERT INTO Format (Value) VALUES('1080p2500')");
    sql.exec("INSERT INTO Format (Value) VALUES('1080p2997')");
    sql.exec("INSERT INTO Format (Value) VALUES('1080p3000')");
    sql.exec("INSERT INTO Format (Value) VALUES('1080p5000')");
    sql.exec("INSERT INTO Format (Value) VALUES('1080p5994')");
    sql.exec("INSERT INTO Format (Value) VALUES('1080p6000')");

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
    sql.exec("INSERT INTO Type (Value) VALUES('SCRIPT')");
    sql.exec("INSERT INTO Type (Value) VALUES('STILL')");
    sql.exec("INSERT INTO Type (Value) VALUES('TEMPLATE')");
    sql.exec("INSERT INTO Type (Value) VALUES('FILERECORDER')");
    sql.exec("INSERT INTO Type (Value) VALUES('DECKLINKINPUT')");
    sql.exec("INSERT INTO Type (Value) VALUES('GPIOUTPUT')");
    sql.exec("INSERT INTO Type (Value) VALUES('BLENDMODE')");
    sql.exec("INSERT INTO Type (Value) VALUES('BRIGHTNESS')");
    sql.exec("INSERT INTO Type (Value) VALUES('CONTRAST')");
    sql.exec("INSERT INTO Type (Value) VALUES('CROP')");
    sql.exec("INSERT INTO Type (Value) VALUES('GEOMETRY')");
    sql.exec("INSERT INTO Type (Value) VALUES('GRID')");
    sql.exec("INSERT INTO Type (Value) VALUES('KEYER')");
    sql.exec("INSERT INTO Type (Value) VALUES('LEVELS')");
    sql.exec("INSERT INTO Type (Value) VALUES('OPACITY')");
    sql.exec("INSERT INTO Type (Value) VALUES('SATURATION')");
    sql.exec("INSERT INTO Type (Value) VALUES('VOLUME')");
    sql.exec("INSERT INTO Type (Value) VALUES('COMMIT')");
    sql.exec("INSERT INTO Type (Value) VALUES('IMAGESCROLLER')");
}

void DatabaseManager::updateConfiguration(const ConfigurationModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString query("UPDATE Configuration SET Value = ':value' "
                  "WHERE Name = ':name'");
    query.replace(QRegExp(":name"), model.getName());
    query.replace(QRegExp(":value"), model.getValue());

    executeUpdate(query);

    QSqlDatabase::database().commit();
}

ConfigurationModel DatabaseManager::getConfigurationByName(const QString& name)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;

    QString query("SELECT c.Id, c.Name, c.Value FROM Configuration c "
                  "WHERE c.Name = ':name'");
    query.replace(QRegExp(":name"), name);

    sql.exec(query);
    sql.first();

    return ConfigurationModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString());
}

QList<BlendModeModel> DatabaseManager::getBlendMode()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    QList<BlendModeModel> models;

    QString query("SELECT b.Id, b.Value FROM BlendMode b");

    sql.exec(query);
    while (sql.next())
        models.push_back(BlendModeModel(sql.value(0).toInt(), sql.value(1).toString()));

    return models;
}

QList<DirectionModel> DatabaseManager::getDirection()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    QList<DirectionModel> models;

    QString query("SELECT d.Id, d.Value FROM Direction d");

    sql.exec(query);
    while (sql.next())
        models.push_back(DirectionModel(sql.value(0).toInt(), sql.value(1).toString()));

    return models;
}

QList<FormatModel> DatabaseManager::getFormat()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    QList<FormatModel> models;

    QString query("SELECT f.Id, f.Value FROM Format f");

    sql.exec(query);
    while (sql.next())
        models.push_back(FormatModel(sql.value(0).toInt(), sql.value(1).toString()));

    return models;
}

QList<TransitionModel> DatabaseManager::getTransition()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    QList<TransitionModel> models;

    QString query("SELECT t.Id, t.Value FROM Transition t");

    sql.exec(query);
    while (sql.next())
        models.push_back(TransitionModel(sql.value(0).toInt(), sql.value(1).toString()));

    return models;
}

QList<TweenModel> DatabaseManager::getTween()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    QList<TweenModel> models;

    QString query("SELECT t.Id, t.Value FROM Tween t");

    sql.exec(query);
    while (sql.next())
        models.push_back(TweenModel(sql.value(0).toInt(), sql.value(1).toString()));

    return models;
}

QList<GpiPortModel> DatabaseManager::getGpiPorts()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    QList<GpiPortModel> models;

    QString query("SELECT Id, RisingEdge, Action FROM GpiPort ORDER BY Id");

    sql.exec(query);
    while (sql.next())
        models.push_back(GpiPortModel(
                sql.value(0).toInt(),
                sql.value(1).toInt() == 1,
                Playout::fromString(sql.value(2).toString())));

    return models;
}

void DatabaseManager::updateGpiPort(const GpiPortModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString query("UPDATE GpiPort "
                  "SET Action = ':action', "
                  "    RisingEdge = :risingEdge "
                  "WHERE Id = :input");
    query.replace(QRegExp(":input"), QString("%1").arg(model.getPort()));
    query.replace(QRegExp(":risingEdge"), model.isRisingEdge() ? "1" : "0");
    query.replace(QRegExp(":action"), Playout::toString(model.getAction()));

    executeUpdate(query);

    QSqlDatabase::database().commit();
}

QList<GpoPortModel> DatabaseManager::getGpoPorts()
{
    QMutexLocker locker(&mutex);
    QSqlQuery sql;
    QList<GpoPortModel> models;

    QString query("SELECT Id, RisingEdge, PulseLengthMillis "
                  "FROM GpoPort ORDER BY Id");

    sql.exec(query);
    while (sql.next())
        models.push_back(GpoPortModel(
                sql.value(0).toInt(),
                sql.value(1).toInt() == 1,
                sql.value(2).toInt()));

    return models;
}

void DatabaseManager::updateGpoPort(const GpoPortModel& model)
{
    QMutexLocker locker(&mutex);
    QSqlDatabase::database().transaction();

    QString query("UPDATE GpoPort "
                  "SET PulseLengthMillis = :pulseLengthMillis, "
                  "    RisingEdge = :risingEdge "
                  "WHERE Id = :output");

    query.replace(QRegExp(":output"), QString("%1").arg(model.getPort()));
    query.replace(QRegExp(":risingEdge"), model.isRisingEdge() ? "1" : "0");
    query.replace(QRegExp(":pulseLengthMillis"),
                  QString("%1").arg(model.getPulseLengthMillis()));

    executeUpdate(query);

    QSqlDatabase::database().commit();
}

QList<TypeModel> DatabaseManager::getType()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    QList<TypeModel> models;

    QString query("SELECT t.Id, t.Value FROM Type t");

    sql.exec(query);
    while (sql.next())
        models.push_back(TypeModel(sql.value(0).toInt(), sql.value(1).toString()));

    return models;
}

TypeModel DatabaseManager::getTypeByValue(const QString& value)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;

    QString query("SELECT t.Id FROM Type t "
                  "WHERE t.Value = ':value'");
    query.replace(QRegExp(":value"), value);

    sql.exec(query);
    sql.first();

    return TypeModel(sql.value(0).toInt(), sql.value(1).toString());
}

QList<RundownModel> DatabaseManager::getRundown()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    QList<RundownModel> models;

    QString query("SELECT r.Id, r.Page, r.Name, r.Device, t.Value FROM Rundown r, Type t "
                  "WHERE r.TypeId = t.Id "
                  "ORDER BY r.Id");

    sql.exec(query);
    while (sql.next())
        models.push_back(RundownModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toString(), sql.value(4).toString()));

    return models;
}

QList<RundownModel> DatabaseManager::getRundownByFilter(const QString& filter)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    QList<RundownModel> models;

    QString query("SELECT r.Id, r.Page, r.Name, r.Device, t.Value FROM Rundown r, Type t "
                  "WHERE r.TypeId = t.Id AND (r.Page LIKE '%:filter%' OR r.Name LIKE '%:filter%' OR r.Device LIKE '%:filter%' OR t.Value LIKE '%:filter%') "
                  "ORDER BY r.Id");
    query.replace(QRegExp(":filter"), filter);

    sql.exec(query);
    while (sql.next())
        models.push_back(RundownModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toString(), sql.value(6).toString()));

    return models;
}

RundownModel DatabaseManager::getRundownById(int id)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;

    QString query("SELECT r.Id, r.Page, r.Name, r.Device, t.Value FROM Rundown r, Type t "
                  "WHERE r.TypeId = t.Id AND r.Id=:id");
    query.replace(QRegExp(":id"), QString("%1").arg(id));

    sql.exec(query);
    sql.first();

    return RundownModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toString(), sql.value(6).toString());
}

void DatabaseManager::deleteRundown(int id)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString query("DELETE FROM Rundown WHERE Id = :id");
    query.replace(QRegExp(":id"), QString("%1").arg(id));

    executeUpdate(query);

    QSqlDatabase::database().commit();
}

QList<DeviceModel> DatabaseManager::getDevice()
{
    QSqlQuery sql;
    QList<DeviceModel> models;

    QString query("SELECT d.Id, d.Name, d.Address, d.Port, d.Username, d.Password, d.Description, d.Version, d.Shadow FROM Device d "
                  "ORDER BY d.Name");

    sql.exec(query);
    while (sql.next())
        models.push_back(DeviceModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toInt(), sql.value(4).toString(),
                                     sql.value(5).toString(), sql.value(6).toString(), sql.value(6).toString(), sql.value(8).toString()));

    return models;
}

GpiDeviceModel DatabaseManager::getGpiDevice()
{
    return GpiDeviceModel(
            getConfigurationByName("GpiSerialPort").getValue(),
            getConfigurationByName("GpiBaudRate").getValue().toInt());
}

void DatabaseManager::updateGpiDevice(const GpiDeviceModel& model)
{
    updateConfiguration(
            ConfigurationModel(-1, "GpiSerialPort", model.getSerialPort()));
    updateConfiguration(ConfigurationModel(
            -1, "GpiBaudRate", QString("%1").arg(model.getBaudRate())));
}

DeviceModel DatabaseManager::getDeviceById(int id)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;

    QString query("SELECT d.Id, d.Name, d.Address, d.Port, d.Username, d.Password, d.Description, d.Version, d.Shadow FROM Device d "
                  "WHERE d.Id = :id");
    query.replace(QRegExp(":id"), QString("%1").arg(id));

    sql.exec(query);
    sql.first();

    return DeviceModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toInt(), sql.value(4).toString(),
                       sql.value(5).toString(), sql.value(6).toString(), sql.value(6).toString(), sql.value(8).toString());
}

DeviceModel DatabaseManager::getDeviceByName(const QString& name)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;

    QString query("SELECT d.Id, d.Name, d.Address, d.Port, d.Username, d.Password, d.Description, d.Version, d.Shadow FROM Device d WHERE d.Name = ':name'");
    query.replace(QRegExp(":name"), QString("%1").arg(name));

    sql.exec(query);
    sql.first();

    return DeviceModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toInt(), sql.value(4).toString(),
                       sql.value(5).toString(), sql.value(6).toString(), sql.value(6).toString(), sql.value(8).toString());
}

DeviceModel DatabaseManager::getDeviceByAddress(const QString& address)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;

    QString query("SELECT d.Id, d.Name, d.Address, d.Port, d.Username, d.Password, d.Description, d.Version, d.Shadow FROM Device d WHERE d.Address = ':address'");
    query.replace(QRegExp(":address"), QString("%1").arg(address));

    sql.exec(query);
    sql.first();

    return DeviceModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toInt(), sql.value(4).toString(),
                       sql.value(5).toString(), sql.value(6).toString(), sql.value(6).toString(), sql.value(8).toString());
}

void DatabaseManager::insertDevice(const DeviceModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString query("INSERT INTO Device (Name, Address, Port, Username, Password, Description, Version, Shadow) "
                  "VALUES(':name', ':address', :port, ':username', ':password', ':description', ':version', ':shadow')");
    query.replace(QRegExp(":name"), model.getName());
    query.replace(QRegExp(":address"), model.getAddress());
    query.replace(QRegExp(":port"), QString("%1").arg(model.getPort()));
    query.replace(QRegExp(":username"), model.getUsername());
    query.replace(QRegExp(":password"), model.getPassword());
    query.replace(QRegExp(":description"), model.getDescription());
    query.replace(QRegExp(":version"), model.getVersion());
    query.replace(QRegExp(":shadow"), model.getShadow());

    executeUpdate(query);

    QSqlDatabase::database().commit();
}

void DatabaseManager::deleteDevice(int id)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString deviceQuery("DELETE FROM Device WHERE Id = :id");
    deviceQuery.replace(QRegExp(":id"), QString("%1").arg(id));

    executeUpdate(deviceQuery);

    QString contentQuery("DELETE FROM Library WHERE DeviceId = :id");
    contentQuery.replace(QRegExp(":id"), QString("%1").arg(id));

    executeUpdate(contentQuery);

    QSqlDatabase::database().commit();
}

QList<LibraryModel> DatabaseManager::getLibraryMedia()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    QList<LibraryModel> models;

    QString query("SELECT l.Id, l.Name, d.Name, t.Value FROM Library l, Device d, Type t "
                  "WHERE  l.DeviceId = d.Id AND l.TypeId = t.Id AND (l.TypeId = 1 OR l.TypeId = 3 OR l.TypeId = 5) "
                  "ORDER BY l.Name, l.DeviceId");

    sql.exec(query);
    while (sql.next())
        models.push_back(LibraryModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toString()));

    return models;
}

QList<LibraryModel> DatabaseManager::getLibraryTemplate()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    QList<LibraryModel> models;

    QString query("SELECT l.Id, l.Name, d.Name, t.Value FROM Library l, Device d, Type t "
                  "WHERE  l.DeviceId = d.Id AND l.TypeId = t.Id AND l.TypeId = 6 "
                  "ORDER BY l.Name, l.DeviceId");

    sql.exec(query);
    while (sql.next())
        models.push_back(LibraryModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toString()));

    return models;
}

QList<LibraryModel> DatabaseManager::getLibraryMediaByFilter(const QString& filter)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    QList<LibraryModel> models;

    QString query("SELECT l.Id, l.Name, d.Name, d.Address, t.Value FROM Library l, Device d, Type t "
                  "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND (l.TypeId = 1 OR l.TypeId = 3 OR l.TypeId = 5) AND (l.Name LIKE '%:filter%' OR d.Name LIKE '%:filter%' OR d.Address LIKE '%:filter%') "
                  "ORDER BY l.Name, l.DeviceId");
    query.replace(QRegExp(":filter"), filter);

    sql.exec(query);
    while (sql.next())
        models.push_back(LibraryModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(4).toString()));

    return models;
}

QList<LibraryModel> DatabaseManager::getLibraryTemplateByFilter(const QString& filter)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    QList<LibraryModel> models;

    QString query("SELECT l.Id, l.Name, d.Name, d.Address, t.Value FROM Library l, Device d, Type t "
                  "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND l.TypeId = 6 AND (l.Name LIKE '%:filter%' OR d.Name LIKE '%:filter%' OR d.Address LIKE '%:filter%') "
                  "ORDER BY l.Name, l.DeviceId");
    query.replace(QRegExp(":filter"), filter);

    sql.exec(query);
    while (sql.next())
        models.push_back(LibraryModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(4).toString()));

    return models;
}

QList<LibraryModel> DatabaseManager::getLibraryByDeviceId(int deviceId)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    QList<LibraryModel> models;

    QString query("SELECT l.Id, l.Name, d.Name, t.Value FROM Library l, Device d, Type t "
                  "WHERE d.Id = :deviceId AND t.Id = l.TypeId "
                  "ORDER BY l.Name, l.DeviceId");
    query.replace(QRegExp(":deviceId"), QString("%1").arg(deviceId));

    sql.exec(query);
    while (sql.next())
        models.push_back(LibraryModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toString()));

    return models;
}

QList<LibraryModel> DatabaseManager::getLibraryMediaByDeviceAddress(const QString& address)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    QList<LibraryModel> models;

    QString query("SELECT l.Id, l.Name, d.Name, t.Value FROM Library l, Device d, Type t "
                  "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND (l.TypeId = 1 OR l.TypeId = 3 OR l.TypeId = 5) AND d.Address = ':address' "
                  "ORDER BY l.Id, l.DeviceId");
    query.replace(QRegExp(":address"), address);

    sql.exec(query);
    while (sql.next())
        models.push_back(LibraryModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toString()));

    return models;
}

QList<LibraryModel> DatabaseManager::getLibraryTemplateByDeviceAddress(const QString& address)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    QList<LibraryModel> models;

    QString query("SELECT l.Id, l.Name, d.Name, t.Value FROM Library l, Device d, Type t "
                  "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND l.TypeId = 6 AND d.Address = ':address' "
                  "ORDER BY l.Id, l.DeviceId");
    query.replace(QRegExp(":address"), address);

    sql.exec(query);
    while (sql.next())
        models.push_back(LibraryModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toString()));

    return models;
}

QList<LibraryModel> DatabaseManager::getLibraryDataByDeviceAddress(const QString& address)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    QList<LibraryModel> models;

    QString query("SELECT l.Id, l.Name, d.Name, t.Value FROM Library l, Device d, Type t "
                  "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND l.TypeId = 2 AND d.Address = ':address' "
                  "ORDER BY l.Id, l.DeviceId");
    query.replace(QRegExp(":address"), address);

    sql.exec(query);
    while (sql.next())
        models.push_back(LibraryModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toString()));

    return models;
}

void DatabaseManager::updateLibraryMedia(const QString& address, const QList<LibraryModel>& deleteModels, const QList<LibraryModel>& insertModels)
{
    QMutexLocker locker(&mutex);

    QTime time;
    time.start();

    QSqlQuery sql;

    int deviceId = getDeviceByAddress(address).getId();
    QList<TypeModel> typeModels = getType();

    QSqlDatabase::database().transaction();

    if (deleteModels.count() > 0)
    {
        for (int i = 0; i < deleteModels.count(); i++)
            sql.exec(QString("DELETE FROM Library WHERE Id = %1").arg(deleteModels.at(i).getId()));
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

            sql.exec(QString("INSERT INTO Library (Name, DeviceId, TypeId) VALUES('%1', %2, %3)").arg(insertModels.at(i).getName()).arg(deviceId).arg(typeId));
        }
    }

    QSqlDatabase::database().commit();

    qDebug() << QString("DatabaseManager::updateLibraryMedia: %1 msec").arg(time.elapsed());
}

void DatabaseManager::updateLibraryTemplate(const QString& address, const QList<LibraryModel>& deleteModels, const QList<LibraryModel>& insertModels)
{
    QMutexLocker locker(&mutex);

    QTime time;
    time.start();

    QSqlQuery sql;

    int deviceId = getDeviceByAddress(address).getId();
    QList<TypeModel> typeModels = getType();
    int typeId = std::find_if(typeModels.begin(), typeModels.end(), TypeModel::ByName("TEMPLATE"))->getId();

    QSqlDatabase::database().transaction();

    if (deleteModels.count() > 0)
    {
        for (int i = 0; i < deleteModels.count(); i++)
            sql.exec(QString("DELETE FROM Library WHERE Id = %1").arg(deleteModels.at(i).getId()));
    }

    if (insertModels.count() > 0)
    {
        for (int i = 0; i < insertModels.count(); i++)
            sql.exec(QString("INSERT INTO Library (Name, DeviceId, TypeId) VALUES('%1', %2 ,%3)").arg(insertModels.at(i).getName()).arg(deviceId).arg(typeId));
    }

    QSqlDatabase::database().commit();

    qDebug() << QString("DatabaseManager::updateLibraryTemplate: %1 msec").arg(time.elapsed());
}

void DatabaseManager::updateLibraryData(const QString& address, const QList<LibraryModel>& deleteModels, const QList<LibraryModel>& insertModels)
{
    QMutexLocker locker(&mutex);

    QTime time;
    time.start();

    QSqlQuery sql;

    int deviceId = getDeviceByAddress(address).getId();
    QList<TypeModel> typeModels = getType();

    QSqlDatabase::database().transaction();

    if (deleteModels.count() > 0)
    {
        for (int i = 0; i < deleteModels.count(); i++)
            sql.exec(QString("DELETE FROM Library WHERE Id = %1 AND TypeId = 2").arg(deleteModels.at(i).getId()));
    }

    if (insertModels.count() > 0)
    {
        int typeId;
        for (int i = 0; i < insertModels.count(); i++)
        {
            if (insertModels.at(i).getType() == "DATA")
                typeId = std::find_if(typeModels.begin(), typeModels.end(), TypeModel::ByName("DATA"))->getId();

            sql.exec(QString("INSERT INTO Library (Name, DeviceId, TypeId) VALUES('%1', %2 ,%3)").arg(insertModels.at(i).getName()).arg(deviceId).arg(typeId));
        }
    }

    QSqlDatabase::database().commit();

    qDebug() << QString("DatabaseManager::updateLibraryData: %1 msec").arg(time.elapsed());
}

void DatabaseManager::deleteLibrary(int deviceId)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString query("DELETE FROM Library WHERE DeviceId = :deviceId");
    query.replace(QRegExp(":deviceId"), QString("%1").arg(deviceId));

    executeUpdate(query);

    QSqlDatabase::database().commit();
}
