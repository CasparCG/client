#include "DatabaseManager.h"

#include "Version.h"

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QMutexLocker>
#include <QtCore/QTime>
#include <QtCore/QVariant>

#include <QtSql/QSqlDriver>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

Q_GLOBAL_STATIC(DatabaseManager, databaseManager)

DatabaseManager::DatabaseManager()
    : mutex()
{
}

DatabaseManager& DatabaseManager::getInstance()
{
    return *databaseManager();
}

void DatabaseManager::initialize()
{
    QMutexLocker locker(&mutex);

    if (QSqlDatabase::database().tables().count() == 0)
        createDatabase();
    else
        upgradeDatabase();
}

void DatabaseManager::createDatabase()
{
    QFile file(":/Scripts/Sql/Schema.sql");
    if (file.open(QFile::ReadOnly))
    {
        QStringList queries = QString(file.readAll()).split(";");

        file.close();

        QSqlQuery sql;
        foreach (QString query, queries)
        {
            if (query.trimmed().isEmpty())
                continue;

            if (!sql.exec(query))
                qFatal("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));
        }

#if defined(Q_OS_WIN)
        if (!sql.exec("INSERT INTO Device (Name, Address, Port, Username, Password, Description, Version, Shadow, Channels, ChannelFormats, PreviewChannel, LockedChannel) VALUES('Localhost', '127.0.0.1', 5250, '', '', '', '', 'No', 0, '', 0, 0)"))
            qFatal("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));
#endif

        sql.prepare("UPDATE Configuration SET Value = :Value "
                    "WHERE Name = 'DatabaseVersion'");
        sql.bindValue(":Value", DATABASE_VERSION);

        if (!sql.exec())
            qFatal("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));
    }
}

void DatabaseManager::upgradeDatabase()
{
    QSqlQuery sql;
    if (!sql.exec("SELECT c.Id, c.Name, c.Value FROM Configuration c WHERE c.Name = 'DatabaseVersion'"))
       qFatal("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    sql.first();

    int version = sql.value(2).toInt();
    while (version + 1 <= QString("%1").arg(DATABASE_VERSION).toInt())
    {
        QFile file(QString(":/Scripts/Sql/ChangeScript-%1.sql").arg(version + 1));
        if (file.open(QFile::ReadOnly))
        {
            QStringList queries = QString(file.readAll()).split(";");

            file.close();

            foreach(const QString& query, queries)
            {
                 if (query.trimmed().isEmpty())
                     continue;

                 if (!sql.exec(query))
                    qFatal("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));
            }

            sql.prepare("UPDATE Configuration SET Value = :Value "
                        "WHERE Name = 'DatabaseVersion'");
            sql.bindValue(":Value", version + 1);

            if (!sql.exec())
                qFatal("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

            qDebug("Successfully updated to ChangeScript-%d", version + 1);
        }

        version++;
    }
}

void DatabaseManager::updateConfiguration(const ConfigurationModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QSqlQuery sql;
    sql.prepare("UPDATE Configuration SET Value = :Value "
                "WHERE Name = :Name");
    sql.bindValue(":Value", model.getValue());
    sql.bindValue(":Name", model.getName());

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QSqlDatabase::database().commit();
}

ConfigurationModel DatabaseManager::getConfigurationByName(const QString& name)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    sql.prepare("SELECT c.Id, c.Name, c.Value FROM Configuration c "
                "WHERE c.Name = :Name");
    sql.bindValue(":Name", name);

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    sql.first();

    return ConfigurationModel(sql.value("Id").toInt(), sql.value("Name").toString(), sql.value("Value").toString());
}

QList<FormatModel> DatabaseManager::getFormat()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    if (!sql.exec("SELECT f.Id, f.Name, f.Width, f.Height, f.FramesPerSecond FROM Format f"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<FormatModel> models;
    while (sql.next())
        models.push_back(FormatModel(sql.value("Id").toInt(), sql.value("Name").toString(), sql.value("Width").toInt(),
                                     sql.value("Height").toInt(), sql.value("FramesPerSecond").toString()));

    return models;
}

FormatModel DatabaseManager::getFormat(const QString& name)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    sql.prepare("SELECT f.Id, f.Name, f.Width, f.Height, f.FramesPerSecond FROM Format f "
                "WHERE f.Name = :Name");
    sql.bindValue(":Name", name);

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    sql.first();

    return FormatModel(sql.value("Id").toInt(), sql.value("Name").toString(), sql.value("Width").toInt(),
                       sql.value("Height").toInt(), sql.value("FramesPerSecond").toString());
}

QList<QString> DatabaseManager::getOpenRecent()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    if (!sql.exec("SELECT o.Id, o.Value FROM OpenRecent o ORDER BY o.Id DESC"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<QString> values;
    while (sql.next())
        values.push_back(sql.value("Value").toString());

    return values;
}

void DatabaseManager::insertOpenRecent(const QString& path)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QSqlQuery sql;
    sql.prepare("INSERT INTO OpenRecent (Value) "
                "VALUES(:Value)");
    sql.bindValue(":Value", path);

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    if (!sql.exec("SELECT Count(*) FROM OpenRecent"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    if (sql.first() && sql.value("Id").toInt() > 10)
    {
        if (!sql.exec("DELETE FROM OpenRecent WHERE Id IN (SELECT min(Id) FROM OpenRecent)"))
           qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));
    }

    QSqlDatabase::database().commit();
}

void DatabaseManager::deleteOpenRecent()
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QSqlQuery sql;
    if (!sql.exec("DELETE FROM OpenRecent"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QSqlDatabase::database().commit();
}

QList<PresetModel> DatabaseManager::getPreset()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    if (!sql.exec("SELECT p.Id, p.Name, p.Value FROM Preset p"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<PresetModel> models;
    while (sql.next())
        models.push_back(PresetModel(sql.value("Id").toInt(), sql.value("Name").toString(), sql.value("Value").toString()));

    return models;
}

PresetModel DatabaseManager::getPreset(const QString& name)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    sql.prepare("SELECT p.Id, p.Name, p.Value FROM Preset p "
                "WHERE p.Name = :Name");
    sql.bindValue(":Name", name);

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    sql.first();

    return PresetModel(sql.value("Id").toInt(), sql.value("Name").toString(), sql.value("Value").toString());
}

QList<PresetModel> DatabaseManager::getPresetByFilter(const QString& filter)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    sql.prepare("SELECT p.Id, p.Name, p.Value FROM Preset p "
                "WHERE p.Name LIKE :Name "
                "ORDER BY p.Name, p.Id");
    sql.bindValue(":Name", QString("%%1%").arg(filter));

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<PresetModel> models;
    while (sql.next())
        models.push_back(PresetModel(sql.value("Id").toInt(), sql.value("Name").toString(), sql.value("Value").toString()));

    return models;
}

void DatabaseManager::insertPreset(const PresetModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QSqlQuery sql;
    sql.prepare("INSERT INTO Preset (Name, Value) "
                "VALUES(:Name, :Value)");
    sql.bindValue(":Name", model.getName());
    sql.bindValue(":Value", model.getValue());

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QSqlDatabase::database().commit();
}

void DatabaseManager::deletePreset(int id)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString query = QString("DELETE FROM Preset WHERE Id = %1").arg(id);

    QSqlQuery sql;
    sql.prepare("DELETE FROM Preset WHERE Id = :Id");
    sql.bindValue(":Id", id);

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QSqlDatabase::database().commit();
}

QList<BlendModeModel> DatabaseManager::getBlendMode()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    if (!sql.exec("SELECT b.Id, b.Value FROM BlendMode b"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<BlendModeModel> models;
    while (sql.next())
        models.push_back(BlendModeModel(sql.value("Id").toInt(), sql.value("Value").toString()));

    return models;
}

QList<ChromaModel> DatabaseManager::getChroma()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    if (!sql.exec("SELECT c.Id, c.Value FROM Chroma c"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<ChromaModel> models;
    while (sql.next())
        models.push_back(ChromaModel(sql.value("Id").toInt(), sql.value("Value").toString()));

    return models;
}

QList<DirectionModel> DatabaseManager::getDirection()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    if (!sql.exec("SELECT d.Id, d.Value FROM Direction d"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<DirectionModel> models;
    while (sql.next())
        models.push_back(DirectionModel(sql.value("Id").toInt(), sql.value("Value").toString()));

    return models;
}

QList<TransitionModel> DatabaseManager::getTransition()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    if (!sql.exec("SELECT t.Id, t.Value FROM Transition t"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<TransitionModel> models;
    while (sql.next())
        models.push_back(TransitionModel(sql.value("Id").toInt(), sql.value("Value").toString()));

    return models;
}

QList<TweenModel> DatabaseManager::getTween()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    if (!sql.exec("SELECT t.Id, t.Value FROM Tween t"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<TweenModel> models;
    while (sql.next())
        models.push_back(TweenModel(sql.value("Id").toInt(), sql.value("Value").toString()));

    return models;
}

QList<OscOutputModel> DatabaseManager::getOscOutput()
{
    QSqlQuery sql;
    if (!sql.exec("SELECT o.Id, o.Name, o.Address, o.Port, o.Description FROM OscOutput o ORDER BY o.Name"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<OscOutputModel> models;
    while (sql.next())
        models.push_back(OscOutputModel(sql.value("Id").toInt(), sql.value("Name").toString(), sql.value("Address").toString(),
                                        sql.value("Port").toInt(), sql.value("Description").toString()));

    return models;
}

void DatabaseManager::insertOscOutput(const OscOutputModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QSqlQuery sql;
    sql.prepare("INSERT INTO OscOutput (Name, Address, Port, Description) "
                "VALUES(:Name, :Address, :Port, :Description)");
    sql.bindValue(":Name", model.getName());
    sql.bindValue(":Address", model.getAddress());
    sql.bindValue(":Port", model.getPort());
    sql.bindValue(":Description", model.getDescription());

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QSqlDatabase::database().commit();
}

OscOutputModel DatabaseManager::getOscOutputByName(const QString& name)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    sql.prepare("SELECT o.Id, o.Name, o.Address, o.Port, o.Description FROM OscOutput o "
                "WHERE o.Name = :Name");
    sql.bindValue(":Name", name);

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    sql.first();

    return OscOutputModel(sql.value("Id").toInt(), sql.value("Name").toString(), sql.value("Address").toString(),
                          sql.value("Port").toInt(), sql.value("Description").toString());
}

OscOutputModel DatabaseManager::getOscOutputByAddress(const QString& address)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    sql.prepare("SELECT o.Id, o.Name, o.Address, o.Port, o.Description FROM OscOutput o "
                "WHERE o.Address = :Address");
    sql.bindValue(":Address", address);

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    sql.first();

    return OscOutputModel(sql.value("Id").toInt(), sql.value("Name").toString(), sql.value("Address").toString(),
                          sql.value("Port").toInt(), sql.value("Description").toString());
}

void DatabaseManager::updateOscOutput(const OscOutputModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QSqlQuery sql;
    sql.prepare("UPDATE OscOutput SET Name = :Name, Address = :Address, Port = :Port, Description = :Description "
                "WHERE Id = :Id");
    sql.bindValue(":Name", model.getName());
    sql.bindValue(":Address", model.getAddress());
    sql.bindValue(":Port", model.getPort());
    sql.bindValue(":Description", model.getDescription());
    sql.bindValue(":Id", model.getId());

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QSqlDatabase::database().commit();
}

void DatabaseManager::deleteOscOutput(int id)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString query = QString().arg(id);

    QSqlQuery sql;
    sql.prepare("DELETE FROM OscOutput "
                "WHERE Id = :Id");
    sql.bindValue(":Id", id);

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QSqlDatabase::database().commit();
}

QList<GpiPortModel> DatabaseManager::getGpiPorts()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    if (!sql.exec("SELECT g.Id, g.RisingEdge, g.Action FROM GpiPort g ORDER BY Id"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<GpiPortModel> models;
    while (sql.next())
        models.push_back(GpiPortModel(sql.value("Id").toInt(),
                                      sql.value("RisingEdge").toInt() == 1, Playout::fromString(sql.value("Action").toString())));

    return models;
}

void DatabaseManager::updateGpiPort(const GpiPortModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QSqlQuery sql;
    sql.prepare("UPDATE GpiPort SET Action = :Action, RisingEdge = :RisingEdge "
                "WHERE Id = :Id");
    sql.bindValue(":Action", Playout::toString(model.getAction()));
    sql.bindValue(":RisingEdge", model.isRisingEdge() ? "1" : "0");
    sql.bindValue(":Id", model.getPort() + 1);

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QSqlDatabase::database().commit();
}

QList<GpoPortModel> DatabaseManager::getGpoPorts()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    if (!sql.exec("SELECT g.Id, g.RisingEdge, g.PulseLengthMillis FROM GpoPort g ORDER BY Id"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<GpoPortModel> models;
    while (sql.next())
        models.push_back(GpoPortModel(sql.value("Id").toInt(),
                                      sql.value("RisingEdge").toInt() == 1, sql.value("PulseLengthMillis").toInt()));

    return models;
}

void DatabaseManager::updateGpoPort(const GpoPortModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QSqlQuery sql;
    sql.prepare("UPDATE GpoPort SET PulseLengthMillis = :PulseLengthMillis, RisingEdge = :RisingEdge "
                "WHERE Id = :Id");
    sql.bindValue(":PulseLengthMillis", model.getPulseLengthMillis());
    sql.bindValue(":RisingEdge", model.isRisingEdge() ? "1" : "0");
    sql.bindValue(":Id", model.getPort() + 1);

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QSqlDatabase::database().commit();
}

QList<TypeModel> DatabaseManager::getType()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    if (!sql.exec("SELECT t.Id, t.Value FROM Type t"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<TypeModel> models;
    while (sql.next())
        models.push_back(TypeModel(sql.value(0).toInt(), sql.value(1).toString()));

    return models;
}

TypeModel DatabaseManager::getTypeByValue(const QString& value)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    sql.prepare("SELECT t.Id FROM Type t "
                "WHERE t.Value = :Value");
    sql.bindValue(":Value", value);

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    sql.first();

    return TypeModel(sql.value(0).toInt(), sql.value(1).toString());
}

QList<DeviceModel> DatabaseManager::getDevice()
{
    QSqlQuery sql;
    if (!sql.exec("SELECT d.Id, d.Name, d.Address, d.Port, d.Username, d.Password, d.Description, d.Version, d.Shadow, d.Channels, d.ChannelFormats, d.PreviewChannel, d.LockedChannel FROM Device d ORDER BY d.Name"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<DeviceModel> models;
    while (sql.next())
        models.push_back(DeviceModel(sql.value("Id").toInt(), sql.value("Name").toString(), sql.value("Address").toString(), sql.value("Port").toInt(),
                                     sql.value("Username").toString(), sql.value("Password").toString(), sql.value("Description").toString(), sql.value("Version").toString(),
                                     sql.value("Shadow").toString(), sql.value("Channels").toInt(), sql.value("ChannelFormats").toString(), sql.value("PreviewChannel").toInt(), sql.value("LockedChannel").toInt()));

    return models;
}


DeviceModel DatabaseManager::getDeviceById(int deviceId)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    sql.prepare("SELECT d.Id, d.Name, d.Address, d.Port, d.Username, d.Password, d.Description, d.Version, d.Shadow, d.Channels, d.ChannelFormats, d.PreviewChannel, d.LockedChannel FROM Device d "
                "WHERE d.Id = :Id");
    sql.bindValue(":Id", deviceId);

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    sql.first();

    return DeviceModel(sql.value("Id").toInt(), sql.value("Name").toString(), sql.value("Address").toString(), sql.value("Port").toInt(),
                       sql.value("Username").toString(), sql.value("Password").toString(), sql.value("Description").toString(), sql.value("Version").toString(),
                       sql.value("Shadow").toString(), sql.value("Channels").toInt(), sql.value("ChannelFormats").toString(), sql.value("PreviewChannel").toInt(), sql.value("LockedChannel").toInt());
}

DeviceModel DatabaseManager::getDeviceByName(const QString& name)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    sql.prepare("SELECT d.Id, d.Name, d.Address, d.Port, d.Username, d.Password, d.Description, d.Version, d.Shadow, d.Channels, d.ChannelFormats, d.PreviewChannel, d.LockedChannel FROM Device d "
                "WHERE d.Name = :Name");
    sql.bindValue(":Name", name);

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    sql.first();

    return DeviceModel(sql.value("Id").toInt(), sql.value("Name").toString(), sql.value("Address").toString(), sql.value("Port").toInt(),
                       sql.value("Username").toString(), sql.value("Password").toString(), sql.value("Description").toString(), sql.value("Version").toString(),
                       sql.value("Shadow").toString(), sql.value("Channels").toInt(), sql.value("ChannelFormats").toString(), sql.value("PreviewChannel").toInt(), sql.value("LockedChannel").toInt());
}

DeviceModel DatabaseManager::getDeviceByAddress(const QString& address)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    sql.prepare("SELECT d.Id, d.Name, d.Address, d.Port, d.Username, d.Password, d.Description, d.Version, d.Shadow, d.Channels, d.ChannelFormats, d.PreviewChannel, d.LockedChannel FROM Device d "
                "WHERE d.Address = :Address");
    sql.bindValue(":Address", address);

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    sql.first();

    return DeviceModel(sql.value("Id").toInt(), sql.value("Name").toString(), sql.value("Address").toString(), sql.value("Port").toInt(),
                       sql.value("Username").toString(), sql.value("Password").toString(), sql.value("Description").toString(), sql.value("Version").toString(),
                       sql.value("Shadow").toString(), sql.value("Channels").toInt(), sql.value("ChannelFormats").toString(), sql.value("PreviewChannel").toInt(), sql.value("LockedChannel").toInt());
}

void DatabaseManager::insertDevice(const DeviceModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QSqlQuery sql;
    sql.prepare("INSERT INTO Device (Name, Address, Port, Username, Password, Description, Version, Shadow, Channels, ChannelFormats, PreviewChannel, LockedChannel) "
                "VALUES(:Name, :Address, :Port, :Username, :Password, :Description, :Version, :Shadow, :Channels, :ChannelFormats, :PreviewChannel, :LockedChannel)");
    sql.bindValue(":Name", model.getName());
    sql.bindValue(":Address", model.getAddress());
    sql.bindValue(":Port", model.getPort());
    sql.bindValue(":Username", model.getUsername());
    sql.bindValue(":Password", model.getPassword());
    sql.bindValue(":Description", model.getDescription());
    sql.bindValue(":Version", model.getVersion());
    sql.bindValue(":Shadow", model.getShadow());
    sql.bindValue(":Channels", model.getChannels());
    sql.bindValue(":ChannelFormats", model.getChannelFormats());
    sql.bindValue(":PreviewChannel", model.getPreviewChannel());
    sql.bindValue(":LockedChannel", model.getLockedChannel());

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QSqlDatabase::database().commit();
}

void DatabaseManager::updateDevice(const DeviceModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QSqlQuery sql;
    sql.prepare("UPDATE Device SET Name = :Name, Address = :Address, Port = :Port, Username = :Username, Password = :Password, Description = :Description, Version = :Version, Shadow = :Shadow, Channels = :Channels, ChannelFormats = :ChannelFormats, PreviewChannel = :PreviewChannel, LockedChannel = :LockedChannel "
                "WHERE Id = :Id");
    sql.bindValue(":Name", model.getName());
    sql.bindValue(":Address", model.getAddress());
    sql.bindValue(":Port", model.getPort());
    sql.bindValue(":Username", model.getUsername());
    sql.bindValue(":Password", model.getPassword());
    sql.bindValue(":Description", model.getDescription());
    sql.bindValue(":Version", model.getVersion());
    sql.bindValue(":Shadow", model.getShadow());
    sql.bindValue(":Channels", model.getChannels());
    sql.bindValue(":ChannelFormats", model.getChannelFormats());
    sql.bindValue(":PreviewChannel", model.getPreviewChannel());
    sql.bindValue(":LockedChannel", model.getLockedChannel());
    sql.bindValue(":Id", model.getId());

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QSqlDatabase::database().commit();
}

void DatabaseManager::updateDeviceVersion(const DeviceModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QSqlQuery sql;
    sql.prepare("UPDATE Device SET Version = :Version "
                "WHERE Address = :Address");
    sql.bindValue(":Version", model.getVersion());
    sql.bindValue(":Address", model.getAddress());

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QSqlDatabase::database().commit();
}

void DatabaseManager::updateDeviceChannels(const DeviceModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QSqlQuery sql;
    sql.prepare("UPDATE Device SET Channels = :Channels "
                "WHERE Address = :Address");
    sql.bindValue(":Channels", model.getChannels());
    sql.bindValue(":Address", model.getAddress());

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QSqlDatabase::database().commit();
}

void DatabaseManager::updateDeviceChannelFormats(const DeviceModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QSqlQuery sql;
    sql.prepare("UPDATE Device SET ChannelFormats = :ChannelFormats "
                "WHERE Address = :Address");
    sql.bindValue(":ChannelFormats", model.getChannelFormats());
    sql.bindValue(":Address", model.getAddress());

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QSqlDatabase::database().commit();
}

void DatabaseManager::deleteDevice(int id)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QSqlQuery sql;
    sql.prepare("DELETE FROM Device "
                "WHERE Id = :Id");
    sql.bindValue(":Id",id);

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    sql.prepare("DELETE FROM Thumbnail "
                "WHERE Id IN (SELECT l.ThumbnailId FROM Library l WHERE DeviceId = :DeviceId)");
    sql.bindValue(":DeviceId",id);

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    sql.prepare("DELETE FROM Library "
                "WHERE DeviceId = :DeviceId");
    sql.bindValue(":DeviceId",id);

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QSqlDatabase::database().commit();
}

QList<LibraryModel> DatabaseManager::getLibraryMedia()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    if (!sql.exec("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t WHERE  l.DeviceId = d.Id AND l.TypeId = t.Id AND (l.TypeId = 1 OR l.TypeId = 3 OR l.TypeId = 4) ORDER BY l.Name, l.DeviceId"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<LibraryModel> models;
    while (sql.next())
        models.push_back(LibraryModel(sql.value("Id").toInt(), sql.value(1).toString(), sql.value(1).toString(),
                                      sql.value(2).toString(), sql.value("Value").toString(), sql.value("ThumbnailId").toInt(),
                                      sql.value("Timecode").toString()));

    return models;
}

QList<LibraryModel> DatabaseManager::getLibraryTemplate()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    if (!sql.exec("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t WHERE  l.DeviceId = d.Id AND l.TypeId = t.Id AND l.TypeId = 5 ORDER BY l.Name, l.DeviceId"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<LibraryModel> models;
    while (sql.next())
        models.push_back(LibraryModel(sql.value("Id").toInt(), sql.value(1).toString(), sql.value(1).toString(),
                                      sql.value(2).toString(), sql.value("Value").toString(), sql.value("ThumbnailId").toInt(),
                                      sql.value("Timecode").toString()));

    return models;
}

QList<LibraryModel> DatabaseManager::getLibraryData()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    if (!sql.exec("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t WHERE  l.DeviceId = d.Id AND l.TypeId = t.Id AND l.TypeId = 2 ORDER BY l.Name, l.DeviceId"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<LibraryModel> models;
    while (sql.next())
        models.push_back(LibraryModel(sql.value("Id").toInt(), sql.value(1).toString(), sql.value(1).toString(),
                                      sql.value(2).toString(), sql.value("Value").toString(), sql.value("ThumbnailId").toInt(),
                                      sql.value("Timecode").toString()));

    return models;
}

QList<LibraryModel> DatabaseManager::getLibraryMediaByFilter(const QString& filter, QList<QString> devices)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;

    if (!filter.isEmpty() && devices.isEmpty()) // Filter on all devices.
    {    
        sql.prepare("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                    "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND (l.TypeId = 1 OR l.TypeId = 3 OR l.TypeId = 4) AND l.Name LIKE :Name "
                    "ORDER BY l.Name, l.DeviceId");
        sql.bindValue(":Name", QString("%%1%").arg(filter));
    }
    else if (!filter.isEmpty() && !devices.isEmpty()) // Filter specific devices.
    {
        sql.prepare("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                    "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND (l.TypeId = 1 OR l.TypeId = 3 OR l.TypeId = 4) AND l.Name LIKE :Name AND d.Address IN ('" + QStringList(devices).join("', '") + "') "
                    "ORDER BY l.Name, l.DeviceId");
        sql.bindValue(":Name", QString("%%1%").arg(filter));
    }
    else if (filter.isEmpty() && !devices.isEmpty()) // All on specific devices.
    {
        sql.prepare("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                    "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND (l.TypeId = 1 OR l.TypeId = 3 OR l.TypeId = 4) AND d.Address IN ('" + QStringList(devices).join("', '") + "') "
                    "ORDER BY l.Name, l.DeviceId");
    }

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<LibraryModel> models;
    while (sql.next())
        models.push_back(LibraryModel(sql.value("Id").toInt(), sql.value(1).toString(), sql.value(1).toString(),
                                      sql.value(2).toString(), sql.value("Value").toString(), sql.value("ThumbnailId").toInt(),
                                      sql.value("Timecode").toString()));

    return models;
}

QList<LibraryModel> DatabaseManager::getLibraryTemplateByFilter(const QString& filter, QList<QString> devices)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;

    if (!filter.isEmpty() && devices.isEmpty()) // Filter on all devices.
    {
        sql.prepare("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                    "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND l.TypeId = 5 AND l.Name LIKE :Name "
                    "ORDER BY l.Name, l.DeviceId");
        sql.bindValue(":Name", QString("%%1%").arg(filter));
    }
    else if (!filter.isEmpty() && !devices.isEmpty()) // Filter specific devices.
    {
        sql.prepare("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                    "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND l.TypeId = 5 AND l.Name LIKE :Name AND d.Address IN ('" + QStringList(devices).join("', '") + "') "
                    "ORDER BY l.Name, l.DeviceId");
        sql.bindValue(":Name", QString("%%1%").arg(filter));
    }
    else if (filter.isEmpty() && !devices.isEmpty()) // All on specific devices.
    {
        sql.prepare("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                    "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND l.TypeId = 5 AND d.Address IN ('" + QStringList(devices).join("', '") + "') "
                    "ORDER BY l.Name, l.DeviceId");
    }

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<LibraryModel> models;
    while (sql.next())
        models.push_back(LibraryModel(sql.value("Id").toInt(), sql.value(1).toString(), sql.value(1).toString(),
                                      sql.value(2).toString(), sql.value("Value").toString(), sql.value("ThumbnailId").toInt(),
                                      sql.value("Timecode").toString()));

    return models;
}

QList<LibraryModel> DatabaseManager::getLibraryDataByFilter(const QString& filter, QList<QString> devices)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;

    if (!filter.isEmpty() && devices.isEmpty()) // Filter on all devices.
    {  
        sql.prepare("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                    "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND l.TypeId = 2 AND l.Name LIKE :Name "
                    "ORDER BY l.Name, l.DeviceId");
        sql.bindValue(":Name", QString("%%1%").arg(filter));
    }
    else if (!filter.isEmpty() && !devices.isEmpty()) // Filter specific devices.
    {
        sql.prepare("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                    "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND l.TypeId = 2 AND l.Name LIKE :Name AND d.Address IN ('" + QStringList(devices).join("', '") + "') "
                    "ORDER BY l.Name, l.DeviceId");
        sql.bindValue(":Name", QString("%%1%").arg(filter));
    }
    else if (filter.isEmpty() && !devices.isEmpty()) // All on specific devices.
    {
        sql.prepare("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                    "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND l.TypeId = 2 AND d.Address IN ('" + QStringList(devices).join("', '") + "') "
                    "ORDER BY l.Name, l.DeviceId");
    }

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<LibraryModel> models;
    while (sql.next())
        models.push_back(LibraryModel(sql.value("Id").toInt(), sql.value(1).toString(), sql.value(1).toString(),
                                      sql.value(2).toString(), sql.value("Value").toString(), sql.value("ThumbnailId").toInt(),
                                      sql.value("Timecode").toString()));

    return models;
}

QList<LibraryModel> DatabaseManager::getLibraryByDeviceId(int deviceId)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    sql.prepare("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND d.Id = :Id "
                "ORDER BY l.Name, l.DeviceId");
    sql.bindValue(":Id", deviceId);

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<LibraryModel> models;
    while (sql.next())
        models.push_back(LibraryModel(sql.value("Id").toInt(), sql.value(1).toString(), sql.value(1).toString(),
                                      sql.value(2).toString(), sql.value("Value").toString(), sql.value("ThumbnailId").toInt(),
                                      sql.value("Timecode").toString()));

    return models;
}

QList<LibraryModel> DatabaseManager::getLibraryByDeviceIdAndFilter(int deviceId, const QString& filter)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;

    if (filter.isEmpty())
    {
        sql.prepare("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                    "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND d.Id = :Id "
                    "ORDER BY l.Name, l.DeviceId");
        sql.bindValue(":Id", deviceId);
    }
    else // Filter.
    {
        sql.prepare("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                    "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND d.Id = :Id AND l.Name LIKE :Name "
                    "ORDER BY l.Name, l.DeviceId");
        sql.bindValue(":Id", deviceId);
        sql.bindValue(":Name", QString("%%1%").arg(filter));
    }

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<LibraryModel> models;
    while (sql.next())
        models.push_back(LibraryModel(sql.value("Id").toInt(), sql.value(1).toString(), sql.value(1).toString(),
                                      sql.value(2).toString(), sql.value("Value").toString(), sql.value("ThumbnailId").toInt(),
                                      sql.value("Timecode").toString()));

    return models;
}

QList<LibraryModel> DatabaseManager::getLibraryMediaByDeviceAddress(const QString& address)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    sql.prepare("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND (l.TypeId = 1 OR l.TypeId = 3 OR l.TypeId = 4) AND d.Address = :Address "
                "ORDER BY l.Id, l.DeviceId");
    sql.bindValue(":Address", address);

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<LibraryModel> models;
    while (sql.next())
        models.push_back(LibraryModel(sql.value("Id").toInt(), sql.value(1).toString(), sql.value(1).toString(),
                                      sql.value(2).toString(), sql.value("Value").toString(), sql.value("ThumbnailId").toInt(),
                                      sql.value("Timecode").toString()));

    return models;
}

QList<LibraryModel> DatabaseManager::getLibraryTemplateByDeviceAddress(const QString& address)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    sql.prepare("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND l.TypeId = 5 AND d.Address = :Address "
                "ORDER BY l.Id, l.DeviceId");
    sql.bindValue(":Address", address);

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<LibraryModel> models;
    while (sql.next())
        models.push_back(LibraryModel(sql.value("Id").toInt(), sql.value(1).toString(), sql.value(1).toString(),
                                      sql.value(2).toString(), sql.value("Value").toString(), sql.value("ThumbnailId").toInt(),
                                      sql.value("Timecode").toString()));

    return models;
}

QList<LibraryModel> DatabaseManager::getLibraryDataByDeviceAddress(const QString& address)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    sql.prepare("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND l.TypeId = 2 AND d.Address = :Address "
                "ORDER BY l.Id, l.DeviceId");
    sql.bindValue(":Address", address);

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<LibraryModel> models;
    while (sql.next())
        models.push_back(LibraryModel(sql.value("Id").toInt(), sql.value(1).toString(), sql.value(1).toString(),
                                      sql.value(2).toString(), sql.value("Value").toString(), sql.value("ThumbnailId").toInt(),
                                      sql.value("Timecode").toString()));

    return models;
}

QList<LibraryModel> DatabaseManager::getLibraryByNameAndDeviceId(const QString& name, int deviceId)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    sql.prepare("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                "WHERE  l.Name = :Name AND l.DeviceId = :DeviceId AND l.DeviceId = d.Id AND l.TypeId = t.Id");
    sql.bindValue(":Name", name);
    sql.bindValue(":DeviceId", deviceId);

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<LibraryModel> models;
    while (sql.next())
        models.push_back(LibraryModel(sql.value("Id").toInt(), sql.value(1).toString(), sql.value(1).toString(),
                                      sql.value(2).toString(), sql.value("Value").toString(), sql.value("ThumbnailId").toInt(),
                                      sql.value("Timecode").toString()));

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
            sql.prepare("DELETE FROM Library "
                        "WHERE Id = :Id");
            sql.bindValue(":Id", deleteModels.at(i).getId());

            if (!sql.exec())
               qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));
        }
    }

    if (insertModels.count() > 0)
    {
        int typeId;
        for (int i = 0; i < insertModels.count(); i++)
        {
            if (insertModels.at(i).getType() == Rundown::AUDIO)
                typeId = std::find_if(typeModels.begin(), typeModels.end(), TypeModel::ByName(Rundown::AUDIO))->getId();
            else if (insertModels.at(i).getType() == Rundown::MOVIE)
                typeId = std::find_if(typeModels.begin(), typeModels.end(), TypeModel::ByName(Rundown::MOVIE))->getId();
            else if (insertModels.at(i).getType() == Rundown::STILL)
                typeId = std::find_if(typeModels.begin(), typeModels.end(), TypeModel::ByName(Rundown::STILL))->getId();

            sql.prepare("INSERT INTO Library (Name, DeviceId, TypeId, ThumbnailId, Timecode) "
                        "VALUES(:Name, :DeviceId, :TypeId, :ThumbnailId, :Timecode)");
            sql.bindValue(":Name", insertModels.at(i).getName());
            sql.bindValue(":DeviceId", deviceId);
            sql.bindValue(":TypeId", typeId);
            sql.bindValue(":ThumbnailId", insertModels.at(i).getThumbnailId());
            sql.bindValue(":Timecode", insertModels.at(i).getTimecode());

            if (!sql.exec())
               qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));
        }
    }

    QSqlDatabase::database().commit();
}

