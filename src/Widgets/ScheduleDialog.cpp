#include "ScheduleDialog.h"
#include "DatabaseManager.h"

#include "PresetDialog.h"

#include <QtCore/QStringList>
#include <QtCore/QXmlStreamReader>
#include <QtWidgets/QHeaderView>
#include <QMessageBox>
#include <QtGui/QStandardItem>
#include <QtWidgets/QMenu>

#pragma GCC diagnostic ignored "-Wcovered-switch-default"

/*
 * Features that would be added at the future (TO-DO)
 *
 * - Making changes undoable (Before apply changes)
 * - Support single-media content to be added to schedule
 * - Recursive schedules based on the previous schedule before the selected
 * - Adding option to schedule by days (Eg. Monday, Thursday, Wednesday and so on)
 *
 *
 */

ScheduleDialog::ScheduleDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    // =================================
    // Setup Schedule Content QTableView
    this->listScheduleContent = new QStandardItemModel();
    this->scheduleTableView->setModel(listScheduleContent);

    this->listScheduleContent->setHorizontalHeaderItem(0, new QStandardItem("Preset Name"));
    this->listScheduleContent->setHorizontalHeaderItem(1, new QStandardItem("Duration"));

    this->scheduleTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    this->scheduleTableView->horizontalHeader()->resizeSection(0, 300);
    this->scheduleTableView->horizontalHeader()->resizeSection(1, (this->scheduleTableView->width() - 300));

    this->scheduleTableView->setContextMenuPolicy(Qt::CustomContextMenu);

    // ============================
    // Setup Schedule & Preset list

    this->scheduleList = QList<ScheduleModel>(DatabaseManager::getInstance().getScheduledProgram());
    this->presetList = QList<PresetModel>(DatabaseManager::getInstance().getPreset());

    this->addedSchedule = QList<ScheduleModel>();
    this->removedSchedule = QList<ScheduleModel>();
    this->updatedSchedule = QList<ScheduleModel>();

    // ====================================
    // Setup up Schedule & Preset QListView

    QStringList lvContainer = QStringList();

    if (!this->scheduleList.isEmpty()) {
        foreach (ScheduleModel scheduleMdl, this->scheduleList){
            lvContainer.append(scheduleMdl.getName());
        }
    }
    else {
        lvContainer.append("Click to create new");
    }

    this->listScheduleModel = new QStringListModel(lvContainer);
    this->scheduleListView->setModel(listScheduleModel);

    // -----

    QStringList prsName = QStringList();

    foreach (PresetModel mdl, this->presetList){
        prsName.append(mdl.getName());
    }

    this->listPresetModel = new QStringListModel(prsName);
    this->presetsListView->setModel(this->listPresetModel);

    // =========
    // Pre-setup

    this->refreshPresetList();
    this->setVisibleScheduleContainer(false);
    this->scheduleRemoveList->setEnabled(false);
    this->presetsAddButton->setEnabled(false);

    this->confirmButton->addButton("Save Schedule", QDialogButtonBox::AcceptRole);
    this->confirmButton->addButton(QDialogButtonBox::Cancel);

    // ==============
    // Connect Signal

    QObject::connect(this->scheduleListView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(getSelectedSchedule(const QModelIndex&)));
    QObject::connect(this->presetsListView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(getSelectedPreset(const QModelIndex&)));

    QObject::connect(this->scheduleTableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(scheduleContentCustomMenu(QPoint)));
    QObject::connect(this->scheduleBeganTime, SIGNAL(timeChanged(const QTime&)), this, SLOT(beganTimeChanged(const QTime&)));

    QObject::connect(this->scheduleAddList, SIGNAL(clicked(bool)), this, SLOT(addScheduleButton(bool)));
    QObject::connect(this->scheduleRemoveList, SIGNAL(clicked(bool)), this, SLOT(removeScheduleButton(bool)));
    QObject::connect(this->presetsAddButton, SIGNAL(clicked(bool)), this, SLOT(addPresetButton(bool)));
    QObject::connect(this->scheduleActivateButton, SIGNAL(clicked(bool)), this, SLOT(activateScheduleButton(bool)));
    QObject::connect(this->scheduleDeactivateButton, SIGNAL(clicked(bool)), this, SLOT(deactivateScheduleButton(bool)));

    QObject::connect(this->scheduleListView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                     this, SLOT(scheduleSelectedChanged(QItemSelection)));
    QObject::connect(this->presetsListView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                     this, SLOT(presetSelectedChanged(QItemSelection)));

    // Confirm button signal
    QObject::connect(this->confirmButton, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect(this->confirmButton, SIGNAL(rejected()), this, SLOT(reject()));
}

