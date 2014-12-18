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
                qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());
        }

#if defined(Q_OS_WIN)
    sql.exec("INSERT INTO Configuration (Name, Value) VALUES('FontSize', '11')");
    sql.exec("INSERT INTO Device (Name, Address, Port, Username, Password, Description, Version, Shadow, Channels, ChannelFormats, PreviewChannel) VALUES('Local CasparCG', '127.0.0.1', 5250, '', '', '', '', 'No', 0, '', 0)");
#else
    sql.exec("INSERT INTO Configuration (Name, Value) VALUES('FontSize', '12')");
#endif

        if (!sql.exec(QString("PRAGMA user_version = %1").arg(DATABASE_VERSION)))
            qCritical() << QString("Failed to execute: %1, Error: %1").arg(sql.lastQuery()).arg(sql.lastError().text());

        qDebug() << QString("DatabaseManager::createDatabase(): Created version: %1").arg(DATABASE_VERSION);
    }
}

void DatabaseManager::upgradeDatabase()
{
    qDebug() << QString("DatabaseManager::upgradeDatabase(): Required version: %1").arg(DATABASE_VERSION);

    QSqlQuery sql;
    if (!sql.exec("PRAGMA user_version"))
       qCritical() << QString("Failed to execute: %1, Error: %1").arg(sql.lastQuery()).arg(sql.lastError().text());

    sql.first();

    int version = sql.value(0).toInt();

    qDebug() << QString("DatabaseManager::upgradeDatabase(): Current version: %1").arg(version);

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
                    qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());
            }

            if (!sql.exec(QString("PRAGMA user_version = %1").arg(version + 1)))
                qCritical() << QString("Failed to execute: %1, Error: %1").arg(sql.lastQuery()).arg(sql.lastError().text());

            qDebug() << QString("DatabaseManager::upgradeDatabase(): ChangeScript-%1 executed").arg(version + 1);
        }

        version++;
    }
}

void DatabaseManager::updateConfiguration(const ConfigurationModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString query = QString("UPDATE Configuration SET Value = '%1' "
                            "WHERE Name = '%2'").arg(model.getValue()).arg(model.getName());

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QSqlDatabase::database().commit();
}

ConfigurationModel DatabaseManager::getConfigurationByName(const QString& name)
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT c.Id, c.Name, c.Value FROM Configuration c "
                            "WHERE c.Name = '%1'").arg(name);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    sql.first();

    return ConfigurationModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString());
}

QList<FormatModel> DatabaseManager::getFormat()
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT f.Id, f.Name, f.Width, f.Height, f.FramesPerSecond FROM Format f");

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

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
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    sql.first();

    return FormatModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toInt(), sql.value(3).toInt(), sql.value(4).toString());
}

QList<PresetModel> DatabaseManager::getPreset()
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT p.Id, p.Name, p.Value FROM Preset p");

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

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
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    sql.first();

    return PresetModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString());
}

QList<PresetModel> DatabaseManager::getPresetByFilter(const QString& filter)
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT p.Id, p.Name, p.Value FROM Preset p "
                            "WHERE p.Name LIKE '%%1%' OR p.Value LIKE '%%1%' "
                            "ORDER BY p.Name, p.Id").arg(filter);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

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
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QSqlDatabase::database().commit();
}

void DatabaseManager::deletePreset(int id)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString query = QString("DELETE FROM Preset WHERE Id = %1").arg(id);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QSqlDatabase::database().commit();
}

QList<BlendModeModel> DatabaseManager::getBlendMode()
{
    QMutexLocker locker(&mutex);

    QString query("SELECT b.Id, b.Value FROM BlendMode b");

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

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
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

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
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

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
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

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
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QList<TweenModel> models;
    while (sql.next())
        models.push_back(TweenModel(sql.value(0).toInt(), sql.value(1).toString()));

    return models;
}

QList<OscOutputModel> DatabaseManager::getOscOutput()
{
    QString query("SELECT o.Id, o.Name, o.Address, o.Port, o.Description FROM OscOutput o "
                  "ORDER BY o.Name");

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

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

    QString query = QString("INSERT INTO OscOutput (Name, Address, Port, Description) "
                            "VALUES('%1', '%2', %3, '%4')")
                            .arg(model.getName()).arg(model.getAddress()).arg(model.getPort()).arg(model.getDescription());

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QSqlDatabase::database().commit();
}

