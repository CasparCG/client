#include "ContrastCommand.h"

#include <QtCore/QXmlStreamWriter>

ContrastCommand::ContrastCommand(QObject* parent)
    : AbstractCommand(parent)
{
}

float ContrastCommand::getContrast() const
{
    return this->contrast;
}

int ContrastCommand::getTransitionDuration() const
{
    return this->transitionDuration;
}

const QString& ContrastCommand::getTween() const
{
    return this->tween;
}

bool ContrastCommand::getDefer() const
{
    return this->defer;
}

void ContrastCommand::setContrast(float contrast)
{
    this->contrast = contrast;
    emit contrastChanged(this->contrast);
}

void ContrastCommand::setTransitionDuration(int transitionDuration)
{
    this->transitionDuration = transitionDuration;
    emit transitionDurationChanged(this->transitionDuration);
}

void ContrastCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);
}

void ContrastCommand::setDefer(bool defer)
{
    this->defer = defer;
    emit deferChanged(this->defer);
}

void ContrastCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setContrast(pt.get(L"contrast", Mixer::DEFAULT_CONTRAST));
    setTransitionDuration(pt.get(L"transitionDuration", pt.get(L"transtitionDuration", Mixer::DEFAULT_DURATION)));
    setTween(QString::fromStdWString(pt.get(L"tween", Mixer::DEFAULT_TWEEN.toStdWString())));
    setDefer(pt.get(L"defer", Mixer::DEFAULT_DEFER));
}

void ContrastCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("contrast", QString::number(getContrast()));
    writer->writeTextElement("transitionDuration", QString::number(getTransitionDuration()));
    writer->writeTextElement("tween", this->getTween());
    writer->writeTextElement("defer", (getDefer() == true) ? "true" : "false");
}