const QList<ScheduleModel>& ScheduleDialog::getAddedSchedule() const
{
    return this->addedSchedule;
}

const QList<ScheduleModel>& ScheduleDialog::getUpdatedSchedule() const
{
    return this->updatedSchedule;
}

const QList<ScheduleModel>& ScheduleDialog::getRemovedSchedule() const
{
    return this->removedSchedule;
}

void ScheduleDialog::setScheduleActivated(bool active)
{
    if (active) {
        this->scheduleActivated->setText("Schedule is Activated (On Query)");
        this->scheduleActivated->setStyleSheet("QLabel { color : LimeGreen; }");

        this->scheduleActivateButton->setEnabled(false);
        this->scheduleDeactivateButton->setEnabled(true);
    }
    else {
        this->scheduleActivated->setText("Schedule not Activated (Off Query)");
        this->scheduleActivated->setStyleSheet("QLabel { color : white; }");

        this->scheduleActivateButton->setEnabled(true);
        this->scheduleDeactivateButton->setEnabled(false);
    }
}

void ScheduleDialog::setVisibleScheduleContainer(bool visible)
{
    if (visible) {
        this->scheduleContentContainer->show();
        this->schedulerContainer->show();
        this->selectedScheduleNameLabel->setText(QString("Currently editing schedule '%1'").arg(this->selectedSchedule.getName()));
    }
    else {
        this->scheduleContentContainer->hide();
        this->schedulerContainer->hide();
        this->selectedScheduleNameLabel->setText("Select Schedule on Schedule list to start editing");
    }
}

void ScheduleDialog::refreshScheduleList()
{
    QStringList lvContainer = this->listScheduleModel->stringList();
    lvContainer.clear();

    foreach (ScheduleModel scheduleMdl, this->scheduleList){
        if (!removedSchedule.contains(scheduleMdl))
            lvContainer.append(scheduleMdl.getName());
    }

    foreach (ScheduleModel addedMdl, this->addedSchedule) {
        lvContainer.append(QString("%1*").arg(addedMdl.getName()));
    }

    foreach (ScheduleModel updatedMdl, this->updatedSchedule) {
        int index = lvContainer.indexOf(updatedMdl.getName());
        if (index != -1) {
            lvContainer[index] += "*";
        }
    }

    this->listScheduleModel->setStringList(lvContainer);
    this->scheduleRemoveList->setEnabled(false);
}

bool ScheduleDialog::scheduleExistInList(const QString &name)
{
    if (!this->updatedSchedule.isEmpty())
    {
        foreach (ScheduleModel scheduleMdl, this->updatedSchedule){
            if (scheduleMdl.getName() == name) {
                return true;
            }
        }
    }

    if (!this->addedSchedule.isEmpty()) {
        foreach (ScheduleModel scheduleMdl, this->addedSchedule){
            if (scheduleMdl.getName() == name) {
                return true;
            }
        }
    }

    if (!this->scheduleList.isEmpty())
    {
        foreach (ScheduleModel scheduleMdl, this->scheduleList){
            if (scheduleMdl.getName() == name) {
                return true;
            }
        }
    }

    return false;
}

