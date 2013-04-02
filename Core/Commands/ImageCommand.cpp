#include "ImageCommand.h"

#include "Global.h"

ImageCommand::ImageCommand(QObject* parent)
    : AbstractCommand(parent),
      imageName(Image::DEFAULT_NAME), transition(Mixer::DEFAULT_TRANSITION),
      duration(Mixer::DEFAULT_DURATION), tween(Mixer::DEFAULT_TWEEN), direction(Mixer::DEFAULT_DIRECTION),
      triggerOnNext(Image::DEFAULT_TRIGGER_ON_NEXT), useAuto(Image::DEFAULT_USE_AUTO)
{
}

const QString& ImageCommand::getImageName() const
{
    return this->imageName;
}

const QString& ImageCommand::getTransition() const
{
    return this->transition;
}

int ImageCommand::getDuration() const
{
    return this->duration;
}

const QString& ImageCommand::getDirection() const
{
    return this->direction;
}

const QString& ImageCommand::getTween() const
{
    return this->tween;
}

bool ImageCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

bool ImageCommand::getUseAuto() const
{
    return this->useAuto;
}

void ImageCommand::setImageName(const QString& imageName)
{
    this->imageName = imageName;
    emit imageNameChanged(this->imageName);
}

void ImageCommand::setTransition(const QString& transition)
{
    this->transition = transition;
    emit transitionChanged(this->transition);
}

void ImageCommand::setDuration(int duration)
{
    this->duration = duration;
    emit durationChanged(this->duration);
}

void ImageCommand::setDirection(const QString& direction)
{
    this->direction = direction;
    emit directionChanged(this->direction);
}

void ImageCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);
}

void ImageCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void ImageCommand::setUseAuto(bool useAuto)
{
    this->useAuto = useAuto;
    emit useAutoChanged(this->useAuto);
}

void ImageCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    if (pt.count(L"transition") > 0) setTransition(QString::fromStdWString(pt.get<std::wstring>(L"transition")));
    if (pt.count(L"duration") > 0) setDuration(pt.get<int>(L"duration"));
    if (pt.count(L"tween") > 0) setTween(QString::fromStdWString(pt.get<std::wstring>(L"tween")));
    if (pt.count(L"direction") > 0) setDirection(QString::fromStdWString(pt.get<std::wstring>(L"direction")));
    if (pt.count(L"useauto") > 0) setUseAuto(pt.get<bool>(L"useauto"));
    if (pt.count(L"triggeronnext") > 0) setTriggerOnNext(pt.get<bool>(L"triggeronnext"));
}

void ImageCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("transition", this->getTransition());
    writer->writeTextElement("duration", QString::number(this->getDuration()));
    writer->writeTextElement("tween", this->getTween());
    writer->writeTextElement("direction", this->getDirection());
    writer->writeTextElement("useauto", (getUseAuto() == true) ? "true" : "false");
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
}
