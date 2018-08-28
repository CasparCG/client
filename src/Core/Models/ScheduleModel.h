#pragma once

#include "../Shared.h"
#include "../Models/PresetModel.h"

#include <QtCore/QString>
#include <QList>
#include <QDateTime>

class CORE_EXPORT ScheduleModel
{
    public:
        explicit ScheduleModel(int id, const QString& name);
        explicit ScheduleModel(int id, const QString& name, const QDateTime& timeBegan);
        explicit ScheduleModel(int id, const QString& name, const QDateTime& timeBegan, const QDateTime& timeEnds);
        explicit ScheduleModel(int id, const QString& name, const QDateTime& timeBegan, const QDateTime& timeEnds, bool active);

        int getId() const;
        const QString& getName() const;
        const QDateTime& getTimelineBegan() const;
        const QDateTime& getTimelineEnd() const;
        const QTime& getDuration() const;
        bool getActivated() const;
        const QList<PresetModel>& getPresetList() const;

        void calculateDuration();
        void calculateDurationFromCurrentDateTime();

        void setDateTimeBegan(const QDateTime& setTime);
        void setActivated(bool active);
        void addPresetToList(const PresetModel& preset);
        bool removePresetFromList(const QString& name);
        bool removePresetFromList(int id);

        void clearAllPresetInList();

        friend inline bool operator==(const ScheduleModel &s1, const ScheduleModel &s2){
            if (s1.getName() == s2.getName())
                return true;

            return false;
        }

    private:
        int id;
        QString name;
        QDateTime began;
        QDateTime end;
        bool activated;
        QTime duration;

        QList<PresetModel> presetslist;

        void DurationCalculator(const QDateTime& setTime);
};