OscOutputModel DatabaseManager::getOscOutputByName(const QString& name)
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT o.Id, o.Name, o.Address, o.Port, o.Description FROM OscOutput o "
                            "WHERE o.Name = '%1'").arg(name);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    sql.first();

    return OscOutputModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(),
                          sql.value(3).toInt(), sql.value(4).toString());
}

OscOutputModel DatabaseManager::getOscOutputByAddress(const QString& address)
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT o.Id, o.Name, o.Address, o.Port, o.Description FROM OscOutput o "
                            "WHERE o.Address = '%1'").arg(address);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    sql.first();

    return OscOutputModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(),
                          sql.value(3).toInt(), sql.value(4).toString());
}

void DatabaseManager::updateOscOutput(const OscOutputModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString query = QString("UPDATE OscOutput SET Name = '%1', Address = '%2', Port = %3, Description = '%4' WHERE Id = %5")
                            .arg(model.getName()).arg(model.getAddress()).arg(model.getPort()).arg(model.getDescription()).arg(model.getId());

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QSqlDatabase::database().commit();
}

void DatabaseManager::deleteOscOutput(int id)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString query = QString("DELETE FROM OscOutput WHERE Id = %1").arg(id);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QSqlDatabase::database().commit();
}

QList<AtemStepModel> DatabaseManager::getAtemStep()
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT t.Id, t.Name, t.Value FROM AtemStep t");

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QList<AtemStepModel> models;
    while (sql.next())
        models.push_back(AtemStepModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString()));

    return models;
}

QList<AtemMixerStepModel> DatabaseManager::getAtemMixerStep()
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT t.Id, t.Name, t.Value FROM AtemMixerStep t");

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QList<AtemMixerStepModel> models;
    while (sql.next())
        models.push_back(AtemMixerStepModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString()));

    return models;
}

QList<AtemAudioInputStateModel> DatabaseManager::getAtemAudioInputState()
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT t.Id, t.Name, t.Value FROM AtemAudioInputState t");

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QList<AtemAudioInputStateModel> models;
    while (sql.next())
        models.push_back(AtemAudioInputStateModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString()));

    return models;
}

QList<AtemKeyerModel> DatabaseManager::getAtemKeyer()
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT t.Id, t.Name, t.Value FROM AtemKeyer t");

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QList<AtemKeyerModel> models;
    while (sql.next())
        models.push_back(AtemKeyerModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString()));

    return models;
}

QList<AtemSwitcherModel> DatabaseManager::getAtemSwitcher()
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT t.Id, t.Name, t.Value FROM AtemSwitcher t");

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QList<AtemSwitcherModel> models;
    while (sql.next())
        models.push_back(AtemSwitcherModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString()));

    return models;
}

QList<AtemVideoFormatModel> DatabaseManager::getAtemVideoFormat()
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT t.Id, t.Name, t.Value FROM AtemVideoFormat t");

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QList<AtemVideoFormatModel> models;
    while (sql.next())
        models.push_back(AtemVideoFormatModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString()));

    return models;
}

QList<AtemAutoTransitionModel> DatabaseManager::getAtemAutoTransition()
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT t.Id, t.Name, t.Value FROM AtemAutoTransition t");

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QList<AtemAutoTransitionModel> models;
    while (sql.next())
        models.push_back(AtemAutoTransitionModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString()));

    return models;
}

QList<AtemDeviceModel> DatabaseManager::getAtemDevice()
{
    QString query("SELECT o.Id, o.Name, o.Address, o.Description FROM AtemDevice o "
                  "ORDER BY o.Name");

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QList<AtemDeviceModel> models;
    while (sql.next())
        models.push_back(AtemDeviceModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toString()));

    return models;
}

AtemDeviceModel DatabaseManager::getAtemDeviceByName(const QString& name)
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT d.Id, d.Name, d.Address, d.Description FROM AtemDevice d "
                            "WHERE d.Name = '%1'").arg(name);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    sql.first();

    return AtemDeviceModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toString());
}

