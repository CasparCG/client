#include "ScheduleModel.h"

#include <QXmlStreamReader>
#include <QTime>

ScheduleModel::ScheduleModel(int id, const QString& name)
    : id(id), name(name)
{
    this->began = QDateTime::currentDateTime();
    this->end = QDateTime::currentDateTime();
    this->activated = false;
}

ScheduleModel::ScheduleModel(int id, const QString &name, const QDateTime &timeBegan)
    : id(id), name(name), began(timeBegan)
{
    this->end = QDateTime::currentDateTime();
    this->activated = false;
}

ScheduleModel::ScheduleModel(int id, const QString &name, const QDateTime &timeBegan, const QDateTime &timeEnds)
    : id(id), name(name), began(timeBegan), end(timeEnds)
{
    this->activated = false;
}

ScheduleModel::ScheduleModel(int id, const QString &name, const QDateTime &timeBegan, const QDateTime &timeEnds, bool active)
    : id(id), name(name), began(timeBegan), end(timeEnds), activated(active)
{

}

int ScheduleModel::getId() const
{
    return this->id;
}

const QString& ScheduleModel::getName() const
{
    return this->name;
}

const QDateTime& ScheduleModel::getTimelineBegan() const
{
    return this->began;
}

const QDateTime& ScheduleModel::getTimelineEnd() const
{
    return this->end;
}

const QTime& ScheduleModel::getDuration() const
{
    return this->duration;
}

bool ScheduleModel::getActivated() const
{
    return this->activated;
}

const QList<PresetModel>& ScheduleModel::getPresetList() const
{
    return this->presetslist;
}

void ScheduleModel::calculateDuration()
{
    DurationCalculator(this->began);
}

void ScheduleModel::calculateDurationFromCurrentDateTime()
{
    DurationCalculator(QDateTime::currentDateTime());
}

void ScheduleModel::setDateTimeBegan(const QDateTime& setTime)
{
    this->began = QDateTime(setTime);
}

void ScheduleModel::setActivated(bool active)
{
    this->activated = active;
}

void ScheduleModel::addPresetToList(const PresetModel &preset)
{
    this->presetslist.push_back(preset);
}

bool ScheduleModel::removePresetFromList(const QString& name)
{
    int index = 0;

    foreach(PresetModel model, this->presetslist) {
        if (model.getName() == name) {
            break;
        }
        else {
            index++;
        }
    }

    if (index <= (this->presetslist.size() - 1)) {
        this->presetslist.removeAt(index);
        return true;
    }
    return false;
}

bool ScheduleModel::removePresetFromList(int id)
{
    int index = 0;

    foreach(PresetModel model, this->presetslist) {
        if (model.getId() == id) {
            break;
        }
        else {
            index++;
        }
    }

    if (index <= (this->presetslist.size() - 1)) {
        this->presetslist.removeAt(index);
        return true;
    }
    return false;
}

void ScheduleModel::clearAllPresetInList()
{
    this->presetslist.clear();
}

void ScheduleModel::DurationCalculator(const QDateTime& setTime)
{
    bool passFirst = false;

    QTime totalTime(0,0,0);
    QTime tempTime;

    foreach (PresetModel model, this->presetslist) {
        QXmlStreamReader* xmlReader = new QXmlStreamReader(model.getValue());

        while(!xmlReader->atEnd() && !xmlReader->hasError()) {
                // Read next element
                QXmlStreamReader::TokenType token = xmlReader->readNext();
                // If token is just StartDocument - go to next
                if(token == QXmlStreamReader::StartDocument) {
                        continue;
                }
                // If token is StartElement - read it
                if(token == QXmlStreamReader::StartElement) {
                        if(xmlReader->name() == "timecode") {
                            if (!passFirst) {
                                totalTime = QTime::fromString(xmlReader->readElementText());
                                passFirst = true;
                            }
                            else {
                                tempTime = QTime::fromString(xmlReader->readElementText());
                                totalTime = totalTime.addSecs(((totalTime.hour() + tempTime.hour()) * 3600)
                                                              + ((totalTime.minute() + tempTime.minute()) * 60)
                                                                + (totalTime.second() + tempTime.second()));
                            }
                        }
                }
        }

        delete xmlReader;
    }


    this->duration = QTime(totalTime.hour(), totalTime.minute(), totalTime.second());
    // Add padding of 1 second for a safe end frame
    this->end = QDateTime(setTime).addSecs((totalTime.hour() * 3600) + (totalTime.minute() * 60) + totalTime.second() + 1);

}
