#pragma once

#include "../Shared.h"
#include "AbstractRundownWidget.h"
#include "ui_RundownMovieWidget.h"

#include "Global.h"

#include "CasparDevice.h"

#include "GpiDevice.h"

#include "OscSubscription.h"
#include "Animations/ActiveAnimation.h"
#include "Commands/AbstractCommand.h"
#include "Commands/AbstractPlayoutCommand.h"
#include "Commands/MovieCommand.h"
#include "Events/Inspector/ChannelChangedEvent.h"
#include "Events/Inspector/DeviceChangedEvent.h"
#include "Events/Inspector/LabelChangedEvent.h"
#include "Events/Inspector/TargetChangedEvent.h"
#include "Events/Inspector/VideolayerChangedEvent.h"
#include "Models/LibraryModel.h"
#include "Models/OscFileModel.h"
#include "Utils/ItemScheduler.h"

#include <QtCore/QString>

#include <QtWidgets/QWidget>

class WIDGETS_EXPORT RundownMovieWidget : public QWidget, Ui::RundownMovieWidget, public AbstractRundownWidget, public AbstractPlayoutCommand
{
    Q_OBJECT

    public:
        explicit RundownMovieWidget(const LibraryModel& model, QWidget* parent = 0, const QString& color = Color::DEFAULT_TRANSPARENT_COLOR,
                                    bool active = false, bool loaded = false, bool paused = false, bool playing = false,
                                    bool inGroup = false, bool compactView = false);

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
        bool loaded;
        bool paused;
        bool playing;
        bool inGroup;
        bool compactView;
        QString color;
        LibraryModel model;
        MovieCommand command;
        ActiveAnimation* animation;
        bool reverseOscTime;
        bool sendAutoPlay;
        bool hasSentAutoPlay;
        QString delayType;
        bool markUsedItems;
        bool useFreezeOnLoad;
        bool selected = false;

        OscFileModel fileModel;
        OscSubscription* timeSubscription;
        OscSubscription* clipSubscription;
        OscSubscription* fpsSubscription;
        OscSubscription* nameSubscription;
        OscSubscription* pausedSubscription;
        OscSubscription* loopSubscription;

        OscSubscription* stopControlSubscription;
        OscSubscription* playControlSubscription;
        OscSubscription* playNowControlSubscription;
        OscSubscription* loadControlSubscription;
        OscSubscription* pauseControlSubscription;
        OscSubscription* nextControlSubscription;
        OscSubscription* updateControlSubscription;
        OscSubscription* previewControlSubscription;
        OscSubscription* clearControlSubscription;
        OscSubscription* clearVideolayerControlSubscription;
        OscSubscription* clearChannelControlSubscription;

        ItemScheduler itemScheduler;

        void updateOscWidget();
        void setThumbnail();
        void checkEmptyDevice();
        void checkGpiConnection();
        void checkDeviceConnection();
        void configureOscSubscriptions();
        void setTimecode(const QString& timecode);

        Q_SLOT void channelChanged(int);
        Q_SLOT void executeClearVideolayer();
        Q_SLOT void executeClearChannel();
        Q_SLOT void executeLoad();
        Q_SLOT void executePlay();
        Q_SLOT void executePause();
        Q_SLOT void executeStop();
        Q_SLOT void executeNext();
        Q_SLOT void executeLoadPreview();
        Q_SLOT void videolayerChanged(int);
        Q_SLOT void delayChanged(int);
        Q_SLOT void allowGpiChanged(bool);
        Q_SLOT void loopChanged(bool);
        Q_SLOT void gpiConnectionStateChanged(bool, GpiDevice*);
        Q_SLOT void remoteTriggerIdChanged(const QString&);
        Q_SLOT void deviceConnectionStateChanged(CasparDevice&);
        Q_SLOT void deviceAdded(CasparDevice&);
        Q_SLOT void timeSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void clipSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void fpsSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void nameSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void pausedSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void loopSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void autoPlayChanged(bool);
        Q_SLOT void stopControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void playControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void playNowControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void loadControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void pauseControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void nextControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void updateControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void previewControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void clearControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void clearVideolayerControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void clearChannelControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void videolayerChanged(const VideolayerChangedEvent&);
        Q_SLOT void channelChanged(const ChannelChangedEvent&);
        Q_SLOT void labelChanged(const LabelChangedEvent&);
        Q_SLOT void targetChanged(const TargetChangedEvent&);
        Q_SLOT void deviceChanged(const DeviceChangedEvent&);
};
