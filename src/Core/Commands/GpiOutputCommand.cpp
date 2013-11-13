#include "GpiOutputCommand.h"

#include "Global.h"

GpiOutputCommand::GpiOutputCommand(QObject* parent)
    : AbstractCommand(parent),
      gpoPort(GpiOutput::DEFAULT_PORT)
{
}

int GpiOutputCommand::getGpoPort() const
{
    return this->gpoPort;
}

void GpiOutputCommand::setGpoPort(int gpoPort)
{
    this->gpoPort = gpoPort;
    emit gpoPortChanged(gpoPort);
}

void GpiOutputCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setGpoPort(pt.get(L"gpoport", GpiOutput::DEFAULT_PORT));
}

void GpiOutputCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("gpoport", QString::number(this->getGpoPort()));
}