AtemDeviceModel DatabaseManager::getAtemDeviceByAddress(const QString& address)
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT d.Id, d.Name, d.Address, d.Description FROM AtemDevice d "
                            "WHERE d.Address = '%1'").arg(address);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    sql.first();

    return AtemDeviceModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toString());
}

void DatabaseManager::insertAtemDevice(const AtemDeviceModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString query = QString("INSERT INTO AtemDevice (Name, Address, Description) "
                            "VALUES('%1', '%2', '%3')")
                            .arg(model.getName()).arg(model.getAddress()).arg(model.getDescription());

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QSqlDatabase::database().commit();
}

void DatabaseManager::updateAtemDevice(const AtemDeviceModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString query = QString("UPDATE AtemDevice SET Name = '%1', Address = '%2', Description = '%3' WHERE Id = %4")
                            .arg(model.getName()).arg(model.getAddress()).arg(model.getDescription()).arg(model.getId());

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QSqlDatabase::database().commit();
}

void DatabaseManager::deleteAtemDevice(int id)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString query = QString("DELETE FROM AtemDevice WHERE Id = %1").arg(id);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QSqlDatabase::database().commit();
}

QList<TriCasterProductModel> DatabaseManager::getTriCasterProduct()
{
    QMutexLocker locker(&mutex);

    QString query("SELECT p.Id, p.Name FROM TriCasterProduct p");

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QList<TriCasterProductModel> models;
    while (sql.next())
        models.push_back(TriCasterProductModel(sql.value(0).toInt(), sql.value(1).toString()));

    return models;
}

QList<TriCasterInputModel> DatabaseManager::getTriCasterInput()
{
    QMutexLocker locker(&mutex);

    QString product = getConfigurationByName("TriCasterProduct").getValue();
    QString query = QString("SELECT t.Id, t.Name, t.Value, t.Products FROM TriCasterInput t "
                            "WHERE t.Products LIKE '%%1%'").arg(product);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QList<TriCasterInputModel> models;
    while (sql.next())
        models.push_back(TriCasterInputModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString()));

    return models;
}

QList<TriCasterStepModel> DatabaseManager::getTriCasterStep()
{
    QMutexLocker locker(&mutex);

    QString product = getConfigurationByName("TriCasterProduct").getValue();
    QString query = QString("SELECT t.Id, t.Name, t.Value, t.Products FROM TriCasterStep t "
                            "WHERE t.Products LIKE '%%1%'").arg(product);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QList<TriCasterStepModel> models;
    while (sql.next())
        models.push_back(TriCasterStepModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toString()));

    return models;
}

QList<TriCasterAutoSpeedModel> DatabaseManager::getTriCasterAutoSpeed()
{
    QMutexLocker locker(&mutex);

    QString product = getConfigurationByName("TriCasterProduct").getValue();
    QString query = QString("SELECT t.Id, t.Name, t.Value, t.Products FROM TriCasterAutoSpeed t "
                            "WHERE t.Products LIKE '%%1%'").arg(product);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QList<TriCasterAutoSpeedModel> models;
    while (sql.next())
        models.push_back(TriCasterAutoSpeedModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toString()));

    return models;
}

QList<TriCasterAutoTransitionModel> DatabaseManager::getTriCasterAutoTransition()
{
    QMutexLocker locker(&mutex);

    QString product = getConfigurationByName("TriCasterProduct").getValue();
    QString query = QString("SELECT t.Id, t.Name, t.Value, t.Products FROM TriCasterAutoTransition t "
                            "WHERE t.Products LIKE '%%1%'").arg(product);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QList<TriCasterAutoTransitionModel> models;
    while (sql.next())
        models.push_back(TriCasterAutoTransitionModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toString()));

    return models;
}

QList<TriCasterPresetModel> DatabaseManager::getTriCasterPreset()
{
    QMutexLocker locker(&mutex);

    QString product = getConfigurationByName("TriCasterProduct").getValue();
    QString query = QString("SELECT t.Id, t.Name, t.Value, t.Products FROM TriCasterPreset t "
                            "WHERE t.Products LIKE '%%1%'").arg(product);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QList<TriCasterPresetModel> models;
    while (sql.next())
        models.push_back(TriCasterPresetModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toString()));

    return models;
}

