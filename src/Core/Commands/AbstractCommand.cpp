#include "AbstractCommand.h"

#include "Global.h"

AbstractCommand::AbstractCommand(QObject* parent)
    : QObject(parent),
      channel(Output::DEFAULT_CHANNEL), videolayer(Output::DEFAULT_VIDEOLAYER), delay(Output::DEFAULT_DELAY), duration(Output::DEFAULT_DURATION),
      allowGpi(Output::DEFAULT_ALLOW_GPI), allowRemoteTriggering(Output::DEFAULT_ALLOW_REMOTE_TRIGGERING), remoteTriggerId(Output::DEFAULT_REMOTE_TRIGGER_ID),
      storyId("")
{
}

AbstractCommand::~AbstractCommand()
{
}

int AbstractCommand::getChannel() const
{
    return this->channel;
}

int AbstractCommand::getVideolayer() const
{
    return this->videolayer;
}

int AbstractCommand::getDelay() const
{
    return this->delay;
}

int AbstractCommand::getDuration() const
{
    return this->duration;
}

bool AbstractCommand::getAllowGpi() const
{
    return this->allowGpi;
}

bool AbstractCommand::getAllowRemoteTriggering() const
{
    return this->allowRemoteTriggering;
}

QString AbstractCommand::getRemoteTriggerId() const
{
    return this->remoteTriggerId;
}

QString AbstractCommand::getStoryId() const
{
    return this->storyId;
}

void AbstractCommand::setChannel(int channel)
{
    this->channel = channel;
    emit channelChanged(this->channel);
}

void AbstractCommand::setVideolayer(int videolayer)
{
    this->videolayer = videolayer;
    emit videolayerChanged(this->videolayer);
}

void AbstractCommand::setDelay(int delay)
{
    this->delay = delay;
    emit delayChanged(this->delay);
}

void AbstractCommand::setDuration(int duration)
{
    this->duration = duration;
    emit durationChanged(this->duration);
}

void AbstractCommand::setAllowGpi(bool allowGpi)
{
    this->allowGpi = allowGpi;
    emit allowGpiChanged(this->allowGpi);
}

void AbstractCommand::setAllowRemoteTriggering(bool allowRemoteTriggering)
{
    this->allowRemoteTriggering = allowRemoteTriggering;
    emit allowRemoteTriggeringChanged(this->allowRemoteTriggering);
}

void AbstractCommand::setRemoteTriggerId(const QString& remoteTriggerId)
{
    this->remoteTriggerId = remoteTriggerId;
    emit remoteTriggerIdChanged(this->remoteTriggerId);
}

void AbstractCommand::setStoryId(const QString& storyId)
{
    this->storyId = storyId;
    emit storyIdChanged(this->storyId);
}

void AbstractCommand::readProperties(boost::property_tree::wptree& pt)
{
    if (pt.count(L"channel") > 0) setChannel(pt.get<int>(L"channel", Output::DEFAULT_CHANNEL));
    if (pt.count(L"videolayer") > 0) setVideolayer(pt.get<int>(L"videolayer", Output::DEFAULT_VIDEOLAYER));
    if (pt.count(L"delay") > 0) setDelay(pt.get<int>(L"delay", Output::DEFAULT_DELAY));
    if (pt.count(L"duration") > 0) setDuration(pt.get<int>(L"duration", Output::DEFAULT_DURATION));
    if (pt.count(L"allowgpi") > 0) setAllowGpi(pt.get<bool>(L"allowgpi", Output::DEFAULT_ALLOW_GPI));
    if (pt.count(L"allowremotetriggering") > 0) setAllowRemoteTriggering(pt.get<bool>(L"allowremotetriggering", Output::DEFAULT_ALLOW_REMOTE_TRIGGERING));
    if (pt.count(L"remotetriggerid") > 0) setRemoteTriggerId(QString::fromStdWString(pt.get(L"remotetriggerid", Output::DEFAULT_REMOTE_TRIGGER_ID.toStdWString())));
    if (pt.count(L"storyid") > 0) setStoryId(QString::fromStdWString(pt.get(L"storyid", QString("").toStdWString())));
}

void AbstractCommand::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("channel", QString::number(getChannel()));
    writer->writeTextElement("videolayer", QString::number(getVideolayer()));
    writer->writeTextElement("delay", QString::number(getDelay()));
    writer->writeTextElement("duration", QString::number(getDuration()));
    writer->writeTextElement("allowgpi", (getAllowGpi() == true) ? "true" : "false");
    writer->writeTextElement("allowremotetriggering", (getAllowRemoteTriggering() == true) ? "true" : "false");
    writer->writeTextElement("remotetriggerid", getRemoteTriggerId());
    writer->writeTextElement("storyid", getStoryId());
}
