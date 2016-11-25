#pragma once

#include "../Shared.h"
#include "AbstractRundownWidget.h"
#include "ui_RundownGroupWidget.h"

#include "Global.h"

#include "GpiDevice.h"

#include "OscSubscription.h"
#include "Animations/ActiveAnimation.h"
#include "Commands/AbstractCommand.h"
#include "Commands/AbstractPlayoutCommand.h"
#include "Commands/GroupCommand.h"
#include "Events/Inspector/LabelChangedEvent.h"
#include "Models/LibraryModel.h"

#include <stdexcept>

#include <QtCore/QString>

#include <QtWidgets/QWidget>

class WIDGETS_EXPORT RundownGroupWidget : public QWidget, Ui::RundownGroupWidget, public AbstractRundownWidget, public AbstractPlayoutCommand
{
    Q_OBJECT

    public:
        explicit RundownGroupWidget(const LibraryModel& model, QWidget* parent = 0, const QString& color = Color::DEFAULT_TRANSPARENT_COLOR,
                                    bool active = false, bool compactView = false);

        virtual AbstractRundownWidget* clone();

        virtual bool isGroup() const;
        virtual bool isInGroup() const;
        virtual bool executeCommand(Playout::PlayoutType type);

        virtual AbstractCommand* getCommand();
        virtual LibraryModel* getLibraryModel();

        virtual QString getColor() const;

        virtual void setExpanded(bool expanded);
        virtual void setActive(bool active);
        virtual void setInGroup(bool /* inGroup */) {}
        virtual void setColor(const QString& color);
        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter& writer);
        virtual void setCompactView(bool compactView);
        virtual void clearDelayedCommands() {}
        virtual void setUsed(bool used);
        virtual void setSelected(bool selected);

    private:
        bool active = false;
        bool compactView = false;
        bool useDropFrameNotation = false;
        QString color; 
        LibraryModel model;
        GroupCommand command;
        ActiveAnimation* animation;
        QString delayType;
        bool markUsedItems;
        bool selected = false;

        OscSubscription* stopControlSubscription;
        OscSubscription* playControlSubscription;
        OscSubscription* loadControlSubscription;
        OscSubscription* pauseControlSubscription;
        OscSubscription* nextControlSubscription;
        OscSubscription* updateControlSubscription;
        OscSubscription* invokeControlSubscription;
        OscSubscription* clearControlSubscription;
        OscSubscription* clearVideolayerControlSubscription;
        OscSubscription* clearChannelControlSubscription;

        void checkGpiConnection();
        void configureOscSubscriptions();
        bool executeOscCommand(Playout::PlayoutType type);

        Q_SLOT void durationChanged(int);
        Q_SLOT void notesChanged(const QString&);
        Q_SLOT void allowGpiChanged(bool);
        Q_SLOT void autoStepChanged(bool);
        Q_SLOT void gpiConnectionStateChanged(bool, GpiDevice*);
        Q_SLOT void autoPlayChanged(bool);
        Q_SLOT void remoteTriggerIdChanged(const QString&);
        Q_SLOT void stopControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void playControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void loadControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void pauseControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void nextControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void updateControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void invokeControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void clearControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void clearVideolayerControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void clearChannelControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void labelChanged(const LabelChangedEvent&);
};
