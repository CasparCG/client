#include "PerspectiveCommand.h"

#include "Global.h"

PerspectiveCommand::PerspectiveCommand(QObject* parent)
    : AbstractCommand(parent),
      upperLeftX(Mixer::DEFAULT_PERSPECTIVE_UPPERLEFT_X), upperLeftY(Mixer::DEFAULT_PERSPECTIVE_UPPERLEFT_Y), upperRightX(Mixer::DEFAULT_PERSPECTIVE_UPPERRIGHT_X),
      upperRightY(Mixer::DEFAULT_PERSPECTIVE_UPPERRIGHT_Y), lowerRightX(Mixer::DEFAULT_PERSPECTIVE_LOWERRIGHT_X), lowerRightY(Mixer::DEFAULT_PERSPECTIVE_LOWERRIGHT_Y),
      lowerLeftX(Mixer::DEFAULT_PERSPECTIVE_LOWERLEFT_X), lowerLeftY(Mixer::DEFAULT_PERSPECTIVE_LOWERLEFT_Y), transtitionDuration(Mixer::DEFAULT_DURATION),
      tween(Mixer::DEFAULT_TWEEN), triggerOnNext(Fill::DEFAULT_TRIGGER_ON_NEXT), defer(Mixer::DEFAULT_DEFER), useMipmap(Mixer::DEFAULT_MIPMAP)
{
}

float PerspectiveCommand::getUpperLeftX() const
{
    return this->upperLeftX;
}

float PerspectiveCommand::getUpperLeftY() const
{
    return this->upperLeftY;
}

float PerspectiveCommand::getUpperRightX() const
{
    return this->upperRightX;
}

float PerspectiveCommand::getUpperRightY() const
{
    return this->upperRightY;
}

float PerspectiveCommand::getLowerRightX() const
{
    return this->lowerRightX;
}

float PerspectiveCommand::getLowerRightY() const
{
    return this->lowerRightY;
}

float PerspectiveCommand::getLowerLeftX() const
{
    return this->lowerLeftX;
}

float PerspectiveCommand::getLowerLeftY() const
{
    return this->lowerLeftY;
}

int PerspectiveCommand::getTransitionDuration() const
{
    return this->transtitionDuration;
}

const QString& PerspectiveCommand::getTween() const
{
    return this->tween;
}

bool PerspectiveCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

bool PerspectiveCommand::getDefer() const
{
    return this->defer;
}

bool PerspectiveCommand::getUseMipmap() const
{
    return this->useMipmap;
}

void PerspectiveCommand::setUpperLeftX(float upperLeftX)
{
    this->upperLeftX = upperLeftX;
    emit upperLeftXChanged(this->upperLeftX);
}

void PerspectiveCommand::setUpperLeftY(float upperLeftY)
{
    this->upperLeftY = upperLeftY;
    emit upperLeftYChanged(this->upperLeftY);
}

void PerspectiveCommand::setUpperRightX(float upperRightX)
{
    this->upperRightX = upperRightX;
    emit upperRightXChanged(this->upperRightX);
}

void PerspectiveCommand::setUpperRightY(float upperRightY)
{
    this->upperRightY = upperRightY;
    emit upperRightYChanged(this->upperRightY);
}

void PerspectiveCommand::setLowerRightX(float lowerRightX)
{
    this->lowerRightX = lowerRightX;
    emit lowerRightXChanged(this->lowerRightX);
}

void PerspectiveCommand::setLowerRightY(float lowerRightY)
{
    this->lowerRightY = lowerRightY;
    emit lowerRightYChanged(this->lowerRightY);
}

void PerspectiveCommand::setLowerLeftX(float lowerLeftX)
{
    this->lowerLeftX = lowerLeftX;
    emit lowerLeftXChanged(this->lowerLeftX);
}

void PerspectiveCommand::setLowerLeftY(float lowerLeftY)
{
    this->lowerLeftY = lowerLeftY;
    emit lowerLeftYChanged(this->lowerLeftY);
}

void PerspectiveCommand::setTransitionDuration(int transtitionDuration)
{
    this->transtitionDuration = transtitionDuration;
    emit transtitionDurationChanged(this->transtitionDuration);
}

void PerspectiveCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);
}

void PerspectiveCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void PerspectiveCommand::setDefer(bool defer)
{
    this->defer = defer;
    emit deferChanged(this->defer);
}

void PerspectiveCommand::setUseMipmap(bool useMipmap)
{
    this->useMipmap = useMipmap;
    emit useMipmapChanged(this->useMipmap);
}

void PerspectiveCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setUpperLeftX(pt.get(L"upperleftx", Mixer::DEFAULT_PERSPECTIVE_UPPERLEFT_X));
    setUpperLeftY(pt.get(L"upperlefty", Mixer::DEFAULT_PERSPECTIVE_UPPERLEFT_Y));
    setUpperRightX(pt.get(L"upperrightx", Mixer::DEFAULT_PERSPECTIVE_UPPERRIGHT_X));
    setUpperRightY(pt.get(L"upperrighty", Mixer::DEFAULT_PERSPECTIVE_UPPERRIGHT_Y));
    setLowerRightX(pt.get(L"lowerrightx", Mixer::DEFAULT_PERSPECTIVE_LOWERRIGHT_X));
    setLowerRightY(pt.get(L"lowerrighty", Mixer::DEFAULT_PERSPECTIVE_LOWERRIGHT_Y));
    setLowerLeftX(pt.get(L"lowerleftx", Mixer::DEFAULT_PERSPECTIVE_LOWERLEFT_X));
    setLowerLeftY(pt.get(L"lowerlefty", Mixer::DEFAULT_PERSPECTIVE_LOWERLEFT_Y));
    setTransitionDuration(pt.get(L"transtitionDuration", Mixer::DEFAULT_DURATION));
    setTween(QString::fromStdWString(pt.get(L"tween", Mixer::DEFAULT_TWEEN.toStdWString())));
    setTriggerOnNext(pt.get(L"triggeronnext", Fill::DEFAULT_TRIGGER_ON_NEXT));
    setDefer(pt.get(L"defer", Mixer::DEFAULT_DEFER));
    setUseMipmap(pt.get(L"usemipmap", Mixer::DEFAULT_MIPMAP));
}

void PerspectiveCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("upperleftx", QString::number(getUpperLeftX()));
    writer->writeTextElement("upperlefty", QString::number(getUpperLeftY()));
    writer->writeTextElement("upperrightx", QString::number(getUpperRightX()));
    writer->writeTextElement("upperrighty", QString::number(getUpperRightY()));
    writer->writeTextElement("lowerrightx", QString::number(getLowerRightX()));
    writer->writeTextElement("lowerrighty", QString::number(getLowerRightY()));
    writer->writeTextElement("lowerleftx", QString::number(getLowerLeftX()));
    writer->writeTextElement("lowerlefty", QString::number(getLowerLeftY()));
    writer->writeTextElement("transtitionDuration", QString::number(getTransitionDuration()));
    writer->writeTextElement("tween", getTween());
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
    writer->writeTextElement("defer", (getDefer() == true) ? "true" : "false");
    writer->writeTextElement("usemipmap", (getUseMipmap() == true) ? "true" : "false");
}
