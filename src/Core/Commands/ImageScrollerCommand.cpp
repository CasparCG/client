#include "ImageScrollerCommand.h"

#include "Global.h"

ImageScrollerCommand::ImageScrollerCommand(QObject* parent)
    : AbstractCommand(parent),
      imageScrollerName(ImageScroller::DEFAULT_NAME), blur(ImageScroller::DEFAULT_BLUR), speed(ImageScroller::DEFAULT_SPEED),
      premultiply(ImageScroller::DEFAULT_PREMULTIPLY), progressive(ImageScroller::DEFAULT_PROGRESSIVE)
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

const QString& ImageScrollerCommand::getImageScrollerName() const
{
    return this->imageScrollerName;
}

void ImageScrollerCommand::setImageScrollerName(const QString& imageScrollerName)
{
    this->imageScrollerName = imageScrollerName;
    emit imageScrollerNameChanged(this->imageScrollerName);
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

    setBlur(pt.get(L"blur", ImageScroller::DEFAULT_BLUR));
    setSpeed(pt.get(L"speed", ImageScroller::DEFAULT_SPEED));
    setPremultiply(pt.get(L"premultiply", ImageScroller::DEFAULT_PREMULTIPLY));
    setProgressive(pt.get(L"progressive", ImageScroller::DEFAULT_PROGRESSIVE));
}

void ImageScrollerCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("blur", QString::number(this->getBlur()));
    writer->writeTextElement("speed", QString::number(this->getSpeed()));
    writer->writeTextElement("premultiply", (getPremultiply() == true) ? "true" : "false");
    writer->writeTextElement("progressive", (getProgressive() == true) ? "true" : "false");
}