void DatabaseManager::updateLibraryTemplate(const QString& address, const QList<LibraryModel>& deleteModels, const QList<LibraryModel>& insertModels)
{
    QMutexLocker locker(&mutex);

    int deviceId = getDeviceByAddress(address).getId();
    QList<TypeModel> typeModels = getType();
    int typeId = std::find_if(typeModels.begin(), typeModels.end(), TypeModel::ByName(Rundown::TEMPLATE))->getId();

    QSqlDatabase::database().transaction();

    QSqlQuery sql;
    if (deleteModels.count() > 0)
    {
        for (int i = 0; i < deleteModels.count(); i++)
        {
            sql.prepare("DELETE FROM Library "
                        "WHERE Id = :Id");
            sql.bindValue(":Id", deleteModels.at(i).getId());

            if (!sql.exec())
               qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));
        }
    }

    if (insertModels.count() > 0)
    {
        for (int i = 0; i < insertModels.count(); i++)
        {
            sql.prepare("INSERT INTO Library (Name, DeviceId, TypeId, ThumbnailId, Timecode) "
                        "VALUES(:Name, :DeviceId, :TypeId, :ThumbnailId, :Timecode)");
            sql.bindValue(":Name", insertModels.at(i).getName());
            sql.bindValue(":DeviceId", deviceId);
            sql.bindValue(":TypeId", typeId);
            sql.bindValue(":ThumbnailId", insertModels.at(i).getThumbnailId());
            sql.bindValue(":Timecode", insertModels.at(i).getTimecode());

            if (!sql.exec())
               qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));
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
            sql.prepare("DELETE FROM Thumbnail "
                        "WHERE Id = :Id");
            sql.bindValue(":Id", deleteModels.at(i).getThumbnailId());

            if (!sql.exec())
               qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

            sql.prepare("DELETE FROM Library "
                        "WHERE Id = :Id AND TypeId = 2");
            sql.bindValue(":Id", deleteModels.at(i).getId());

            if (!sql.exec())
               qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));
        }
    }

    if (insertModels.count() > 0)
    {
        int typeId;
        for (int i = 0; i < insertModels.count(); i++)
        {
            if (insertModels.at(i).getType() == "DATA")
                typeId = std::find_if(typeModels.begin(), typeModels.end(), TypeModel::ByName("DATA"))->getId();

            sql.prepare("INSERT INTO Library (Name, DeviceId, TypeId, ThumbnailId, Timecode) "
                        "VALUES(:Name, :DeviceId, :TypeId, :ThumbnailId, :Timecode)");
            sql.bindValue(":Name", insertModels.at(i).getName());
            sql.bindValue(":DeviceId", deviceId);
            sql.bindValue(":TypeId", typeId);
            sql.bindValue(":ThumbnailId", insertModels.at(i).getThumbnailId());
            sql.bindValue(":Timecode", insertModels.at(i).getTimecode());

            if (!sql.exec())
               qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));
        }
    }

    QSqlDatabase::database().commit();
}

