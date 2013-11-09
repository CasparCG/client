#pragma once

#include "../Shared.h"
#include "AbstractRundownWidget.h"
#include "ui_RundownVideoWidget.h"

#include "Global.h"

#include "CasparDevice.h"

#include "GpiDevice.h"

#include "OscSubscription.h"
#include "Animations/ActiveAnimation.h"
#include "Commands/AbstractCommand.h"
#include "Commands/AbstractPlayoutCommand.h"
#include "Commands/VideoCommand.h"
#include "Models/LibraryModel.h"
#include "Models/OscFileModel.h"

#include <QtCore/QString>
#include <QtCore/QTimer>

#include <QtGui/QWidget>

class WIDGETS_EXPORT RundownVideoWidget : public QWidget, Ui::RundownVideoWidget, public AbstractRundownWidget, public AbstractPlayoutCommand
{
    Q_OBJECT

    public:
        explicit RundownVideoWidget(const LibraryModel& model, QWidget* parent = 0, const QString& color = Color::DEFAULT_TRANSPARENT_COLOR,
                                    bool active = false, bool loaded = false, bool paused = false, bool playing = false,
                                    bool inGroup = false, bool compactView = false);

        virtual AbstractRundownWidget* clone();

        virtual bool isGroup() const;
        virtual bool isInGroup() const;

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
        bool loaded;
        bool paused;
        bool playing;
        bool inGroup;
        bool compactView;
        QString color;
        LibraryModel model;
        VideoCommand command;
        ActiveAnimation* animation;
        bool reverseOscTime;
        bool sendAutoPlay;

        OscFileModel* fileModel;
        OscSubscription* timeSubscription;
        OscSubscription* frameSubscription;
        OscSubscription* fpsSubscription;
        OscSubscription* pathSubscription;
        OscSubscription* pausedSubscription;
        OscSubscription* loopSubscription;

        QTimer executeTimer;

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
        Q_SLOT void videolayerChanged(int);
        Q_SLOT void delayChanged(int);
        Q_SLOT void allowGpiChanged(bool);
        Q_SLOT void loopChanged(bool);
        Q_SLOT void gpiConnectionStateChanged(bool, GpiDevice*);
        Q_SLOT void deviceConnectionStateChanged(CasparDevice&);
        Q_SLOT void deviceAdded(CasparDevice&);
        Q_SLOT void timeSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void frameSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void fpsSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void pathSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void pausedSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void loopSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void autoPlayChanged(bool);
};
