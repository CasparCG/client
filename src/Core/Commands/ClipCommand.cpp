#include "ClipCommand.h"

#include "Global.h"

ClipCommand::ClipCommand(QObject* parent)
    : AbstractCommand(parent),
      left(Mixer::DEFAULT_CLIP_LEFT), width(Mixer::DEFAULT_CLIP_WIDTH), top(Mixer::DEFAULT_CLIP_TOP),
      height(Mixer::DEFAULT_CLIP_HEIGHT), transtitionDuration(Mixer::DEFAULT_DURATION), tween(Mixer::DEFAULT_TWEEN), defer(Mixer::DEFAULT_DEFER)
{
}

float ClipCommand::getLeft() const
{
    return this->left;
}

float ClipCommand::getWidth() const
{
    return this->width;
}

float ClipCommand::getTop() const
{
    return this->top;
}

float ClipCommand::getHeight() const
{
    return this->height;
}

int ClipCommand::getTransitionDuration() const
{
    return this->transtitionDuration;
}

const QString& ClipCommand::getTween() const
{
    return this->tween;
}

bool ClipCommand::getDefer() const
{
    return this->defer;
}

void ClipCommand::setLeft(float left)
{
    this->left = left;
    emit leftChanged(this->left);
}

void ClipCommand::setWidth(float width)
{
    this->width = width;
    emit widthChanged(this->width);
}

void ClipCommand::setTop(float top)
{
    this->top = top;
    emit topChanged(this->top);
}

void ClipCommand::setHeight(float height)
{
    this->height = height;
    emit heightChanged(this->height);
}

void ClipCommand::setTransitionDuration(int transtitionDuration)
{
    this->transtitionDuration = transtitionDuration;
    emit transtitionDurationChanged(this->transtitionDuration);
}

void ClipCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);
}

void ClipCommand::setDefer(bool defer)
{
    this->defer = defer;
    emit deferChanged(this->defer);
}

void ClipCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setLeft(pt.get(L"left", Mixer::DEFAULT_CLIP_LEFT));
    setWidth(pt.get(L"width", Mixer::DEFAULT_CLIP_WIDTH));
    setTop(pt.get(L"top", Mixer::DEFAULT_CLIP_TOP));
    setHeight(pt.get(L"height", Mixer::DEFAULT_CLIP_HEIGHT));
    setTransitionDuration(pt.get(L"transtitionDuration", Mixer::DEFAULT_DURATION));
    setTween(QString::fromStdWString(pt.get(L"tween", Mixer::DEFAULT_TWEEN.toStdWString())));
    setDefer(pt.get(L"defer", Mixer::DEFAULT_DEFER));
}

void ClipCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("left", QString::number(getLeft()));
    writer->writeTextElement("width", QString::number(getWidth()));
    writer->writeTextElement("top", QString::number(getTop()));
    writer->writeTextElement("height", QString::number(getHeight()));
    writer->writeTextElement("transtitionDuration", QString::number(getTransitionDuration()));
    writer->writeTextElement("tween", this->getTween());
    writer->writeTextElement("defer", (getDefer() == true) ? "true" : "false");
}
