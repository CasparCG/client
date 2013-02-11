#pragma once

#include "../Shared.h"
#include "AbstractRundownWidget.h"
#include "ui_RundownGpiOutputWidget.h"
#include "GpiDevice.h"

#include "Global.h"

#include "Animations/ActiveAnimation.h"
#include "Commands/AbstractCommand.h"
#include "Commands/AbstractPlayoutCommand.h"
#include "Commands/GpiOutputCommand.h"
#include "Models/LibraryModel.h"

#include <QtCore/QString>
#include <QtCore/QTimer>

#include <QtGui/QWidget>

class WIDGETS_EXPORT RundownGpiOutputWidget : public QWidget, Ui::RundownGpiOutputWidget, public AbstractRundownWidget, public AbstractPlayoutCommand
{
    Q_OBJECT

    public:
        explicit RundownGpiOutputWidget(const LibraryModel& model, QWidget* parent = 0,
                                        const QString& color = Color::DEFAULT_TRANSPARENT_COLOR, bool active = false,
                                        bool inGroup = false, bool compactView = false);

        virtual AbstractRundownWidget* clone();

        virtual bool isGroup() const;

        virtual AbstractCommand* getCommand();
        virtual LibraryModel* getLibraryModel();

        virtual void setActive(bool active);
        virtual void setInGroup(bool inGroup);
        virtual void setColor(const QString& color);

        virtual void setExpanded(bool expanded) {}

        virtual bool executeCommand(enum Playout::PlayoutType::Type type);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        virtual void setCompactView(bool compactView);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        bool active;
        bool inGroup;
        bool compactView;
        QString color;
        LibraryModel model;
        GpiOutputCommand command;
        ActiveAnimation* animation;

        QTimer executeTimer;

        void checkGpiTriggerable();

        Q_SLOT void executePlay();
        Q_SLOT void executeStop();
        Q_SLOT void delayChanged(int);
        Q_SLOT void gpiOutputPortChanged(int);
        Q_SLOT void gpiDeviceConnected(bool, GpiDevice*);
        Q_SLOT void allowGpiChanged(bool);
};
