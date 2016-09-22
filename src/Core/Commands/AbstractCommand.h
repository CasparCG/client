#pragma once

#include "../Shared.h"
#include "AbstractProperties.h"
#include "Properties.h"

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

    ChannelProperty channel;
    VideolayerProperty videolayer;
    DelayProperty delay;
    DurationProperty duration;
    AllowGpiProperty allowGpi;
    AllowRemoteTriggeringProperty allowRemoteTriggering;

    virtual QString getRemoteTriggerId() const;
    virtual QString getStoryId() const;

    virtual void setRemoteTriggerId(const QString& remoteTriggerId);
    virtual void setStoryId(const QString& storyId);

    virtual void readProperties(boost::property_tree::wptree& pt);
    virtual void writeProperties(QXmlStreamWriter* writer);

protected:
    explicit AbstractCommand(QObject* parent = 0);

    QString storyId = "";
    QString remoteTriggerId = Output::DEFAULT_REMOTE_TRIGGER_ID;

private:
    Q_SIGNAL void remoteTriggerIdChanged(const QString&);
    Q_SIGNAL void storyIdChanged(const QString&);
};