QTime ScheduleDialog::getPresetDuration(const PresetModel &model) const
{
    QTime totalTime(0,0);
    QTime tempTime;

    bool passFirst = false;

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
    return totalTime;
}

void ScheduleDialog::addPresetToScheduleContent(const PresetModel &model)
{
    QList<QStandardItem *> presets = QList<QStandardItem *>();

    QStandardItem* name = new QStandardItem(model.getName());
    name->setFlags(Qt::ItemIsSelectable);
    presets.append(name);
    QStandardItem* duration = new QStandardItem(this->getPresetDuration(model).toString());
    duration->setFlags(Qt::ItemIsSelectable);
    presets.append(duration);

    dynamic_cast<QStandardItemModel*>(this->scheduleTableView->model())->appendRow(presets);
}

void ScheduleDialog::removePresetFromScheduleContent(int row)
{
    dynamic_cast<QStandardItemModel*>(this->scheduleTableView->model())->removeRow(row);
}

void ScheduleDialog::refreshPresetList()
{
    QStringList prsName = this->listPresetModel->stringList();
    prsName.clear();

    foreach (PresetModel mdl, this->presetList){
        prsName.append(mdl.getName());
    }

    this->listPresetModel->setStringList(prsName);
    this->presetsAddButton->setEnabled(false);
}

void ScheduleDialog::matchPresetListWithScheduleContent()
{
    if (dynamic_cast<QStringListModel*>(this->presetsListView->model())->rowCount() != 0) {
        int index = 0;
        foreach (QString str, dynamic_cast<QStringListModel*>(this->presetsListView->model())->stringList()) {
            if (!dynamic_cast<QStandardItemModel*>(this->scheduleTableView->model())->findItems(str, Qt::MatchExactly, 0).isEmpty()) {
                dynamic_cast<QStringListModel*>(this->presetsListView->model())->removeRow(index);
            }
            else {
                index++;
            }
        }
    }
}

void ScheduleDialog::addNewSchedule(ScheduleModel model)
{
    // Trigger schedule updated
    if (!this->updateTriggered)
        this->updateTriggered = true;

    this->addedSchedule.append(model);
    this->refreshScheduleList();
    if (scheduleType == ScheduleType::NoSchedule) {
        preInput = true;
        this->findScheduleInList(model.getName());
        preInput = false;
    }
}

void ScheduleDialog::removeSchedule(const QString& name)
{
    bool removed = false;

    if (name.contains("*")) {
        QString trimmedName = QString(name).remove("*");

        foreach (ScheduleModel model, this->addedSchedule) {
            if (model.getName() == trimmedName) {
                this->addedSchedule.removeAt(this->addedSchedule.indexOf(model));
                removed = true;
                break;
            }
        }

        if (!removed) {
            foreach (ScheduleModel model, this->updatedSchedule) {
                if (model.getName() == trimmedName) {
                    this->updatedSchedule.removeAt(this->updatedSchedule.indexOf(model));
                    this->removedSchedule.append(model);
                    removed = true;
                    break;
                }
            }
        }
    }
    else {
        foreach (ScheduleModel model, this->scheduleList) {
            if (model.getName() == name) {
                this->removedSchedule.append(model);
                removed = true;
                break;
            }
        }
    }

    if (removed) {
        if (this->selectedSchedule.getName() == (name.contains("*") ? QString(name).remove("*") : name)) {
            this->selectedSchedule = ScheduleModel(0, "");
            this->scheduleType = ScheduleType::NoSchedule;
            this->setVisibleScheduleContainer(false);
        }
        this->refreshScheduleList();
    }

}

void ScheduleDialog::showScheduleToView(ScheduleModel model)
{
    this->clearScheduleContent();
    QList<PresetModel> presetList = model.getPresetList();

    if (!presetList.isEmpty()) {
        foreach (PresetModel prsMdl, presetList){
            this->addPresetToScheduleContent(prsMdl);
        }
        this->matchPresetListWithScheduleContent();
    }
    this->scheduleBeganTime->setTime(model.getTimelineBegan().time());
    this->setScheduleActivated(model.getActivated());

    this->selectedSchedule = model;

    this->setVisibleScheduleContainer(true);
    this->updateScheduleTimeline();
}

