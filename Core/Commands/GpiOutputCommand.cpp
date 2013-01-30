#include "GpiOutputCommand.h"

#include "Global.h"

GpiOutputCommand::GpiOutputCommand(QObject* parent)
    : AbstractCommand(parent),
      delay(Output::DEFAULT_DELAY), allowGpi(Output::DEFAULT_ALLOW_GPI), gpoPort(Gpi::DEFAULT_GPO_PORT)
{
}

int GpiOutputCommand::getDelay() const
{
    return this->delay;
}

bool GpiOutputCommand::getAllowGpi() const
{
    return this->allowGpi;
}

void GpiOutputCommand::setAllowGpi(bool allowGpi)
{
    this->allowGpi = allowGpi;
    emit allowGpiChanged(this->allowGpi);
}

void GpiOutputCommand::setDelay(int delay)
{
    this->delay = delay;
    emit delayChanged(this->delay);
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
    if (pt.count(L"delay") > 0) setDelay(pt.get<int>(L"delay"));
    if (pt.count(L"allowgpi") > 0) setAllowGpi(pt.get<bool>(L"allowgpi"));
    if (pt.count(L"gpoport") > 0) setGpoPort(pt.get<float>(L"gpoport"));
}

void GpiOutputCommand::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("delay", QString::number(this->getDelay()));
    writer->writeTextElement("allowgpi", (getAllowGpi() == true) ? "true" : "false");
    writer->writeTextElement("gpoport", QString::number(this->getGpoPort()));
}
