#pragma once

#include "../Shared.h"
#include "AbstractProperties.h"
#include "Properties.h"

#include <QtCore/QObject>

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
    RemoteTriggerIdProperty remoteTriggerId;
    StoryIdProperty storyId;

protected:
    explicit AbstractCommand(QObject* parent = 0);
};
