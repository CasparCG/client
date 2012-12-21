#pragma once

#include "../Shared.h"
#include "IPlayoutCommand.h"
#include "IRundownWidget.h"
#include "ui_RundownGpiOutputWidget.h"
#include "GpiDevice.h"

#include "Global.h"

#include "Commands/ICommand.h"
#include "Commands/GpiOutputCommand.h"
#include "Models/LibraryModel.h"

#include <QtCore/QString>

#include <QtGui/QWidget>

class WIDGETS_EXPORT RundownGpiOutputWidget : public QWidget, Ui::RundownGpiOutputWidget, public IRundownWidget, public IPlayoutCommand
{
    Q_OBJECT

    public:
        explicit RundownGpiOutputWidget(const LibraryModel& model, QWidget* parent = 0,
                                        const QString& color = Color::DEFAULT_GPI_COLOR, bool active = false,
                                        bool inGroup = false);

        virtual IRundownWidget* clone();

        virtual bool isGroup() const;

        virtual ICommand* getCommand();
        virtual LibraryModel* getLibraryModel();

        virtual void setActive(bool active);
        virtual void setInGroup(bool inGroup);
        virtual void setColor(const QString& color);

        virtual void setExpanded(bool expanded) {}

        virtual bool executeCommand(enum Playout::PlayoutType::Type type);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        bool active;
        bool inGroup;
        QString color;
        LibraryModel model;
        GpiOutputCommand command;

        void checkGpiTriggerable();

        Q_SLOT void executePlay();
        Q_SLOT void delayChanged(int);
        Q_SLOT void gpiOutputPortChanged(int);
        Q_SLOT void gpiDeviceConnected(bool, GpiDevice*);
        Q_SLOT void allowGpiChanged(bool);
};
