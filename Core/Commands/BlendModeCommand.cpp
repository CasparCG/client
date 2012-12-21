#include "BlendModeCommand.h"

#include "Global.h"

BlendModeCommand::BlendModeCommand(QObject* parent) : QObject(parent),
    channel(Output::DEFAULT_CHANNEL), videolayer(Output::DEFAULT_VIDEOLAYER), delay(Output::DEFAULT_DELAY),
    allowGpi(Output::DEFAULT_ALLOW_GPI), blendMode(Mixer::DEFAULT_BLENDMODE)
{
}

int BlendModeCommand::getDelay() const
{
    return this->delay;
}

int BlendModeCommand::getChannel() const
{
    return this->channel;
}

int BlendModeCommand::getVideolayer() const
{
    return this->videolayer;
}

void BlendModeCommand::setChannel(int channel)
{
    this->channel = channel;
    emit channelChanged(this->channel);
}

void BlendModeCommand::setVideolayer(int videolayer)
{
    this->videolayer = videolayer;
    emit videolayerChanged(this->videolayer);
}

void BlendModeCommand::setDelay(int delay)
{
    this->delay = delay;
    emit delayChanged(this->delay);
}

const QString& BlendModeCommand::getBlendMode() const
{
    return this->blendMode;
}

void BlendModeCommand::setBlendMode(const QString& blendMode)
{
    this->blendMode = blendMode;
    emit blendModeChanged(this->blendMode);
}

bool BlendModeCommand::getAllowGpi() const
{
    return this->allowGpi;
}

void BlendModeCommand::setAllowGpi(bool allowGpi)
{
    this->allowGpi = allowGpi;
    emit allowGpiChanged(this->allowGpi);
}

void BlendModeCommand::readProperties(boost::property_tree::wptree& pt)
{
    setChannel(pt.get<int>(L"channel"));
    setVideolayer(pt.get<int>(L"videolayer"));
    setDelay(pt.get<int>(L"delay"));
    setAllowGpi(pt.get<bool>(L"allowgpi"));
    setBlendMode(QString::fromStdWString(pt.get<std::wstring>(L"blendmode")));
}

void BlendModeCommand::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("channel", QString::number(this->getChannel()));
    writer->writeTextElement("videolayer", QString::number(this->getVideolayer()));
    writer->writeTextElement("delay", QString::number(this->getDelay()));
    writer->writeTextElement("allowgpi", (getAllowGpi() == true) ? "true" : "false");
    writer->writeTextElement("blendmode", this->getBlendMode());
}
