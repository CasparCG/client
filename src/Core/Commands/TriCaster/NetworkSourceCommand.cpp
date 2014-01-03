#include "NetworkSourceCommand.h"

#include "Global.h"

NetworkSourceCommand::NetworkSourceCommand(QObject* parent)
    : AbstractCommand(parent),
      source(TriCaster::DEFAULT_NETWORK_SOURCE), target(TriCaster::DEFAULT_NETWORK_TARGET)
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

void NetworkSourceCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setSource(QString::fromStdWString(pt.get(L"source", TriCaster::DEFAULT_NETWORK_SOURCE.toStdWString())));
    setTarget(QString::fromStdWString(pt.get(L"target", TriCaster::DEFAULT_NETWORK_TARGET.toStdWString())));
}

void NetworkSourceCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("source", this->getSource());
    writer->writeTextElement("target", this->getTarget());
}

