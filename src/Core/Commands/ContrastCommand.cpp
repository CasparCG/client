#include "ContrastCommand.h"

#include "Global.h"

ContrastCommand::ContrastCommand(QObject* parent)
    : AbstractCommand(parent),
      contrast(Mixer::DEFAULT_CONTRAST), duration(Mixer::DEFAULT_DURATION), tween(Mixer::DEFAULT_TWEEN), defer(Mixer::DEFAULT_DEFER)
{
}

float ContrastCommand::getContrast() const
{
    return this->contrast;
}

int ContrastCommand::getDuration() const
{
    return this->duration;
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

void ContrastCommand::setDuration(int duration)
{
    this->duration = duration;
    emit durationChanged(this->duration);
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

    if (pt.count(L"contrast") > 0) setContrast(pt.get<float>(L"contrast"));
    if (pt.count(L"duration") > 0) setDuration(pt.get<int>(L"duration"));
    if (pt.count(L"tween") > 0) setTween(QString::fromStdWString(pt.get<std::wstring>(L"tween")));
    if (pt.count(L"defer") > 0) setDefer(pt.get<bool>(L"defer"));
}

void ContrastCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("contrast", QString::number(this->getContrast()));
    writer->writeTextElement("duration", QString::number(this->getDuration()));
    writer->writeTextElement("tween", this->getTween());
    writer->writeTextElement("defer", (getDefer() == true) ? "true" : "false");
}
