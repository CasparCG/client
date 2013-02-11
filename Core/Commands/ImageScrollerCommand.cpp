#include "ImageScrollerCommand.h"

#include "Global.h"

ImageScrollerCommand::ImageScrollerCommand(QObject* parent)
    : AbstractCommand(parent),
      mediaName(Media::DEFAULT_MEDIA_NAME), blur(Media::DEFAULT_BLUR), speed(Media::DEFAULT_SPEED),
      premultiply(Media::DEFAULT_PREMULTIPLY), progressive(Media::DEFAULT_PROGRESSIVE)
{
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
    AbstractCommand::readProperties(pt);

    if (pt.count(L"blur") > 0) setBlur(pt.get<int>(L"blur"));
    if (pt.count(L"speed") > 0) setSpeed(pt.get<int>(L"speed"));
    if (pt.count(L"premultiply") > 0) setPremultiply(pt.get<bool>(L"premultiply"));
    if (pt.count(L"progressive") > 0) setProgressive(pt.get<bool>(L"progressive"));
}

void ImageScrollerCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("blur", QString::number(this->getBlur()));
    writer->writeTextElement("speed", QString::number(this->getSpeed()));
    writer->writeTextElement("premultiply", (getPremultiply() == true) ? "true" : "false");
    writer->writeTextElement("progressive", (getProgressive() == true) ? "true" : "false");
}
