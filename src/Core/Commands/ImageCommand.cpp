#include "ImageCommand.h"

#include "Global.h"

ImageCommand::ImageCommand(QObject* parent)
    : AbstractCommand(parent),
      imageName(Image::DEFAULT_NAME), transition(Mixer::DEFAULT_TRANSITION),
      transitionDuration(Mixer::DEFAULT_DURATION), tween(Mixer::DEFAULT_TWEEN), direction(Mixer::DEFAULT_DIRECTION),
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

int ImageCommand::getTransitionDuration() const
{
    return this->transitionDuration;
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

void ImageCommand::setTransitionDuration(int transitionDuration)
{
    this->transitionDuration = transitionDuration;
    emit transitionDurationChanged(this->transitionDuration);
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

    setTransition(QString::fromStdWString(pt.get(L"transition", Mixer::DEFAULT_TRANSITION.toStdWString())));
    setTransitionDuration(pt.get(L"transitionDuration", Mixer::DEFAULT_DURATION));
    setTween(QString::fromStdWString(pt.get(L"tween", Mixer::DEFAULT_TWEEN.toStdWString())));
    setDirection(QString::fromStdWString(pt.get(L"direction", Mixer::DEFAULT_DIRECTION.toStdWString())));
    setUseAuto(pt.get(L"useauto", Image::DEFAULT_USE_AUTO));
    setTriggerOnNext(pt.get(L"triggeronnext", Image::DEFAULT_TRIGGER_ON_NEXT));
}

void ImageCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("transition", this->getTransition());
    writer->writeTextElement("transitionDuration", QString::number(this->getTransitionDuration()));
    writer->writeTextElement("tween", this->getTween());
    writer->writeTextElement("direction", this->getDirection());
    writer->writeTextElement("useauto", (getUseAuto() == true) ? "true" : "false");
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
}
