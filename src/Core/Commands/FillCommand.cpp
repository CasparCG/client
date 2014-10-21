#include "FillCommand.h"

#include "Global.h"

FillCommand::FillCommand(QObject* parent)
    : AbstractCommand(parent),
      positionX(Mixer::DEFAULT_FILL_XPOS), positionY(Mixer::DEFAULT_FILL_YPOS), scaleX(Mixer::DEFAULT_FILL_XSCALE),
      scaleY(Mixer::DEFAULT_FILL_YSCALE), transtitionDuration(Mixer::DEFAULT_DURATION), tween(Mixer::DEFAULT_TWEEN), triggerOnNext(Fill::DEFAULT_TRIGGER_ON_NEXT),
      defer(Mixer::DEFAULT_DEFER), useMipmap(Mixer::DEFAULT_MIPMAP)
{
}

float FillCommand::getPositionX() const
{
    return this->positionX;
}

float FillCommand::getPositionY() const
{
    return this->positionY;
}

float FillCommand::getScaleX() const
{
    return this->scaleX;
}

float FillCommand::getScaleY() const
{
    return this->scaleY;
}

int FillCommand::getTransitionDuration() const
{
    return this->transtitionDuration;
}

const QString& FillCommand::getTween() const
{
    return this->tween;
}

bool FillCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

bool FillCommand::getDefer() const
{
    return this->defer;
}

bool FillCommand::getUseMipmap() const
{
    return this->useMipmap;
}

void FillCommand::setPositionX(float positionX)
{
    this->positionX = positionX;
    emit positionXChanged(this->positionX);
}

void FillCommand::setPositionY(float positionY)
{
    this->positionY = positionY;
    emit positionYChanged(this->positionY);
}

void FillCommand::setScaleX(float scaleX)
{
    this->scaleX = scaleX;
    emit scaleXChanged(this->scaleX);
}

void FillCommand::setScaleY(float scaleY)
{
    this->scaleY = scaleY;
    emit scaleYChanged(this->scaleY);
}

void FillCommand::setTransitionDuration(int transtitionDuration)
{
    this->transtitionDuration = transtitionDuration;
    emit transtitionDurationChanged(this->transtitionDuration);
}

void FillCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);
}

void FillCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void FillCommand::setDefer(bool defer)
{
    this->defer = defer;
    emit deferChanged(this->defer);
}

void FillCommand::setUseMipmap(bool useMipmap)
{
    this->useMipmap = useMipmap;
    emit useMipmapChanged(this->useMipmap);
}

void FillCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setPositionX(pt.get(L"positionx", Mixer::DEFAULT_FILL_XPOS));
    setPositionY(pt.get(L"positiony", Mixer::DEFAULT_FILL_YPOS));
    setScaleX(pt.get(L"scalex", Mixer::DEFAULT_FILL_XSCALE));
    setScaleY(pt.get(L"scaley", Mixer::DEFAULT_FILL_YSCALE));
    setTransitionDuration(pt.get(L"transtitionDuration", Mixer::DEFAULT_DURATION));
    setTween(QString::fromStdWString(pt.get(L"tween", Mixer::DEFAULT_TWEEN.toStdWString())));
    setTriggerOnNext(pt.get(L"triggeronnext", Fill::DEFAULT_TRIGGER_ON_NEXT));
    setDefer(pt.get(L"defer", Mixer::DEFAULT_DEFER));
    setUseMipmap(pt.get(L"usemipmap", Mixer::DEFAULT_MIPMAP));
}

void FillCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("positionx", QString::number(getPositionX()));
    writer->writeTextElement("positiony", QString::number(getPositionY()));
    writer->writeTextElement("scalex", QString::number(getScaleX()));
    writer->writeTextElement("scaley", QString::number(getScaleY()));
    writer->writeTextElement("transtitionDuration", QString::number(getTransitionDuration()));
    writer->writeTextElement("tween", getTween());
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
    writer->writeTextElement("defer", (getDefer() == true) ? "true" : "false");
    writer->writeTextElement("usemipmap", (getUseMipmap() == true) ? "true" : "false");
}