QList<TriCasterSourceModel> DatabaseManager::getTriCasterSource()
{
    QMutexLocker locker(&mutex);

    QString product = getConfigurationByName("TriCasterProduct").getValue();
    QString query = QString("SELECT t.Id, t.Name, t.Value, t.Products FROM TriCasterSource t "
                            "WHERE t.Products LIKE '%%1%'").arg(product);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QList<TriCasterSourceModel> models;
    while (sql.next())
        models.push_back(TriCasterSourceModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toString()));

    return models;
}

QList<TriCasterSwitcherModel> DatabaseManager::getTriCasterSwitcher()
{
    QMutexLocker locker(&mutex);

    QString product = getConfigurationByName("TriCasterProduct").getValue();
    QString query = QString("SELECT t.Id, t.Name, t.Value, t.Products FROM TriCasterSwitcher t "
                            "WHERE t.Products LIKE '%%1%'").arg(product);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QList<TriCasterSwitcherModel> models;
    while (sql.next())
        models.push_back(TriCasterSwitcherModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toString()));

    return models;
}

QList<TriCasterNetworkTargetModel> DatabaseManager::getTriCasterNetworkTarget()
{
    QMutexLocker locker(&mutex);

    QString product = getConfigurationByName("TriCasterProduct").getValue();
    QString query = QString("SELECT t.Id, t.Name, t.Value, t.Products FROM TriCasterNetworkTarget t "
                            "WHERE t.Products LIKE '%%1%'").arg(product);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QList<TriCasterNetworkTargetModel> models;
    while (sql.next())
        models.push_back(TriCasterNetworkTargetModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toString()));

    return models;
}

QList<TriCasterDeviceModel> DatabaseManager::getTriCasterDevice()
{
    QMutexLocker locker(&mutex);

    QString query("SELECT d.Id, d.Name, d.Address, d.Port, d.Description FROM TriCasterDevice d "
                  "ORDER BY d.Name");

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QList<TriCasterDeviceModel> models;
    while (sql.next())
        models.push_back(TriCasterDeviceModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(),
                                              sql.value(3).toInt(), sql.value(4).toString()));

    return models;
}

TriCasterDeviceModel DatabaseManager::getTriCasterDeviceByName(const QString& name)
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT d.Id, d.Name, d.Address, d.Port, d.Description FROM TriCasterDevice d "
                            "WHERE d.Name = '%1'").arg(name);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    sql.first();

    return TriCasterDeviceModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(),
                                sql.value(3).toInt(), sql.value(4).toString());
}

TriCasterDeviceModel DatabaseManager::getTriCasterDeviceByAddress(const QString& address)
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT d.Id, d.Name, d.Address, d.Port, d.Description FROM TriCasterDevice d "
                            "WHERE d.Address = '%1'").arg(address);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    sql.first();

    return TriCasterDeviceModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(),
                                sql.value(3).toInt(), sql.value(4).toString());
}

void DatabaseManager::insertTriCasterDevice(const TriCasterDeviceModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString query = QString("INSERT INTO TriCasterDevice (Name, Address, Port, Description) "
                            "VALUES('%1', '%2', %3, '%4')")
                            .arg(model.getName()).arg(model.getAddress()).arg(model.getPort()).arg(model.getDescription());

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QSqlDatabase::database().commit();
}

void DatabaseManager::updateTriCasterDevice(const TriCasterDeviceModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString query = QString("UPDATE TriCasterDevice SET Name = '%1', Address = '%2', Port = %3, Description = '%4' WHERE Id = %5")
                            .arg(model.getName()).arg(model.getAddress()).arg(model.getPort()).arg(model.getDescription()).arg(model.getId());

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QSqlDatabase::database().commit();
}

void DatabaseManager::deleteTriCasterDevice(int id)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString query = QString("DELETE FROM TriCasterDevice WHERE Id = %1").arg(id);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QSqlDatabase::database().commit();
}

QList<GpiPortModel> DatabaseManager::getGpiPorts()
{
    QMutexLocker locker(&mutex);

    QString query("SELECT Id, RisingEdge, Action FROM GpiPort ORDER BY Id");

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

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
                            "WHERE Id = %3").arg(Playout::toString(model.getAction())).arg(model.isRisingEdge() ? "1" : "0").arg(model.getPort());

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QSqlDatabase::database().commit();
}

