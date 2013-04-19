#include "GpiOutputCommand.h"

#include "Global.h"

GpiOutputCommand::GpiOutputCommand(QObject* parent)
    : AbstractCommand(parent),
      gpoPort(Gpi::DEFAULT_GPO_PORT)
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

    if (pt.count(L"gpoport") > 0) setGpoPort(pt.get<float>(L"gpoport"));
}

void GpiOutputCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("gpoport", QString::number(this->getGpoPort()));
}