void ScheduleDialog::storeSelectedSchedule()
{
    if (scheduleType == ScheduleType::NoSchedule)
        return;

    switch (scheduleType) {
        case ScheduleType::UpdatedSchedule:
            if (this->updatedSchedule.contains(this->selectedSchedule)) {
                this->updatedSchedule[this->updatedSchedule.indexOf(this->selectedSchedule)] = this->selectedSchedule;
            }
            else {
                this->updatedSchedule.append(this->selectedSchedule);
            }
            break;
        case ScheduleType::AddedSchedule:
            this->addedSchedule[this->addedSchedule.indexOf(this->selectedSchedule)] = this->selectedSchedule;
            break;
        case ScheduleType::RemovedSchedule:
        case ScheduleType::DefaultSchedule:
        case ScheduleType::NoSchedule:
        default:
            break;
    }
}

void ScheduleDialog::triggerScheduleUpdated()
{
    if (!this->updateTriggered)
        this->updateTriggered = true;

    if (this->scheduleType == ScheduleType::DefaultSchedule) {
        this->scheduleType = ScheduleType::UpdatedSchedule;
        this->storeSelectedSchedule();
        this->refreshScheduleList();
    }
}

void ScheduleDialog::clearScheduleContent()
{
    int rowCount = dynamic_cast<QStandardItemModel*>(this->scheduleTableView->model())->rowCount();
    dynamic_cast<QStandardItemModel*>(this->scheduleTableView->model())->removeRows(0, rowCount);
}

void ScheduleDialog::updateScheduleTimeline()
{
    if (scheduleType == ScheduleType::NoSchedule)
        return;

    this->selectedSchedule.calculateDuration();

    this->scheduleEndTime->setTime(this->selectedSchedule.getTimelineEnd().time());

    // Trigger if one of the time component are not zero
    if (this->selectedSchedule.getDuration().second() != 0 ||
            this->selectedSchedule.getDuration().minute() != 0 ||
            this->selectedSchedule.getDuration().hour() != 0)
    {
        QString longNameDuration;
        // Determine who trigger the condition and assign its value
        if (this->selectedSchedule.getDuration().hour() != 0) {
            longNameDuration += QString("%1 Hour ").arg(QString::number(this->selectedSchedule.getDuration().hour()));
        }
        if (this->selectedSchedule.getDuration().minute() != 0) {
            longNameDuration += QString("%1 Minute ").arg(QString::number(this->selectedSchedule.getDuration().minute()));
        }
        if (this->selectedSchedule.getDuration().second() != 0) {
            longNameDuration += QString("%1 Second").arg(QString::number(this->selectedSchedule.getDuration().second()));
        }

        this->scheduleDuration->setText(QString("%1 (%2)").arg(longNameDuration).arg(this->selectedSchedule.getDuration().toString()));
    }
    else {
        this->scheduleDuration->setText("0 Second (Empty Content)");
    }
}



bool ScheduleDialog::findScheduleInList(const QString& name)
{
    // Find schedule exactly match with name's end of character (w/ *)

    if (name.contains("*")) {
        QString trimmedName = QString(name).remove((name.length() - 1), 1);

        if (!this->updatedSchedule.isEmpty())
        {
            foreach (ScheduleModel scheduleMdl, this->updatedSchedule){
                if (scheduleMdl.getName() == trimmedName) {
                    this->scheduleType = ScheduleType::UpdatedSchedule;
                    this->showScheduleToView(scheduleMdl);
                    return true;
                }
            }
        }

        if (!this->addedSchedule.isEmpty()) {
            foreach (ScheduleModel scheduleMdl, this->addedSchedule){
                if (scheduleMdl.getName() == trimmedName) {
                    this->scheduleType = ScheduleType::AddedSchedule;
                    this->showScheduleToView(scheduleMdl);
                    return true;
                }
            }
        }
    }
    else {
        if (!this->scheduleList.isEmpty())
        {
            foreach (ScheduleModel scheduleMdl, this->scheduleList){
                if (scheduleMdl.getName() == name) {
                    this->scheduleType = ScheduleType::DefaultSchedule;
                    this->showScheduleToView(scheduleMdl);
                    return true;
                }
            }
        }
    }

    return false;
}

