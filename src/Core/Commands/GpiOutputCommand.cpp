#include "GpiOutputCommand.h"

#include <QtCore/QXmlStreamWriter>

GpiOutputCommand::GpiOutputCommand(QObject* parent)
    : AbstractCommand(parent)
{
}

int GpiOutputCommand::getGpoPort() const
{
    return this->gpoPort;
}

bool GpiOutputCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

void GpiOutputCommand::setGpoPort(int gpoPort)
{
    this->gpoPort = gpoPort;
    emit gpoPortChanged(gpoPort);
}

void GpiOutputCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void GpiOutputCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setGpoPort(pt.get(L"gpoport", GpiOutput::DEFAULT_PORT));
    setTriggerOnNext(pt.get(L"triggeronnext", GpiOutput::DEFAULT_TRIGGER_ON_NEXT));
}

void GpiOutputCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("gpoport", QString::number(this->getGpoPort()));
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
}
