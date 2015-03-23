#pragma once

#include "../Shared.h"
#include "AbstractProperties.h"

#include "Global.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QObject>

class QXmlStreamWriter;

class CORE_EXPORT AbstractCommand : public QObject, public AbstractProperties
{
    Q_OBJECT

    public:
        virtual ~AbstractCommand();

        virtual int getDelay() const;
        virtual int getDuration() const;
        virtual int getChannel() const;
        virtual int getVideolayer() const;
        virtual bool getAllowGpi() const;
        virtual bool getAllowRemoteTriggering() const;
        virtual QString getRemoteTriggerId() const;
        virtual QString getStoryId() const;

        virtual void setChannel(int channel);
        virtual void setVideolayer(int videolayer);
        virtual void setDelay(int delay);
        virtual void setDuration(int duration);
        virtual void setAllowGpi(bool allowGpi);
        virtual void setAllowRemoteTriggering(bool allowRemoteTriggering);
        virtual void setRemoteTriggerId(const QString& remoteTriggerId);
        virtual void setStoryId(const QString& storyId);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

    protected:
        explicit AbstractCommand(QObject* parent = 0);

        QString storyId = "";
        QString remoteTriggerId = Output::DEFAULT_REMOTE_TRIGGER_ID;

        int channel = Output::DEFAULT_CHANNEL;
        int videolayer = Output::DEFAULT_VIDEOLAYER;
        int delay = Output::DEFAULT_DELAY;
        int duration = Output::DEFAULT_DURATION ;
        bool allowGpi = Output::DEFAULT_ALLOW_GPI;
        bool allowRemoteTriggering = Output::DEFAULT_ALLOW_REMOTE_TRIGGERING;


    private:
        Q_SIGNAL void channelChanged(int);
        Q_SIGNAL void videolayerChanged(int);
        Q_SIGNAL void delayChanged(int);
        Q_SIGNAL void durationChanged(int);
        Q_SIGNAL void allowGpiChanged(bool);
        Q_SIGNAL void allowRemoteTriggeringChanged(bool);
        Q_SIGNAL void remoteTriggerIdChanged(const QString&);
        Q_SIGNAL void storyIdChanged(const QString&);
};
