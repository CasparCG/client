#include "NetworkSourceCommand.h"

#include "Global.h"

NetworkSourceCommand::NetworkSourceCommand(QObject* parent)
    : AbstractCommand(parent),
      source(TriCaster::DEFAULT_NETWORK_SOURCE), target(TriCaster::DEFAULT_NETWORK_TARGET), triggerOnNext(TriCaster::DEFAULT_TRIGGER_ON_NEXT)
{
}

const QString& NetworkSourceCommand::getSource() const
{
    return this->source;
}

const QString& NetworkSourceCommand::getTarget() const
{
    return this->target;
}

bool NetworkSourceCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

void NetworkSourceCommand::setSource(const QString& source)
{
    this->source = source;
    emit sourceChanged(this->source);
}

void NetworkSourceCommand::setTarget(const QString& target)
{
    this->target = target;
    emit targetChanged(this->target);
}

void NetworkSourceCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void NetworkSourceCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setSource(QString::fromStdWString(pt.get(L"source", TriCaster::DEFAULT_NETWORK_SOURCE.toStdWString())));
    setTarget(QString::fromStdWString(pt.get(L"target", TriCaster::DEFAULT_NETWORK_TARGET.toStdWString())));
    setTriggerOnNext(pt.get(L"triggeronnext", TriCaster::DEFAULT_TRIGGER_ON_NEXT));
}

void NetworkSourceCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("source", this->getSource());
    writer->writeTextElement("target", this->getTarget());
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
}

