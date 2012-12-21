#include "GpiOutputCommand.h"

#include "Global.h"

GpiOutputCommand::GpiOutputCommand(QObject* parent)
    : QObject(parent),
      channel(Output::DEFAULT_CHANNEL), videolayer(Output::DEFAULT_VIDEOLAYER), delay(Output::DEFAULT_DELAY),
      allowGpi(Output::DEFAULT_ALLOW_GPI), gpoPort(Gpi::DEFAULT_GPO_PORT)
{
}

int GpiOutputCommand::getDelay() const
{
    return this->delay;
}

int GpiOutputCommand::getChannel() const
{
    return this->channel;
}

int GpiOutputCommand::getVideolayer() const
{
    return this->videolayer;
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

void GpiOutputCommand::setChannel(int channel)
{
    this->channel = channel;
    emit channelChanged(this->channel);
}

void GpiOutputCommand::setVideolayer(int videolayer)
{
    this->videolayer = videolayer;
    emit videolayerChanged(this->videolayer);
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
    setDelay(pt.get<int>(L"delay"));
    setAllowGpi(pt.get<bool>(L"allowgpi"));
    setGpoPort(pt.get<float>(L"gpoport"));
}

void GpiOutputCommand::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("channel", "");
    writer->writeTextElement("videolayer", "");
    writer->writeTextElement("delay", QString::number(this->getDelay()));
    writer->writeTextElement("allowgpi", (getAllowGpi() == true) ? "true" : "false");
    writer->writeTextElement("gpoport", QString::number(this->getGpoPort()));
}
