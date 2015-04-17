#include "DatabaseManager.h"

#include "Version.h"

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
        foreach (const QString& query, queries)
        {
             if (query.trimmed().isEmpty())
                 continue;

             if (!sql.exec(query))
                qFatal(qPrintable(QString("Failed to execute sql query: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text())));
        }

#if defined(Q_OS_WIN)
        if (!sql.exec("INSERT INTO Configuration (Name, Value) VALUES('FontSize', '11')"))
            qFatal(qPrintable(QString("Failed to execute sql query: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text())));

        if (!sql.exec("INSERT INTO Device (Name, Address, Port, Username, Password, Description, Version, Shadow, Channels, ChannelFormats, PreviewChannel, LockedChannel) VALUES('Local CasparCG', '127.0.0.1', 5250, '', '', '', '', 'No', 0, '', 0, 0)"))
            qFatal(qPrintable(QString("Failed to execute sql query: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text())));
#else
        if (!sql.exec("INSERT INTO Configuration (Name, Value) VALUES('FontSize', '12')"))
            qFatal(qPrintable(QString("Failed to execute sql query: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text())));
#endif

        if (!sql.exec(QString("PRAGMA user_version = %1").arg(DATABASE_VERSION)))
            qFatal(qPrintable(QString("Failed to execute sql query: %1, Error: %1").arg(sql.lastQuery()).arg(sql.lastError().text())));
    }
}

void DatabaseManager::upgradeDatabase()
{
    QSqlQuery sql;
    if (!sql.exec("PRAGMA user_version"))
       qFatal(qPrintable(QString("Failed to execute sql query: %1, Error: %1").arg(sql.lastQuery()).arg(sql.lastError().text())));

    sql.first();

    int version = sql.value(0).toInt();

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
                    qFatal(qPrintable(QString("Failed to execute sql query: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text())));
            }

            sql.prepare("PRAGMA user_version = :Version");
            sql.bindValue(":Version", version + 1);

            if (!sql.exec())
                qFatal(qPrintable(QString("Failed to execute sql query: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text())));

            qDebug("Successfully upgraded to ChangeScript-%d", version + 1);
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

    return ConfigurationModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString());
}

QList<FormatModel> DatabaseManager::getFormat()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    if (!sql.exec("SELECT f.Id, f.Name, f.Width, f.Height, f.FramesPerSecond FROM Format f"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<FormatModel> models;
    while (sql.next())
        models.push_back(FormatModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toInt(), sql.value(3).toInt(), sql.value(4).toString()));

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

    return FormatModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toInt(), sql.value(3).toInt(), sql.value(4).toString());
}

QList<QString> DatabaseManager::getOpenRecent()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    if (!sql.exec("SELECT o.Id, o.Value FROM OpenRecent o ORDER BY o.Id DESC"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<QString> values;
    while (sql.next())
        values.push_back(sql.value(1).toString());

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

    if (!sql.exec("DELETE FROM OpenRecent WHERE Id > 10"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

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
        models.push_back(PresetModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString()));

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

    return PresetModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString());
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
        models.push_back(PresetModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString()));

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
        models.push_back(BlendModeModel(sql.value(0).toInt(), sql.value(1).toString()));

    return models;
}

QList<ChromaModel> DatabaseManager::getChroma()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    if (!sql.exec("SELECT c.Id, c.Key FROM Chroma c"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<ChromaModel> models;
    while (sql.next())
        models.push_back(ChromaModel(sql.value(0).toInt(), sql.value(1).toString()));

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
        models.push_back(DirectionModel(sql.value(0).toInt(), sql.value(1).toString()));

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
        models.push_back(TransitionModel(sql.value(0).toInt(), sql.value(1).toString()));

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
        models.push_back(TweenModel(sql.value(0).toInt(), sql.value(1).toString()));

    return models;
}

QList<OscOutputModel> DatabaseManager::getOscOutput()
{
    QSqlQuery sql;
    if (!sql.exec("SELECT o.Id, o.Name, o.Address, o.Port, o.Description FROM OscOutput o ORDER BY o.Name"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<OscOutputModel> models;
    while (sql.next())
        models.push_back(OscOutputModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(),
                                        sql.value(3).toInt(), sql.value(4).toString()));

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

    return OscOutputModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(),
                          sql.value(3).toInt(), sql.value(4).toString());
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

    return OscOutputModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(),
                          sql.value(3).toInt(), sql.value(4).toString());
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

QList<AtemStepModel> DatabaseManager::getAtemStep()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    if (!sql.exec("SELECT t.Id, t.Name, t.Value FROM AtemStep t"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<AtemStepModel> models;
    while (sql.next())
        models.push_back(AtemStepModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString()));

    return models;
}

QList<AtemMixerStepModel> DatabaseManager::getAtemMixerStep()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    if (!sql.exec("SELECT t.Id, t.Name, t.Value FROM AtemMixerStep t"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<AtemMixerStepModel> models;
    while (sql.next())
        models.push_back(AtemMixerStepModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString()));

    return models;
}

QList<AtemAudioInputStateModel> DatabaseManager::getAtemAudioInputState()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    if (!sql.exec("SELECT t.Id, t.Name, t.Value FROM AtemAudioInputState t"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<AtemAudioInputStateModel> models;
    while (sql.next())
        models.push_back(AtemAudioInputStateModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString()));

    return models;
}

QList<AtemKeyerModel> DatabaseManager::getAtemKeyer()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    if (!sql.exec("SELECT t.Id, t.Name, t.Value FROM AtemKeyer t"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<AtemKeyerModel> models;
    while (sql.next())
        models.push_back(AtemKeyerModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString()));

    return models;
}

QList<AtemSwitcherModel> DatabaseManager::getAtemSwitcher()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    if (!sql.exec("SELECT t.Id, t.Name, t.Value FROM AtemSwitcher t"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<AtemSwitcherModel> models;
    while (sql.next())
        models.push_back(AtemSwitcherModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString()));

    return models;
}

QList<AtemVideoFormatModel> DatabaseManager::getAtemVideoFormat()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    if (!sql.exec("SELECT t.Id, t.Name, t.Value FROM AtemVideoFormat t"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<AtemVideoFormatModel> models;
    while (sql.next())
        models.push_back(AtemVideoFormatModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString()));

    return models;
}

QList<AtemAutoTransitionModel> DatabaseManager::getAtemAutoTransition()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    if (!sql.exec("SELECT t.Id, t.Name, t.Value FROM AtemAutoTransition t"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<AtemAutoTransitionModel> models;
    while (sql.next())
        models.push_back(AtemAutoTransitionModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString()));

    return models;
}

QList<AtemDeviceModel> DatabaseManager::getAtemDevice()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    if (!sql.exec("SELECT o.Id, o.Name, o.Address, o.Description FROM AtemDevice o ORDER BY o.Name"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<AtemDeviceModel> models;
    while (sql.next())
        models.push_back(AtemDeviceModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toString()));

    return models;
}

AtemDeviceModel DatabaseManager::getAtemDeviceByName(const QString& name)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    sql.prepare("SELECT d.Id, d.Name, d.Address, d.Description FROM AtemDevice d "
                "WHERE d.Name = :Name");
    sql.bindValue(":Name", name);

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    sql.first();

    return AtemDeviceModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toString());
}

AtemDeviceModel DatabaseManager::getAtemDeviceByAddress(const QString& address)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    sql.prepare("SELECT d.Id, d.Name, d.Address, d.Description FROM AtemDevice d "
                "WHERE d.Address = :Address");
    sql.bindValue(":Address", address);

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    sql.first();

    return AtemDeviceModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toString());
}

void DatabaseManager::insertAtemDevice(const AtemDeviceModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QSqlQuery sql;
    sql.prepare("INSERT INTO AtemDevice (Name, Address, Description) "
                "VALUES(:Name, :Address, :Description)");
    sql.bindValue(":Name", model.getName());
    sql.bindValue(":Address", model.getAddress());
    sql.bindValue(":Description", model.getDescription());

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QSqlDatabase::database().commit();
}

void DatabaseManager::updateAtemDevice(const AtemDeviceModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QSqlQuery sql;
    sql.prepare("UPDATE AtemDevice SET Name = :Name, Address = :Address, Description = :Description "
                "WHERE Id = :Id");
    sql.bindValue(":Name", model.getName());
    sql.bindValue(":Address", model.getAddress());
    sql.bindValue(":Description", model.getDescription());
    sql.bindValue(":Description", model.getId());

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QSqlDatabase::database().commit();
}

void DatabaseManager::deleteAtemDevice(int id)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QSqlQuery sql;
    sql.prepare("DELETE FROM AtemDevice "
                "WHERE Id = :Id");
    sql.bindValue(":Id", id);

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QSqlDatabase::database().commit();
}

QList<TriCasterProductModel> DatabaseManager::getTriCasterProduct()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    if (!sql.exec("SELECT p.Id, p.Name FROM TriCasterProduct p"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<TriCasterProductModel> models;
    while (sql.next())
        models.push_back(TriCasterProductModel(sql.value(0).toInt(), sql.value(1).toString()));

    return models;
}

QList<TriCasterInputModel> DatabaseManager::getTriCasterInput()
{
    QMutexLocker locker(&mutex);

    QString product = getConfigurationByName("TriCasterProduct").getValue();

    QSqlQuery sql;
    sql.prepare("SELECT t.Id, t.Name, t.Value, t.Products FROM TriCasterInput t "
                "WHERE t.Products LIKE :Products");
    sql.bindValue(":Products", QString("%%1%").arg(product));

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<TriCasterInputModel> models;
    while (sql.next())
        models.push_back(TriCasterInputModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString()));

    return models;
}

QList<TriCasterStepModel> DatabaseManager::getTriCasterStep()
{
    QMutexLocker locker(&mutex);

    QString product = getConfigurationByName("TriCasterProduct").getValue();

    QSqlQuery sql;
    sql.prepare("SELECT t.Id, t.Name, t.Value, t.Products FROM TriCasterStep t "
                "WHERE t.Products LIKE :Products");
    sql.bindValue(":Products", QString("%%1%").arg(product));

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<TriCasterStepModel> models;
    while (sql.next())
        models.push_back(TriCasterStepModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toString()));

    return models;
}

QList<TriCasterAutoSpeedModel> DatabaseManager::getTriCasterAutoSpeed()
{
    QMutexLocker locker(&mutex);

    QString product = getConfigurationByName("TriCasterProduct").getValue();

    QSqlQuery sql;
    sql.prepare("SELECT t.Id, t.Name, t.Value, t.Products FROM TriCasterAutoSpeed t "
                "WHERE t.Products LIKE :Products");
    sql.bindValue(":Products", QString("%%1%").arg(product));

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<TriCasterAutoSpeedModel> models;
    while (sql.next())
        models.push_back(TriCasterAutoSpeedModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toString()));

    return models;
}

QList<TriCasterAutoTransitionModel> DatabaseManager::getTriCasterAutoTransition()
{
    QMutexLocker locker(&mutex);

    QString product = getConfigurationByName("TriCasterProduct").getValue();

    QSqlQuery sql;
    sql.prepare("SELECT t.Id, t.Name, t.Value, t.Products FROM TriCasterAutoTransition t "
                "WHERE t.Products LIKE :Products");
    sql.bindValue(":Products", QString("%%1%").arg(product));

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<TriCasterAutoTransitionModel> models;
    while (sql.next())
        models.push_back(TriCasterAutoTransitionModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toString()));

    return models;
}

QList<TriCasterPresetModel> DatabaseManager::getTriCasterPreset()
{
    QMutexLocker locker(&mutex);

    QString product = getConfigurationByName("TriCasterProduct").getValue();

    QSqlQuery sql;
    sql.prepare("SELECT t.Id, t.Name, t.Value, t.Products FROM TriCasterPreset t "
                "WHERE t.Products LIKE :Products");
    sql.bindValue(":Products", QString("%%1%").arg(product));

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<TriCasterPresetModel> models;
    while (sql.next())
        models.push_back(TriCasterPresetModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toString()));

    return models;
}

QList<TriCasterSourceModel> DatabaseManager::getTriCasterSource()
{
    QMutexLocker locker(&mutex);

    QString product = getConfigurationByName("TriCasterProduct").getValue();

    QSqlQuery sql;
    sql.prepare("SELECT t.Id, t.Name, t.Value, t.Products FROM TriCasterSource t "
                "WHERE t.Products LIKE :Products");
    sql.bindValue(":Products", QString("%%1%").arg(product));

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<TriCasterSourceModel> models;
    while (sql.next())
        models.push_back(TriCasterSourceModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toString()));

    return models;
}

QList<TriCasterSwitcherModel> DatabaseManager::getTriCasterSwitcher()
{
    QMutexLocker locker(&mutex);

    QString product = getConfigurationByName("TriCasterProduct").getValue();

    QSqlQuery sql;
    sql.prepare("SELECT t.Id, t.Name, t.Value, t.Products FROM TriCasterSwitcher t "
                "WHERE t.Products LIKE :Products");
    sql.bindValue(":Products", QString("%%1%").arg(product));

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<TriCasterSwitcherModel> models;
    while (sql.next())
        models.push_back(TriCasterSwitcherModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toString()));

    return models;
}

QList<TriCasterNetworkTargetModel> DatabaseManager::getTriCasterNetworkTarget()
{
    QMutexLocker locker(&mutex);

    QString product = getConfigurationByName("TriCasterProduct").getValue();

    QSqlQuery sql;
    sql.prepare("SELECT t.Id, t.Name, t.Value, t.Products FROM TriCasterNetworkTarget t "
                "WHERE t.Products LIKE :Products");
    sql.bindValue(":Products", QString("%%1%").arg(product));

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<TriCasterNetworkTargetModel> models;
    while (sql.next())
        models.push_back(TriCasterNetworkTargetModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toString()));

    return models;
}

QList<TriCasterDeviceModel> DatabaseManager::getTriCasterDevice()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    if (!sql.exec("SELECT d.Id, d.Name, d.Address, d.Port, d.Description FROM TriCasterDevice d ORDER BY d.Name"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<TriCasterDeviceModel> models;
    while (sql.next())
        models.push_back(TriCasterDeviceModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(),
                                              sql.value(3).toInt(), sql.value(4).toString()));

    return models;
}

TriCasterDeviceModel DatabaseManager::getTriCasterDeviceByName(const QString& name)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    sql.prepare("SELECT d.Id, d.Name, d.Address, d.Port, d.Description FROM TriCasterDevice d "
                "WHERE d.Name = :Name");
    sql.bindValue(":Name", name);

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    sql.first();

    return TriCasterDeviceModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(),
                                sql.value(3).toInt(), sql.value(4).toString());
}

