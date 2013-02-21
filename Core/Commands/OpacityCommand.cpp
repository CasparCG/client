#include "OpacityCommand.h"

#include "Global.h"

OpacityCommand::OpacityCommand(QObject* parent)
    : AbstractCommand(parent),
      opacity(Mixer::DEFAULT_OPACITY), duration(Mixer::DEFAULT_DURATION),
      tween(Mixer::DEFAULT_TWEEN), defer(Mixer::DEFAULT_DEFER)
{
}

float OpacityCommand::getOpacity() const
{
    return this->opacity;
}

int OpacityCommand::getDuration() const
{
    return this->duration;
}

const QString& OpacityCommand::getTween() const
{
    return this->tween;
}

bool OpacityCommand::getDefer() const
{
    return this->defer;
}

void OpacityCommand::setOpacity(float opacity)
{
    this->opacity = opacity;
    emit opacityChanged(this->opacity);
}

void OpacityCommand::setDuration(int duration)
{
    this->duration = duration;
    emit durationChanged(this->duration);
}

void OpacityCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);
}

void OpacityCommand::setDefer(bool defer)
{
    this->defer = defer;
    emit deferChanged(this->defer);
}

void OpacityCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    if (pt.count(L"opacity") > 0) setOpacity(pt.get<float>(L"opacity"));
    if (pt.count(L"duration") > 0) setDuration(pt.get<int>(L"duration"));
    if (pt.count(L"tween") > 0) setTween(QString::fromStdWString(pt.get<std::wstring>(L"tween")));
    if (pt.count(L"defer") > 0) setDefer(pt.get<bool>(L"defer"));
}

void OpacityCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("opacity", QString::number(this->getOpacity()));
    writer->writeTextElement("duration", QString::number(this->getDuration()));
    writer->writeTextElement("tween", this->getTween());
    writer->writeTextElement("defer", (getDefer() == true) ? "true" : "false");
}