QList<GpoPortModel> DatabaseManager::getGpoPorts()
{
    QMutexLocker locker(&mutex);

    QString query("SELECT Id, RisingEdge, PulseLengthMillis "
                  "FROM GpoPort ORDER BY Id");

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

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
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QSqlDatabase::database().commit();
}

QList<TypeModel> DatabaseManager::getType()
{
    QMutexLocker locker(&mutex);

    QString query("SELECT t.Id, t.Value FROM Type t");

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

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
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    sql.first();

    return TypeModel(sql.value(0).toInt(), sql.value(1).toString());
}

QList<DeviceModel> DatabaseManager::getDevice()
{
    QString query("SELECT d.Id, d.Name, d.Address, d.Port, d.Username, d.Password, d.Description, d.Version, d.Shadow, d.Channels, d.ChannelFormats, d.PreviewChannel FROM Device d "
                  "ORDER BY d.Name");

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QList<DeviceModel> models;
    while (sql.next())
        models.push_back(DeviceModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toInt(),
                                     sql.value(4).toString(), sql.value(5).toString(), sql.value(6).toString(), sql.value(7).toString(),
                                     sql.value(8).toString(), sql.value(9).toInt(), sql.value(10).toString(), sql.value(11).toInt()));

    return models;
}

DeviceModel DatabaseManager::getDeviceByName(const QString& name)
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT d.Id, d.Name, d.Address, d.Port, d.Username, d.Password, d.Description, d.Version, d.Shadow, d.Channels, d.ChannelFormats, d.PreviewChannel FROM Device d "
                            "WHERE d.Name = '%1'").arg(name);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    sql.first();

    return DeviceModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toInt(),
                       sql.value(4).toString(), sql.value(5).toString(), sql.value(6).toString(), sql.value(6).toString(),
                       sql.value(8).toString(), sql.value(9).toInt(), sql.value(10).toString(), sql.value(11).toInt());
}

DeviceModel DatabaseManager::getDeviceByAddress(const QString& address)
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT d.Id, d.Name, d.Address, d.Port, d.Username, d.Password, d.Description, d.Version, d.Shadow, d.Channels, d.ChannelFormats, d.PreviewChannel FROM Device d "
                            "WHERE d.Address = '%1'").arg(address);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    sql.first();

    return DeviceModel(sql.value(0).toInt(), sql.value(1).toString(), sql.value(2).toString(), sql.value(3).toInt(),
                       sql.value(4).toString(), sql.value(5).toString(), sql.value(6).toString(), sql.value(6).toString(),
                       sql.value(8).toString(), sql.value(9).toInt(), sql.value(10).toString(), sql.value(11).toInt());
}

void DatabaseManager::insertDevice(const DeviceModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString query = QString("INSERT INTO Device (Name, Address, Port, Username, Password, Description, Version, Shadow, Channels, ChannelFormats, PreviewChannel) "
                            "VALUES('%1', '%2', %3, '%4', '%5', '%6', '%7', '%8', %9, '%10', %11)")
                            .arg(model.getName()).arg(model.getAddress()).arg(model.getPort()).arg(model.getUsername())
                            .arg(model.getPassword()).arg(model.getDescription()).arg(model.getVersion()).arg(model.getShadow())
                            .arg(model.getChannels()).arg(model.getChannelFormats()).arg(model.getPreviewChannel());

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QSqlDatabase::database().commit();
}

void DatabaseManager::updateDevice(const DeviceModel& model)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString query = QString("UPDATE Device SET Name = '%1', Address = '%2', Port = %3, Username = '%4', Password = '%5', Description = '%6', Version = '%7', Shadow = '%8', Channels = %9, ChannelFormats = '%10', PreviewChannel = %11 "
                            "WHERE Id = %12")
                            .arg(model.getName()).arg(model.getAddress()).arg(model.getPort()).arg(model.getUsername())
                            .arg(model.getPassword()).arg(model.getDescription()).arg(model.getVersion()).arg(model.getShadow())
                            .arg(model.getChannels()).arg(model.getChannelFormats()).arg(model.getPreviewChannel())
                            .arg(model.getId());

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

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
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

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
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

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
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QSqlDatabase::database().commit();
}