void DatabaseManager::deleteLibrary(int deviceId)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QSqlQuery sql;
    sql.prepare("DELETE FROM Library "
                "WHERE DeviceId = :DeviceId");
    sql.bindValue(":DeviceId", deviceId);

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QSqlDatabase::database().commit();
}

QList<ThumbnailModel> DatabaseManager::getThumbnailByDeviceAddress(const QString& address)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    sql.prepare("SELECT t.Id, t.Data, t.Timestamp, t.Size, l.Name, d.Address FROM Thumbnail t, Library l, Device d "
                "WHERE d.Address = :Address AND l.DeviceId = d.Id AND l.ThumbnailId = t.Id");
    sql.bindValue(":Address", address);

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<ThumbnailModel> models;
    while (sql.next())
        models.push_back(ThumbnailModel(sql.value("Id").toInt(), sql.value("Data").toString(), sql.value("Timestamp").toString(),
                                        sql.value("Size").toString(), sql.value("Name").toString(), sql.value("Address").toString()));

    return models;
}

ThumbnailModel DatabaseManager::getThumbnailByNameAndDeviceName(const QString& name, const QString& deviceName)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    sql.prepare("SELECT t.Id, t.Data, t.Timestamp, t.Size, l.Name, d.Name, d.Address FROM Thumbnail t, Library l, Device d "
                "WHERE l.Name = :Name AND d.Name = :DeviceName AND l.ThumbnailId = t.Id");
    sql.bindValue(":Name", name);
    sql.bindValue(":DeviceName", deviceName);

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    sql.first();

    return ThumbnailModel(sql.value("Id").toInt(), sql.value("Data").toString(), sql.value("Timestamp").toString(),
                          sql.value("Size").toString(), sql.value("Name").toString(), sql.value("Address").toString());
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
                sql.prepare("UPDATE Thumbnail SET Data = :Data, Timestamp = :Timestamp, Size = :Size "
                            "WHERE Id = :Id");
                sql.bindValue(":Data", model.getData());
                sql.bindValue(":Timestamp", model.getTimestamp());
                sql.bindValue(":Size", model.getSize());
                sql.bindValue(":Id", libraryModel.getThumbnailId());

                if (!sql.exec())
                   qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));
            }
            else
            {
                sql.prepare("INSERT INTO Thumbnail (Data, Timestamp, Size) "
                            "VALUES(:Data, :Timestamp, :Size)");
                sql.bindValue(":Data", model.getData());
                sql.bindValue(":Timestamp", model.getTimestamp());
                sql.bindValue(":Size", model.getSize());

                if (!sql.exec())
                   qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

                int lastInsertId = sql.lastInsertId().toInt();
                sql.prepare("UPDATE Library SET ThumbnailId = :ThumbnailId "
                            "WHERE Id = :Id");
                sql.bindValue(":ThumbnailId", lastInsertId);
                sql.bindValue(":Id", libraryModel.getId());

                if (!sql.exec())
                   qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));
            }
        }
    }

    QSqlDatabase::database().commit();
}

void DatabaseManager::deleteThumbnails()
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QSqlQuery sql;
    if (!sql.exec("DELETE FROM Thumbnail"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    const QList<LibraryModel>& models = this->getLibraryMedia();
    for (int i = 0; i < models.count(); i++)
    {
        const LibraryModel& model = models.at(i);
        if (model.getThumbnailId() > 0)
        {
            sql.prepare("UPDATE Library SET ThumbnailId = 0 "
                        "WHERE Id = :Id");
            sql.bindValue(":Id", model.getId());

            if (!sql.exec())
               qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));
        }
    }

    QSqlDatabase::database().commit();
}
