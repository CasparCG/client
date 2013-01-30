#include "CropCommand.h"

#include "Global.h"

CropCommand::CropCommand(QObject* parent)
    : AbstractCommand(parent),
      channel(Output::DEFAULT_CHANNEL), videolayer(Output::DEFAULT_VIDEOLAYER), delay(Output::DEFAULT_DELAY),
      allowGpi(Output::DEFAULT_ALLOW_GPI), cropLeft(Mixer::DEFAULT_CROP_LEFT), cropRight(Mixer::DEFAULT_CROP_RIGHT),
      cropTop(Mixer::DEFAULT_CROP_TOP), cropBottom(Mixer::DEFAULT_CROP_BOTTOM), duration(Mixer::DEFAULT_DURATION),
      tween(Mixer::DEFAULT_TWEEN), defer(Mixer::DEFAULT_DEFER)
{
}

int CropCommand::getDelay() const
{
    return this->delay;
}

int CropCommand::getChannel() const
{
    return this->channel;
}

int CropCommand::getVideolayer() const
{
    return this->videolayer;
}

void CropCommand::setChannel(int channel)
{
    this->channel = channel;
    emit channelChanged(this->channel);
}

void CropCommand::setVideolayer(int videolayer)
{
    this->videolayer = videolayer;
    emit videolayerChanged(this->videolayer);
}

void CropCommand::setDelay(int delay)
{
    this->delay = delay;
    emit delayChanged(this->delay);
}

float CropCommand::getCropLeft() const
{
    return this->cropLeft;
}

float CropCommand::getCropRight() const
{
    return this->cropRight;
}

float CropCommand::getCropTop() const
{
    return this->cropTop;
}

float CropCommand::getCropBottom() const
{
    return this->cropBottom;
}

int CropCommand::getDuration() const
{
    return this->duration;
}

const QString& CropCommand::getTween() const
{
    return this->tween;
}

bool CropCommand::getDefer() const
{
    return this->defer;
}

void CropCommand::setCropLeft(float cropLeft)
{
    this->cropLeft = cropLeft;
    emit cropLeftChanged(this->cropLeft);
}

void CropCommand::setCropRight(float cropRight)
{
    this->cropRight = cropRight;
    emit cropRightChanged(this->cropRight);
}

void CropCommand::setCropTop(float cropTop)
{
    this->cropTop = cropTop;
    emit cropTopChanged(this->cropTop);
}

void CropCommand::setCropBottom(float cropBottom)
{
    this->cropBottom = cropBottom;
    emit cropBottomChanged(this->cropBottom);
}

void CropCommand::setDuration(int duration)
{
    this->duration = duration;
    emit durationChanged(this->duration);
}

void CropCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);
}

void CropCommand::setDefer(bool defer)
{
    this->defer = defer;
    emit deferChanged(this->defer);
}

bool CropCommand::getAllowGpi() const
{
    return this->allowGpi;
}

void CropCommand::setAllowGpi(bool allowGpi)
{
    this->allowGpi = allowGpi;
    emit allowGpiChanged(this->allowGpi);
}

void CropCommand::readProperties(boost::property_tree::wptree& pt)
{
    if (pt.count(L"channel") > 0) setChannel(pt.get<int>(L"channel"));
    if (pt.count(L"videolayer") > 0) setVideolayer(pt.get<int>(L"videolayer"));
    if (pt.count(L"delay") > 0) setDelay(pt.get<int>(L"delay"));
    if (pt.count(L"allowgpi") > 0) setAllowGpi(pt.get<bool>(L"allowgpi"));
    if (pt.count(L"cropleft") > 0) setCropLeft(pt.get<float>(L"cropleft"));
    if (pt.count(L"cropright") > 0) setCropRight(pt.get<float>(L"cropright"));
    if (pt.count(L"croptop") > 0) setCropTop(pt.get<float>(L"croptop"));
    if (pt.count(L"cropbottom") > 0) setCropBottom(pt.get<float>(L"cropbottom"));
    if (pt.count(L"duration") > 0) setDuration(pt.get<int>(L"duration"));
    if (pt.count(L"tween") > 0) setTween(QString::fromStdWString(pt.get<std::wstring>(L"tween")));
    if (pt.count(L"defer") > 0) setDefer(pt.get<bool>(L"defer"));
}

void CropCommand::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("channel", QString::number(this->getChannel()));
    writer->writeTextElement("videolayer", QString::number(this->getVideolayer()));
    writer->writeTextElement("delay", QString::number(this->getDelay()));
    writer->writeTextElement("allowgpi", (getAllowGpi() == true) ? "true" : "false");
    writer->writeTextElement("cropleft", QString::number(this->getCropLeft()));
    writer->writeTextElement("cropright", QString::number(this->getCropRight()));
    writer->writeTextElement("croptop", QString::number(this->getCropTop()));
    writer->writeTextElement("cropbottom", QString::number(this->getCropBottom()));
    writer->writeTextElement("duration", QString::number(this->getDuration()));
    writer->writeTextElement("tween", this->getTween());
    writer->writeTextElement("defer", (getDefer() == true) ? "true" : "false");
}
