#include "LevelsCommand.h"

#include "Global.h"

LevelsCommand::LevelsCommand(QObject* parent)
    : AbstractCommand(parent),
      channel(Output::DEFAULT_CHANNEL), videolayer(Output::DEFAULT_VIDEOLAYER), delay(Output::DEFAULT_DELAY),
      allowGpi(Output::DEFAULT_ALLOW_GPI), minIn(Mixer::DEFAULT_LEVELS_MIN_IN), maxIn(Mixer::DEFAULT_LEVELS_MAX_IN),
      minOut(Mixer::DEFAULT_LEVELS_MIN_OUT), maxOut(Mixer::DEFAULT_LEVELS_MAX_OUT), gamma(Mixer::DEFAULT_LEVELS_GAMMA),
      duration(Mixer::DEFAULT_DURATION), tween(Mixer::DEFAULT_TWEEN), defer(Mixer::DEFAULT_DEFER)
{
}

int LevelsCommand::getDelay() const
{
    return this->delay;
}

int LevelsCommand::getChannel() const
{
    return this->channel;
}

int LevelsCommand::getVideolayer() const
{
    return this->videolayer;
}

void LevelsCommand::setChannel(int channel)
{
    this->channel = channel;
    emit channelChanged(this->channel);
}

void LevelsCommand::setVideolayer(int videolayer)
{
    this->videolayer = videolayer;
    emit videolayerChanged(this->videolayer);
}

void LevelsCommand::setDelay(int delay)
{
    this->delay = delay;
    emit delayChanged(this->delay);
}

float LevelsCommand::getMinIn() const
{
    return this->minIn;
}

float LevelsCommand::getMaxIn() const
{
    return this->maxIn;
}

float LevelsCommand::getMinOut() const
{
    return this->minOut;
}

float LevelsCommand::getMaxOut() const
{
    return this->maxOut;
}

float LevelsCommand::getGamma() const
{
    return this->gamma;
}

int LevelsCommand::getDuration() const
{
    return this->duration;
}

const QString& LevelsCommand::getTween() const
{
    return this->tween;
}

bool LevelsCommand::getDefer() const
{
    return this->defer;
}

void LevelsCommand::setMinIn(float minIn)
{
    this->minIn = minIn;
    emit minInChanged(this->minIn);
}

void LevelsCommand::setMaxIn(float maxIn)
{
    this->maxIn = maxIn;
    emit maxInChanged(this->maxIn);
}

void LevelsCommand::setMinOut(float minOut)
{
    this->minOut = minOut;
    emit minOutChanged(this->minOut);
}

void LevelsCommand::setMaxOut(float maxOut)
{
    this->maxOut = maxOut;
    emit maxOutChanged(this->maxOut);
}

void LevelsCommand::setGamma(float gamma)
{
    this->gamma = gamma;
    emit gammaChanged(this->gamma);
}

void LevelsCommand::setDuration(int duration)
{
    this->duration = duration;
    emit durationChanged(this->duration);
}

void LevelsCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);
}

void LevelsCommand::setDefer(bool defer)
{
    this->defer = defer;
    emit deferChanged(this->defer);
}

bool LevelsCommand::getAllowGpi() const
{
    return this->allowGpi;
}

void LevelsCommand::setAllowGpi(bool allowGpi)
{
    this->allowGpi = allowGpi;
    emit allowGpiChanged(this->allowGpi);
}

void LevelsCommand::readProperties(boost::property_tree::wptree& pt)
{
    if (pt.count(L"channel") > 0) setChannel(pt.get<int>(L"channel"));
    if (pt.count(L"videolayer") > 0) setVideolayer(pt.get<int>(L"videolayer"));
    if (pt.count(L"delay") > 0) setDelay(pt.get<int>(L"delay"));
    if (pt.count(L"allowgpi") > 0) setAllowGpi(pt.get<bool>(L"allowgpi"));
    if (pt.count(L"minin") > 0) setMinIn(pt.get<float>(L"minin"));
    if (pt.count(L"maxin") > 0) setMaxIn(pt.get<float>(L"maxin"));
    if (pt.count(L"minout") > 0) setMinOut(pt.get<float>(L"minout"));
    if (pt.count(L"maxout") > 0) setMaxOut(pt.get<float>(L"maxout"));
    if (pt.count(L"gamma") > 0) setGamma(pt.get<float>(L"gamma"));
    if (pt.count(L"duration") > 0) setDuration(pt.get<int>(L"duration"));
    if (pt.count(L"tween") > 0) setTween(QString::fromStdWString(pt.get<std::wstring>(L"tween")));
    if (pt.count(L"defer") > 0) setDefer(pt.get<bool>(L"defer"));
}

void LevelsCommand::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("channel", QString::number(this->getChannel()));
    writer->writeTextElement("videolayer", QString::number(this->getVideolayer()));
    writer->writeTextElement("delay", QString::number(this->getDelay()));
    writer->writeTextElement("allowgpi", (getAllowGpi() == true) ? "true" : "false");
    writer->writeTextElement("minin", QString::number(this->getMinIn()));
    writer->writeTextElement("maxin", QString::number(this->getMaxIn()));
    writer->writeTextElement("minout", QString::number(this->getMinOut()));
    writer->writeTextElement("maxout", QString::number(this->getMaxOut()));
    writer->writeTextElement("gamma", QString::number(this->getGamma()));
    writer->writeTextElement("duration", QString::number(this->getDuration()));
    writer->writeTextElement("tween", this->getTween());
    writer->writeTextElement("defer", (getDefer() == true) ? "true" : "false");
}