void ScheduleDialog::getSelectedPreset(const QModelIndex &index)
{

    if (scheduleType == ScheduleType::NoSchedule)
        return;

    QString name = index.data().toString();

    foreach (PresetModel model, this->presetList){
        if (model.getName() == name) {
            this->addPresetToScheduleContent(model);
            this->selectedSchedule.addPresetToList(model);
            this->updateScheduleTimeline();
            this->matchPresetListWithScheduleContent();

            // Trigger update
            this->triggerScheduleUpdated();
            break;
        }
    }
}

void ScheduleDialog::getSelectedSchedule(const QModelIndex &index)
{
    QString name = index.data().toString();

    if (name == "Click to create new") {
        // Open a new dialog (Using PresetDialog class for temporary, too lazy to make one lul) to prompt schedule name
        PresetDialog* dialog = new PresetDialog(this);
        if (dialog->exec() == QDialog::Accepted) {
            this->addNewSchedule(ScheduleModel(0, dialog->getName()));
        }

        return;
    }

    // Confirm if selected schedule isn't selected yet
    if (this->selectedSchedule.getName() == QString(name).remove("*")) {
        return;
    }

    // Store the schedule first, if not empty
    if (scheduleType != ScheduleType::NoSchedule)
        this->storeSelectedSchedule();

    this->refreshPresetList();
    preInput = true;

    if (this->findScheduleInList(name)) {
        preInput = false;
        return;
    }

    QMessageBox msgBox(QMessageBox::Critical,
                       "Error retrieving schedule", QString("The schedule named '%1' cannot be found!").arg(name),
                       QMessageBox::Ok, this);
    msgBox.exec();
}

void ScheduleDialog::beganTimeChanged(const QTime &time)
{
    if (preInput)
        return;

    this->selectedSchedule.setDateTimeBegan(QDateTime(QDate::currentDate(), time));
    this->updateScheduleTimeline();

    // Trigger update
    this->triggerScheduleUpdated();
}

void ScheduleDialog::scheduleSelectedChanged(const QItemSelection& selection)
{
    if (!selection.indexes().isEmpty()) {
        this->scheduleRemoveList->setEnabled(true);
    }
    else {
        this->scheduleRemoveList->setEnabled(false);
    }
}

void ScheduleDialog::presetSelectedChanged(const QItemSelection &selection)
{
    if (!selection.indexes().isEmpty()) {
        this->presetsAddButton->setEnabled(true);
    }
    else {
        this->presetsAddButton->setEnabled(false);
    }
}

void ScheduleDialog::scheduleContentCustomMenu(QPoint pos)
{
    QModelIndex index = this->scheduleTableView->indexAt(pos);

    QMenu *menu = new QMenu(this);

    // Determine if the mouse position is on existing cells
    if (!index.data().toString().isEmpty()) {
        QString presetName = dynamic_cast<QStandardItemModel*>(this->scheduleTableView->model())->item(index.row(), 0)->text();
        menu->addAction(QString("Remove '%1'").arg(presetName), this, SLOT(removePresetSelected()));
        this->scheduleTableView->selectRow(index.row());
        this->selectedPresetRow = index.row();
    }
    else {
        menu->addAction(new QAction("Remove Preset", this));
        menu->actions().at(0)->setEnabled(false);
    }

    menu->popup(this->scheduleTableView->viewport()->mapToGlobal(pos));
}

