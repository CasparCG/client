#include "GridCommand.h"

#include "Global.h"

GridCommand::GridCommand(QObject* parent)
    : QObject(parent),
      channel(Output::DEFAULT_CHANNEL), videolayer(Output::DEFAULT_VIDEOLAYER), delay(Output::DEFAULT_DELAY),
      allowGpi(Output::DEFAULT_ALLOW_GPI), grid(Mixer::DEFAULT_GRID), duration(Mixer::DEFAULT_DURATION),
      tween(Mixer::DEFAULT_TWEEN), defer(Mixer::DEFAULT_DEFER)
{
}

int GridCommand::getDelay() const
{
    return this->delay;
}

int GridCommand::getChannel() const
{
    return this->channel;
}

int GridCommand::getVideolayer() const
{
    return this->videolayer;
}

void GridCommand::setChannel(int channel)
{
    this->channel = channel;
    emit channelChanged(this->channel);
}

void GridCommand::setVideolayer(int videolayer)
{
    this->videolayer = videolayer;
    emit videolayerChanged(this->videolayer);
}

void GridCommand::setDelay(int delay)
{
    this->delay = delay;
    emit delayChanged(this->delay);
}

int GridCommand::getGrid() const
{
    return this->grid;
}

int GridCommand::getDuration() const
{
    return this->duration;
}

const QString& GridCommand::getTween() const
{
    return this->tween;
}

bool GridCommand::getDefer() const
{
    return this->defer;
}

void GridCommand::setGrid(int grid)
{
    this->grid = grid;
    emit gridChanged(this->grid);
}

void GridCommand::setDuration(int duration)
{
    this->duration = duration;
    emit durationChanged(this->duration);
}

void GridCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);
}

void GridCommand::setDefer(bool defer)
{
    this->defer = defer;
    emit deferChanged(this->defer);
}

bool GridCommand::getAllowGpi() const
{
    return this->allowGpi;
}

void GridCommand::setAllowGpi(bool allowGpi)
{
    this->allowGpi = allowGpi;
    emit allowGpiChanged(this->allowGpi);
}

void GridCommand::readProperties(boost::property_tree::wptree& pt)
{
    setChannel(pt.get<int>(L"channel"));
    setVideolayer(pt.get<int>(L"videolayer"));
    setDelay(pt.get<int>(L"delay"));
    setAllowGpi(pt.get<bool>(L"allowgpi"));
    setGrid(pt.get<float>(L"grid"));
    setDuration(pt.get<int>(L"duration"));
    setTween(QString::fromStdWString(pt.get<std::wstring>(L"tween")));
    setDefer(pt.get<bool>(L"defer"));
}

void GridCommand::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("channel", QString::number(this->getChannel()));
    writer->writeTextElement("videolayer", QString::number(this->getVideolayer()));
    writer->writeTextElement("delay", QString::number(this->getDelay()));
    writer->writeTextElement("allowgpi", (getAllowGpi() == true) ? "true" : "false");
    writer->writeTextElement("grid", QString::number(this->getGrid()));
    writer->writeTextElement("duration", QString::number(this->getDuration()));
    writer->writeTextElement("tween", this->getTween());
    writer->writeTextElement("defer", (getDefer() == true) ? "true" : "false");
}