void DatabaseManager::deleteDevice(int id)
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString query = QString("DELETE FROM Device WHERE Id = %1").arg(id);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    query = QString("DELETE FROM Thumbnail WHERE Id IN (SELECT l.ThumbnailId FROM Library l WHERE DeviceId = %1)").arg(id);

    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    query = QString("DELETE FROM Library WHERE DeviceId = %1").arg(id);

    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

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
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

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
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

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
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

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
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

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
                        "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND l.TypeId = 5 AND l.Name LIKE '%%1%' "
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
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

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
                        "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND l.TypeId = 2 AND l.Name LIKE '%%1%' "
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
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

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
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

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

    QString query;

    if (filter.isEmpty())
    {
        query = QString("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                        "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND d.Id = %1 "
                        "ORDER BY l.Name, l.DeviceId").arg(deviceId);
    }
    else // Filter.
    {
        query = QString("SELECT l.Id, l.Name, d.Name, t.Value, l.ThumbnailId, l.Timecode FROM Library l, Device d, Type t "
                        "WHERE l.DeviceId = d.Id AND l.TypeId = t.Id AND d.Id = %1 AND l.Name LIKE '%%2%' "
                        "ORDER BY l.Name, l.DeviceId").arg(deviceId).arg(filter);
    }

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

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
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

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
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

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
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

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
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

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
               qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());
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
               qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());
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
               qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());
        }
    }

    if (insertModels.count() > 0)
    {
        for (int i = 0; i < insertModels.count(); i++)
        {
            QString query = QString("INSERT INTO Library (Name, DeviceId, TypeId, ThumbnailId, Timecode) VALUES('%1', %2, %3, %4, '%5')")
                                    .arg(insertModels.at(i).getName()).arg(deviceId).arg(typeId).arg(insertModels.at(i).getThumbnailId()).arg(insertModels.at(i).getTimecode());

            if (!sql.exec(query))
               qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());
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
               qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

            query = QString("DELETE FROM Library WHERE Id = %1 AND TypeId = 2").arg(deleteModels.at(i).getId());

            if (!sql.exec(query))
               qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());
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
               qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());
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
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    QSqlDatabase::database().commit();
}

QList<ThumbnailModel> DatabaseManager::getThumbnailByDeviceAddress(const QString& address)
{
    QMutexLocker locker(&mutex);

    QString query = QString("SELECT t.Id, t.Data, t.Timestamp, t.Size, l.Name, d.Address FROM Thumbnail t, Library l, Device d "
                            "WHERE d.Address = '%1' AND l.DeviceId = d.Id AND l.ThumbnailId = t.Id").arg(address);

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

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
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

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
                   qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());
            }
            else
            {
                QString query = QString("INSERT INTO Thumbnail (Data, Timestamp, Size) VALUES('%1', '%2', '%3')").arg(model.getData()).arg(model.getTimestamp()).arg(model.getSize());

                if (!sql.exec(query))
                   qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

                query = QString("UPDATE Library SET ThumbnailId = %1 WHERE Id = %2").arg(sql.lastInsertId().toInt()).arg(libraryModel.getId());

                if (!sql.exec(query))
                   qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());
            }
        }
    }

    QSqlDatabase::database().commit();
}

void DatabaseManager::deleteThumbnails()
{
    QMutexLocker locker(&mutex);

    QSqlDatabase::database().transaction();

    QString query = QString("DELETE FROM Thumbnail");

    QSqlQuery sql;
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());

    const QList<LibraryModel>& models = this->getLibraryMedia();
    for (int i = 0; i < models.count(); i++)
    {
        const LibraryModel& model = models.at(i);
        if (model.getThumbnailId() > 0)
        {
            QString query = QString("UPDATE Library SET ThumbnailId = 0 WHERE Id = %1").arg(model.getId());

            if (!sql.exec(query))
               qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());
        }
    }

    QSqlDatabase::database().commit();

    // Shrink file on disk.
    query = QString("VACUUM Thumbnail");
    if (!sql.exec(query))
       qCritical() << QString("Failed to execute: %1, Error: %2").arg(sql.lastQuery()).arg(sql.lastError().text());
}
