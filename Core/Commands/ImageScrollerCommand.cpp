#include "ImageScrollerCommand.h"

#include "Global.h"

ImageScrollerCommand::ImageScrollerCommand(QObject* parent)
    : QObject(parent),
      channel(Output::DEFAULT_CHANNEL), videolayer(Output::DEFAULT_VIDEOLAYER), delay(Output::DEFAULT_DELAY),
      allowGpi(Output::DEFAULT_ALLOW_GPI), mediaName(Media::DEFAULT_MEDIA_NAME), blur(Media::DEFAULT_BLUR), speed(Media::DEFAULT_SPEED),
      premultiply(Media::DEFAULT_PREMULTIPLY), progressive(Media::DEFAULT_PROGRESSIVE)
{
}

int ImageScrollerCommand::getDelay() const
{
    return this->delay;
}

int ImageScrollerCommand::getChannel() const
{
    return this->channel;
}

int ImageScrollerCommand::getVideolayer() const
{
    return this->videolayer;
}

bool ImageScrollerCommand::getAllowGpi() const
{
    return this->allowGpi;
}

int ImageScrollerCommand::getBlur() const
{
    return this->blur;
}

int ImageScrollerCommand::getSpeed() const
{
    return this->speed;
}

bool ImageScrollerCommand::getPremultiply() const
{
    return this->premultiply;
}

bool ImageScrollerCommand::getProgressive() const
{
    return this->progressive;
}

const QString& ImageScrollerCommand::getMediaName() const
{
    return this->mediaName;
}

void ImageScrollerCommand::setChannel(int channel)
{
    this->channel = channel;
    emit channelChanged(this->channel);
}

void ImageScrollerCommand::setVideolayer(int videolayer)
{
    this->videolayer = videolayer;
    emit videolayerChanged(this->videolayer);
}

void ImageScrollerCommand::setDelay(int delay)
{
    this->delay = delay;
    emit delayChanged(this->delay);
}

void ImageScrollerCommand::setAllowGpi(bool allowGpi)
{
    this->allowGpi = allowGpi;
    emit allowGpiChanged(this->allowGpi);
}

void ImageScrollerCommand::setMediaName(const QString& mediaName)
{
    this->mediaName = mediaName;
    emit mediaNameChanged(this->mediaName);
}

void ImageScrollerCommand::setBlur(int blur)
{
    this->blur = blur;
    emit blurChanged(this->blur);
}

void ImageScrollerCommand::setSpeed(int speed)
{
    this->speed = speed;
    emit speedChanged(this->speed);
}

void ImageScrollerCommand::setPremultiply(bool premultiply)
{
    this->premultiply = premultiply;
    emit premultiplyChanged(this->premultiply);
}

void ImageScrollerCommand::setProgressive(bool progressive)
{
    this->progressive = progressive;
    emit progressiveChanged(this->progressive);
}

void ImageScrollerCommand::readProperties(boost::property_tree::wptree& pt)
{
    setChannel(pt.get<int>(L"channel"));
    setVideolayer(pt.get<int>(L"videolayer"));
    setDelay(pt.get<int>(L"delay"));
    setAllowGpi(pt.get<bool>(L"allowgpi"));
    setBlur(pt.get<int>(L"blur"));
    setSpeed(pt.get<int>(L"speed"));
    setPremultiply(pt.get<bool>(L"premultiply"));
    setProgressive(pt.get<bool>(L"progressive"));
}

void ImageScrollerCommand::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("channel", QString::number(this->getChannel()));
    writer->writeTextElement("videolayer", QString::number(this->getVideolayer()));
    writer->writeTextElement("delay", QString::number(this->getDelay()));
    writer->writeTextElement("allowgpi", (getAllowGpi() == true) ? "true" : "false");
    writer->writeTextElement("blur", QString::number(this->getBlur()));
    writer->writeTextElement("speed", QString::number(this->getSpeed()));
    writer->writeTextElement("premultiply", (getPremultiply() == true) ? "true" : "false");
    writer->writeTextElement("progressive", (getProgressive() == true) ? "true" : "false");
}