TriCasterDeviceModel DatabaseManager::getTriCasterDeviceByAddress(const QString& address)
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    sql.prepare("SELECT d.Id, d.Name, d.Address, d.Port, d.Description FROM TriCasterDevice d "
                "WHERE d.Address = :Address");
    sql.bindValue(":Address", address);

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    sql.first();

    return TriCasterDeviceModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(),
                                sql.value(3).toInt(), sql.value(4).toString());
}

void DatabaseManager::insertTriCasterDevice(const TriCasterDeviceModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QSqlQuery sql;
    sql.prepare("INSERT INTO TriCasterDevice (Name, Address, Port, Description) "
                "VALUES(:Name, :Address, :Port, :Description)");
    sql.bindValue(":Name", model.getName());
    sql.bindValue(":Address", model.getAddress());
    sql.bindValue(":Port", model.getPort());
    sql.bindValue(":Description", model.getDescription());

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QSqlDatabase::database().commit();
}

void DatabaseManager::updateTriCasterDevice(const TriCasterDeviceModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QSqlQuery sql;
    sql.prepare("UPDATE TriCasterDevice SET Name = :Name, Address = :Address, Port = :Port, Description = :Description "
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

void DatabaseManager::deleteTriCasterDevice(int id)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QSqlQuery sql;
    sql.prepare("DELETE FROM TriCasterDevice "
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
    if (!sql.exec("SELECT Id, RisingEdge, Action FROM GpiPort ORDER BY Id"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<GpiPortModel> models;
    while (sql.next())
        models.push_back(GpiPortModel(sql.value(0).toInt(), sql.value(1).toInt() == 1, Playout::fromString(sql.value(2).toString())));

    return models;
}

void DatabaseManager::updateGpiPort(const GpiPortModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QSqlQuery sql;
    sql.prepare("UPDATE GpiPort SET Action = :Action, RisingEdge = :RisingEdge "
                "WHERE Id = :id");
    sql.bindValue(":Action", Playout::toString(model.getAction()));
    sql.bindValue(":RisingEdge", model.isRisingEdge() ? "1" : "0");
    sql.bindValue(":Id", model.getPort());

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QSqlDatabase::database().commit();
}

QList<GpoPortModel> DatabaseManager::getGpoPorts()
{
    QMutexLocker locker(&mutex);

    QSqlQuery sql;
    if (!sql.exec("SELECT Id, RisingEdge, PulseLengthMillis FROM GpoPort ORDER BY Id"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<GpoPortModel> models;
    while (sql.next())
        models.push_back(GpoPortModel(sql.value(0).toInt(), sql.value(1).toInt() == 1, sql.value(2).toInt()));

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
    sql.bindValue(":Id", model.getPort());

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
        models.push_back(DeviceModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toInt(),
                                     sql.value(4).toString(), sql.value(5).toString(), sql.value(6).toString(), sql.value(7).toString(),
                                     sql.value(8).toString(), sql.value(9).toInt(), sql.value(10).toString(), sql.value(11).toInt(), sql.value(12).toInt()));

    return models;
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

    return DeviceModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toInt(),
                       sql.value(4).toString(), sql.value(5).toString(), sql.value(6).toString(), sql.value(6).toString(),
                       sql.value(8).toString(), sql.value(9).toInt(), sql.value(10).toString(), sql.value(11).toInt(), sql.value(12).toInt());
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

    return DeviceModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toInt(),
                       sql.value(4).toString(), sql.value(5).toString(), sql.value(6).toString(), sql.value(6).toString(),
                       sql.value(8).toString(), sql.value(9).toInt(), sql.value(10).toString(), sql.value(11).toInt(), sql.value(12).toInt());
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
        models.push_back(LibraryModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(1).toString(),
                                      sql.value(2).toString(), sql.value(3).toString(), sql.value(4).toInt(),
                                      sql.value(5).toString()));

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
        models.push_back(LibraryModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(1).toString(),
                                      sql.value(2).toString(), sql.value(3).toString(), sql.value(4).toInt(),
                                      sql.value(5).toString()));

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
        models.push_back(LibraryModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(1).toString(),
                                      sql.value(2).toString(), sql.value(3).toString(), sql.value(4).toInt(),
                                      sql.value(5).toString()));

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
        QString address;
        foreach (QString device, devices)
            address += QString("d.Address LIKE '%%1%' OR ").arg(device);

        address = address.mid(0, address.length() - 4); // Remove the last OR.

        sql.prepare("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                    "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND (l.TypeId = 1 OR l.TypeId = 3 OR l.TypeId = 4) AND l.Name LIKE :Name AND (:Address) "
                    "ORDER BY l.Name, l.DeviceId");
        sql.bindValue(":Name", QString("%%1%").arg(filter));
        sql.bindValue(":Address", address);
    }
    else if (filter.isEmpty() && !devices.isEmpty()) // All on specific devices.
    {
        QString address;
        foreach (QString device, devices)
            address += QString("d.Address LIKE '%%1%' OR ").arg(device);

        address = address.mid(0, address.length() - 4); // Remove the last OR.

        sql.prepare("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                    "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND (l.TypeId = 1 OR l.TypeId = 3 OR l.TypeId = 4) AND (:Address) "
                    "ORDER BY l.Name, l.DeviceId");
        sql.bindValue(":Address", address);
    }

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

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
        QString address;
        foreach (QString device, devices)
            address += QString("d.Address LIKE '%%1%' OR ").arg(device);

        address = address.mid(0, address.length() - 4); // Remove the last OR.

        sql.prepare("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                    "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND l.TypeId = 5 AND l.Name LIKE :Name AND (:Address) "
                    "ORDER BY l.Name, l.DeviceId");
        sql.bindValue(":Name", QString("%%1%").arg(filter));
        sql.bindValue(":Address", address);
    }
    else if (filter.isEmpty() && !devices.isEmpty()) // All on specific devices.
    {
        QString address;
        foreach (QString device, devices)
            address += QString("d.Address LIKE '%%1%' OR ").arg(device);

        address = address.mid(0, address.length() - 4); // Remove the last OR.

        sql.prepare("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                    "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND l.TypeId = 5 AND (:Address) "
                    "ORDER BY l.Name, l.DeviceId");
        sql.bindValue(":Address", address);
    }

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

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
        QString address;
        foreach (QString device, devices)
            address += QString("d.Address LIKE '%%1%' OR ").arg(device);

        address = address.mid(0, address.length() - 4); // Remove the last OR.

        sql.prepare("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                    "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND l.TypeId = 2 AND l.Name LIKE :Name AND (:Address) "
                    "ORDER BY l.Name, l.DeviceId");
        sql.bindValue(":Name", QString("%%1%").arg(filter));
        sql.bindValue(":Address", address);
    }
    else if (filter.isEmpty() && !devices.isEmpty()) // All on specific devices.
    {
        QString address;
        foreach (QString device, devices)
            address += QString("d.Address LIKE '%%1%' OR ").arg(device);

        address = address.mid(0, address.length() - 4); // Remove the last OR.

        sql.prepare("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                    "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND l.TypeId = 2 AND (:Address) "
                    "ORDER BY l.Name, l.DeviceId");
        sql.bindValue(":Address", address);
    }

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

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

    QSqlQuery sql;
    sql.prepare("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND d.Id = :Id "
                "ORDER BY l.Name, l.DeviceId");
    sql.bindValue(":Id", deviceId);

    if (!sql.exec())
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));

    QList<LibraryModel> models;
    while (sql.next())
        models.push_back(LibraryModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(1).toString(),
                                      sql.value(2).toString(), sql.value(3).toString(), sql.value(4).toInt(),
                                      sql.value(5).toString()));

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
        models.push_back(LibraryModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(1).toString(),
                                      sql.value(2).toString(), sql.value(3).toString(), sql.value(4).toInt(),
                                      sql.value(5).toString()));

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
        models.push_back(LibraryModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(1).toString(),
                                      sql.value(2).toString(), sql.value(3).toString(), sql.value(4).toInt(),
                                      sql.value(5).toString()));

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
        models.push_back(LibraryModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(1).toString(),
                                      sql.value(2).toString(), sql.value(3).toString(), sql.value(4).toInt(),
                                      sql.value(5).toString()));

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
        models.push_back(LibraryModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(1).toString(),
                                      sql.value(2).toString(), sql.value(3).toString(), sql.value(4).toInt(),
                                      sql.value(5).toString()));

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
            if (insertModels.at(i).getType() == "AUDIO")
                typeId = std::find_if(typeModels.begin(), typeModels.end(), TypeModel::ByName("AUDIO"))->getId();
            else if (insertModels.at(i).getType() == "MOVIE")
                typeId = std::find_if(typeModels.begin(), typeModels.end(), TypeModel::ByName("MOVIE"))->getId();
            else if (insertModels.at(i).getType() == "STILL")
                typeId = std::find_if(typeModels.begin(), typeModels.end(), TypeModel::ByName("STILL"))->getId();

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
    int typeId = std::find_if(typeModels.begin(), typeModels.end(), TypeModel::ByName("TEMPLATE"))->getId();

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
        models.push_back(ThumbnailModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(),
                                        sql.value(3).toString(), sql.value(4).toString(), sql.value(5).toString()));

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

                sql.prepare("UPDATE Library SET ThumbnailId = :ThumbnailId "
                            "WHERE Id = :Id");
                sql.bindValue(":ThumbnailId", sql.lastInsertId().toInt());
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

    // Shrink file on disk.
    if (!sql.exec("VACUUM Thumbnail"))
       qCritical("Failed to execute sql query: %s, Error: %s", qPrintable(sql.lastQuery()), qPrintable(sql.lastError().text()));
}
