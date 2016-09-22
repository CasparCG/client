#include "AbstractCommand.h"

#include <QtCore/QXmlStreamWriter>

AbstractCommand::AbstractCommand(QObject* parent)
    : QObject(parent)
    , channel(Output::DEFAULT_CHANNEL, this)
    , videolayer(Output::DEFAULT_VIDEOLAYER, this)
    , delay(Output::DEFAULT_DELAY, this)
    , duration(Output::DEFAULT_DURATION, this)
    , allowGpi(Output::DEFAULT_ALLOW_GPI, this)
{
}

AbstractCommand::~AbstractCommand()
{
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
    AbstractProperties::readProperties(pt);

    setAllowRemoteTriggering(pt.get(L"allowremotetriggering", Output::DEFAULT_ALLOW_REMOTE_TRIGGERING));
    setRemoteTriggerId(QString::fromStdWString(pt.get(L"remotetriggerid", Output::DEFAULT_REMOTE_TRIGGER_ID.toStdWString())));
    setStoryId(QString::fromStdWString(pt.get(L"storyid", QString("").toStdWString())));
}

void AbstractCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractProperties::writeProperties(writer);

    writer->writeTextElement("allowremotetriggering", (getAllowRemoteTriggering() == true) ? "true" : "false");
    writer->writeTextElement("remotetriggerid", getRemoteTriggerId());
    writer->writeTextElement("storyid", getStoryId());
}
