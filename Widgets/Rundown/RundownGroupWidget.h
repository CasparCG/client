#pragma once

#include "../Shared.h"
#include "IPlayoutCommand.h"
#include "IRundownWidget.h"
#include "ui_RundownGroupWidget.h"
#include "GpiDevice.h"

#include "Global.h"

#include "Commands/ICommand.h"
#include "Commands/GroupCommand.h"
#include "Models/LibraryModel.h"

#include <stdexcept>

#include <QtCore/QString>

#include <QtGui/QWidget>

class WIDGETS_EXPORT RundownGroupWidget : public QWidget, Ui::RundownGroupWidget, public IRundownWidget, public IPlayoutCommand
{
    Q_OBJECT

    public:
        explicit RundownGroupWidget(const LibraryModel& model, QWidget* parent = 0, bool active = false);

        virtual IRundownWidget* clone();

        virtual bool isGroup() const;

        virtual ICommand* getCommand();
        virtual LibraryModel* getLibraryModel();

        virtual void setActive(bool active);
        virtual void setInGroup(bool inGroup) {}
        virtual void setColor(const QString& color) {}

        virtual void setExpanded(bool expanded);

        virtual bool executeCommand(enum Playout::PlayoutType::Type type) { return false; }

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        bool active;
        LibraryModel model;
        GroupCommand command;

        void checkGpiTriggerable();

        Q_SLOT void channelChanged(int);
        Q_SLOT void videolayerChanged(int);
        Q_SLOT void delayChanged(int);
        Q_SLOT void allowGpiChanged(bool);
        Q_SLOT void gpiDeviceConnected(bool, GpiDevice*);
};
