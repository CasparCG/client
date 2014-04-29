#include "LevelsCommand.h"

#include "Global.h"

LevelsCommand::LevelsCommand(QObject* parent)
    : AbstractCommand(parent),
      minIn(Mixer::DEFAULT_LEVELS_MIN_IN), maxIn(Mixer::DEFAULT_LEVELS_MAX_IN), minOut(Mixer::DEFAULT_LEVELS_MIN_OUT),
      maxOut(Mixer::DEFAULT_LEVELS_MAX_OUT), gamma(Mixer::DEFAULT_LEVELS_GAMMA), transtitionDuration(Mixer::DEFAULT_DURATION),
      tween(Mixer::DEFAULT_TWEEN), defer(Mixer::DEFAULT_DEFER)
{
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

int LevelsCommand::getTransitionDuration() const
{
    return this->transtitionDuration;
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

void LevelsCommand::setTransitionDuration(int transtitionDuration)
{
    this->transtitionDuration = transtitionDuration;
    emit transtitionDurationChanged(this->transtitionDuration);
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

void LevelsCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setMinIn(pt.get(L"minin", Mixer::DEFAULT_LEVELS_MIN_IN));
    setMaxIn(pt.get(L"maxin", Mixer::DEFAULT_LEVELS_MAX_IN));
    setMinOut(pt.get(L"minout", Mixer::DEFAULT_LEVELS_MIN_OUT));
    setMaxOut(pt.get(L"maxout", Mixer::DEFAULT_LEVELS_MAX_OUT));
    setGamma(pt.get(L"gamma", Mixer::DEFAULT_LEVELS_GAMMA));
    setTransitionDuration(pt.get(L"transtitionDuration", Mixer::DEFAULT_DURATION));
    setTween(QString::fromStdWString(pt.get(L"tween", Mixer::DEFAULT_TWEEN.toStdWString())));
    setDefer(pt.get(L"defer", Mixer::DEFAULT_DEFER));
}

void LevelsCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("minin", QString::number(getMinIn()));
    writer->writeTextElement("maxin", QString::number(getMaxIn()));
    writer->writeTextElement("minout", QString::number(getMinOut()));
    writer->writeTextElement("maxout", QString::number(getMaxOut()));
    writer->writeTextElement("gamma", QString::number(getGamma()));
    writer->writeTextElement("transtitionDuration", QString::number(getTransitionDuration()));
    writer->writeTextElement("tween", getTween());
    writer->writeTextElement("defer", (getDefer() == true) ? "true" : "false");
}
