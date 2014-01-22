#include "AbstractCommand.h"

#include "Global.h"

AbstractCommand::AbstractCommand(QObject* parent)
    : QObject(parent),
      channel(Output::DEFAULT_CHANNEL), videolayer(Output::DEFAULT_VIDEOLAYER), delay(Output::DEFAULT_DELAY),
      allowGpi(Output::DEFAULT_ALLOW_GPI), allowRemoteTriggering(Output::DEFAULT_ALLOW_REMOTE_TRIGGERING), remoteTriggerId(Output::DEFAULT_REMOTE_TRIGGER_ID)
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

void AbstractCommand::readProperties(boost::property_tree::wptree& pt)
{
    if (pt.count(L"channel") > 0) setChannel(pt.get<int>(L"channel", Output::DEFAULT_CHANNEL));
    if (pt.count(L"videolayer") > 0) setVideolayer(pt.get<int>(L"videolayer", Output::DEFAULT_VIDEOLAYER));
    if (pt.count(L"delay") > 0) setDelay(pt.get<int>(L"delay", Output::DEFAULT_DELAY));
    if (pt.count(L"allowgpi") > 0) setAllowGpi(pt.get<bool>(L"allowgpi", Output::DEFAULT_ALLOW_GPI));
    if (pt.count(L"allowremotetriggering") > 0) setAllowRemoteTriggering(pt.get<bool>(L"allowremotetriggering", Output::DEFAULT_ALLOW_REMOTE_TRIGGERING));
    if (pt.count(L"remotetriggerid") > 0) setRemoteTriggerId(QString::fromStdWString(pt.get(L"remotetriggerid", Output::DEFAULT_REMOTE_TRIGGER_ID.toStdWString())));
}

void AbstractCommand::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("channel", QString::number(this->getChannel()));
    writer->writeTextElement("videolayer", QString::number(this->getVideolayer()));
    writer->writeTextElement("delay", QString::number(this->getDelay()));
    writer->writeTextElement("allowgpi", (getAllowGpi() == true) ? "true" : "false");
    writer->writeTextElement("allowremotetriggering", (getAllowRemoteTriggering() == true) ? "true" : "false");
    writer->writeTextElement("remotetriggerid", getRemoteTriggerId());
}
