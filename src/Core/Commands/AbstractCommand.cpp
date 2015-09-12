#include "AbstractCommand.h"

#include <QtCore/QXmlStreamWriter>

AbstractCommand::AbstractCommand(QObject* parent)
    : QObject(parent)
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

bool AbstractCommand::getAutoStep() const
{
    return this->autoStep;
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

void AbstractCommand::setAutoStep(bool autoStep)
{
    this->autoStep = autoStep;
    emit autoStepChanged(this->autoStep);
}

void AbstractCommand::readProperties(boost::property_tree::wptree& pt)
{
    setChannel(pt.get(L"channel", Output::DEFAULT_CHANNEL));
    setVideolayer(pt.get(L"videolayer", Output::DEFAULT_VIDEOLAYER));
    setDelay(pt.get(L"delay", Output::DEFAULT_DELAY));
    setDuration(pt.get(L"duration", Output::DEFAULT_DURATION));
    setAllowGpi(pt.get(L"allowgpi", Output::DEFAULT_ALLOW_GPI));
    setAllowRemoteTriggering(pt.get(L"allowremotetriggering", Output::DEFAULT_ALLOW_REMOTE_TRIGGERING));
    setRemoteTriggerId(QString::fromStdWString(pt.get(L"remotetriggerid", Output::DEFAULT_REMOTE_TRIGGER_ID.toStdWString())));
    setStoryId(QString::fromStdWString(pt.get(L"storyid", QString("").toStdWString())));
    setAutoStep(pt.get(L"autostep", Output::DEFAULT_AUTO_STEP));
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
    writer->writeTextElement("autostep", (getAutoStep() == true) ? "true" : "false");
}
