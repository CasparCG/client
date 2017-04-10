#pragma once

#include "../Shared.h"
#include "AbstractRundownWidget.h"
#include "ui_RundownFileRecorderWidget.h"

#include "Global.h"

#include "CasparDevice.h"

#include "GpiDevice.h"

#include "OscSubscription.h"
#include "Animations/ActiveAnimation.h"
#include "Commands/AbstractCommand.h"
#include "Commands/AbstractPlayoutCommand.h"
#include "Commands/FileRecorderCommand.h"
#include "Events/Inspector/ChannelChangedEvent.h"
#include "Events/Inspector/TargetChangedEvent.h"
#include "Events/Inspector/LabelChangedEvent.h"
#include "Events/Inspector/DeviceChangedEvent.h"
#include "Models/LibraryModel.h"
#include "Models/OscFileModel.h"
#include "Utils/ItemScheduler.h"

#include <QtCore/QString>

#include <QtWidgets/QWidget>

class WIDGETS_EXPORT RundownFileRecorderWidget : public QWidget, Ui::RundownFileRecorderWidget, public AbstractRundownWidget, public AbstractPlayoutCommand
{
    Q_OBJECT

    public:
        explicit RundownFileRecorderWidget(const LibraryModel& model, QWidget* parent = 0, const QString& color = Color::DEFAULT_TRANSPARENT_COLOR,
                                           bool active = false, bool inGroup = false, bool compactView = false);

        virtual AbstractRundownWidget* clone();

        virtual bool isGroup() const;
        virtual bool isInGroup() const;
        virtual bool executeCommand(Playout::PlayoutType type);

        virtual AbstractCommand* getCommand();
        virtual LibraryModel* getLibraryModel();

        virtual QString getColor() const;

        virtual void setExpanded(bool /* expanded */) {}
        virtual void setActive(bool active);
        virtual void setInGroup(bool inGroup);
        virtual void setColor(const QString& color);
        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter& writer);
        virtual void setCompactView(bool compactView);
        virtual void clearDelayedCommands();
        virtual void setUsed(bool used);
        virtual void setSelected(bool selected);

    private:
        bool active;
        bool inGroup;
        bool compactView;
        QString color;
        LibraryModel model;
        FileRecorderCommand command;
        ActiveAnimation* animation;
        QString delayType;
        bool markUsedItems;
        bool selected = false;

        OscFileModel fileModel;
        OscSubscription* frameSubscription;
        OscSubscription* fpsSubscription;
        OscSubscription* pathSubscription;

        OscSubscription* stopControlSubscription;
        OscSubscription* playControlSubscription;
        OscSubscription* playNowControlSubscription;
        OscSubscription* clearControlSubscription;
        OscSubscription* clearVideolayerControlSubscription;
        OscSubscription* clearChannelControlSubscription;

        ItemScheduler itemScheduler;

        void updateOscWidget();
        void checkEmptyDevice();
        void checkGpiConnection();
        void checkDeviceConnection();
        void configureOscSubscriptions();

        Q_SLOT void executePlay();
        Q_SLOT void executeStop();
        Q_SLOT void channelChanged(int);
        Q_SLOT void delayChanged(int);
        Q_SLOT void allowGpiChanged(bool);
        Q_SLOT void remoteTriggerIdChanged(const QString&);
        Q_SLOT void gpiConnectionStateChanged(bool, GpiDevice*);
        Q_SLOT void deviceConnectionStateChanged(CasparDevice&);
        Q_SLOT void deviceAdded(CasparDevice&);
        Q_SLOT void frameSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void fpsSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void pathSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void stopControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void playControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void playNowControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void clearControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void clearVideolayerControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void clearChannelControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void labelChanged(const LabelChangedEvent&);
        Q_SLOT void targetChanged(const TargetChangedEvent&);
        Q_SLOT void deviceChanged(const DeviceChangedEvent&);
        Q_SLOT void channelChanged(const ChannelChangedEvent&);
};
