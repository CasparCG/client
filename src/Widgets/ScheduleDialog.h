#pragma once

#include "Shared.h"
#include "ui_ScheduleDialog.h"

#include "Models/ScheduleModel.h"
#include "Models/PresetModel.h"

#include <QtWidgets/QDialog>

#include <QtCore/QList>
#include <QtCore/QStringListModel>
#include <QtCore/QModelIndex>
#include <QtCore/QTime>
#include <QtCore/QItemSelection>
#include <QtGui/QCloseEvent>
#include <QtGui/QStandardItemModel>


class WIDGETS_EXPORT ScheduleDialog : public QDialog, Ui::ScheduleDialog
{
    Q_OBJECT

    public:
        explicit ScheduleDialog(QWidget *parent = 0);

        const QList<ScheduleModel>& getAddedSchedule() const;
        const QList<ScheduleModel>& getUpdatedSchedule() const;
        const QList<ScheduleModel>& getRemovedSchedule() const;

    protected:
        void accept();
        void reject();
        void closeEvent(QCloseEvent *event);

    private:

        enum class ScheduleType {
            NoSchedule,
            DefaultSchedule,
            AddedSchedule,
            UpdatedSchedule,
            RemovedSchedule
        };

        QStandardItemModel* listScheduleContent;
        QStringListModel* listScheduleModel;
        QStringListModel* listPresetModel;

        ScheduleModel selectedSchedule = ScheduleModel(0,"");
        ScheduleType scheduleType = ScheduleType::NoSchedule;

        int selectedPresetRow;
        bool updateTriggered = false;
        bool preInput = false;

        QList<ScheduleModel> scheduleList;
        QList<PresetModel> presetList;

        QList<ScheduleModel> addedSchedule;
        QList<ScheduleModel> removedSchedule;
        QList<ScheduleModel> updatedSchedule;

        void setVisibleScheduleContainer(bool visible);

        void addNewSchedule(ScheduleModel model);
        void removeSchedule(const QString& name);
        void showScheduleToView(ScheduleModel model);
        void storeSelectedSchedule();
        void triggerScheduleUpdated();
        void clearScheduleContent();

        // Notice there's two constructor doing the same thing, gonna combine that later
        void updateScheduleTimeline();
        bool findScheduleInList(const QString& name);
        void setScheduleActivated(bool active);
        void refreshScheduleList();
        bool scheduleExistInList(const QString& name);

        QTime getPresetDuration(const PresetModel& model) const;
        void addPresetToScheduleContent(const PresetModel& model);
        void removePresetFromScheduleContent(int row);
        void refreshPresetList();
        void matchPresetListWithScheduleContent();


        Q_SLOT void getSelectedPreset(const QModelIndex& index);
        Q_SLOT void getSelectedSchedule(const QModelIndex& index);

        Q_SLOT void beganTimeChanged(const QTime& time);
        Q_SLOT void scheduleSelectedChanged(const QItemSelection& selection);
        Q_SLOT void presetSelectedChanged(const QItemSelection& selection);

        Q_SLOT void scheduleContentCustomMenu(QPoint pos);
        Q_SLOT void removePresetSelected();

        Q_SLOT void addScheduleButton(bool checked = false);
        Q_SLOT void removeScheduleButton(bool checked = false);
        Q_SLOT void addPresetButton(bool checked = false);
        // This button could be used with one slot, but the odds are to big that the selected schedule activated
        // state to change unexpectedly
        Q_SLOT void activateScheduleButton(bool checked = false);
        Q_SLOT void deactivateScheduleButton(bool checked = false);

};