void ScheduleDialog::removePresetSelected()
{
    QString presetName = dynamic_cast<QStandardItemModel*>(this->scheduleTableView->model())->item(this->selectedPresetRow, 0)->text();

    this->removePresetFromScheduleContent(this->selectedPresetRow);
    this->refreshPresetList();
    this->matchPresetListWithScheduleContent();

    this->selectedSchedule.removePresetFromList(presetName);
    this->updateScheduleTimeline();
}

void ScheduleDialog::addScheduleButton(bool checked)
{
    Q_UNUSED(checked);

    // Open a new dialog (Using PresetDialog class for temporary, too lazy to make one lul) to prompt schedule name
    PresetDialog* dialog = new PresetDialog(this);
    while (dialog->exec() == QDialog::Accepted) {
        if (this->scheduleExistInList(dialog->getName())){
            QMessageBox msgBox(QMessageBox::Critical,
                               "Error adding schedule", QString("The schedule named '%1' already exist in list! Please pick another name").arg(dialog->getName()),
                               QMessageBox::Ok, this);
            msgBox.exec();
            continue;
        }

        this->addNewSchedule(ScheduleModel(0, dialog->getName()));
        break;
    }
}

void ScheduleDialog::removeScheduleButton(bool checked)
{
    Q_UNUSED(checked);

    QString name = this->scheduleListView->currentIndex().data().toString();

    if (name.isEmpty() || name == "Click to create new")
        return;

    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText(QString("Removing schedule %1").arg(QString(name).remove("*")));
    msgBox.setInformativeText(QString("Are you sure you want to remove schedule %1? This couldn't be undoed!").arg(QString(name).remove("*")));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if (msgBox.exec() == QMessageBox::Yes) {
        this->removeSchedule(name);

        // Trigger update
        this->triggerScheduleUpdated();
    }
}

void ScheduleDialog::addPresetButton(bool checked)
{
    Q_UNUSED(checked);

    // In no way would a selected schedule doesn't have name
    if (scheduleType == ScheduleType::NoSchedule)
        return;

    QString name = this->presetsListView->currentIndex().data().toString();

    foreach (PresetModel model, this->presetList){
        if (model.getName() == name) {
            this->addPresetToScheduleContent(model);
            this->selectedSchedule.addPresetToList(model);
            this->updateScheduleTimeline();
            this->matchPresetListWithScheduleContent();

            // Trigger update
            this->triggerScheduleUpdated();
            break;
        }
    }
}

void ScheduleDialog::activateScheduleButton(bool checked)
{
    Q_UNUSED(checked);

    this->selectedSchedule.setActivated(true);
    this->setScheduleActivated(true);

    // Trigger update
    this->triggerScheduleUpdated();
}

void ScheduleDialog::deactivateScheduleButton(bool checked)
{
    Q_UNUSED(checked);

    this->selectedSchedule.setActivated(false);
    this->setScheduleActivated(false);

    // Trigger update
    this->triggerScheduleUpdated();
}

void ScheduleDialog::accept()
{
    if (this->updateTriggered) {
        this->storeSelectedSchedule();
        QDialog::accept();
    }
    else {
        QDialog::reject();
    }
}

void ScheduleDialog::reject()
{
    if (this->updateTriggered) {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("Exiting schedule manager");
        msgBox.setInformativeText("All changes within schedule will lost if you proceed, are you sure?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        if (msgBox.exec() == QMessageBox::Yes) {
            QDialog::reject();
        }
        else {
            return;
        }
    }
    else {
        QDialog::reject();
    }
}

void ScheduleDialog::closeEvent(QCloseEvent *event)
{
    if (this->updateTriggered) {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("Exiting schedule manager");
        msgBox.setInformativeText("All changes within schedule will lost if you proceed, are you sure?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        if (msgBox.exec() == QMessageBox::Yes) {
            event->accept();
        }
        else {
            event->ignore();
        }
    }
    else {
        event->accept();
    }
}
