#include "CropCommand.h"

#include "Global.h"

CropCommand::CropCommand(QObject* parent)
    : AbstractCommand(parent),
      cropLeft(Mixer::DEFAULT_CROP_LEFT), cropRight(Mixer::DEFAULT_CROP_RIGHT), cropTop(Mixer::DEFAULT_CROP_TOP),
      cropBottom(Mixer::DEFAULT_CROP_BOTTOM), transtitionDuration(Mixer::DEFAULT_DURATION), tween(Mixer::DEFAULT_TWEEN), defer(Mixer::DEFAULT_DEFER)
{
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

int CropCommand::getTransitionDuration() const
{
    return this->transtitionDuration;
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

void CropCommand::setTransitionDuration(int transtitionDuration)
{
    this->transtitionDuration = transtitionDuration;
    emit transtitionDurationChanged(this->transtitionDuration);
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

void CropCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setCropLeft(pt.get(L"cropleft", Mixer::DEFAULT_CROP_LEFT));
    setCropRight(pt.get(L"cropright", Mixer::DEFAULT_CROP_RIGHT));
    setCropTop(pt.get(L"croptop", Mixer::DEFAULT_CROP_TOP));
    setCropBottom(pt.get(L"cropbottom", Mixer::DEFAULT_CROP_BOTTOM));
    setTransitionDuration(pt.get(L"transtitionDuration", Mixer::DEFAULT_DURATION));
    setTween(QString::fromStdWString(pt.get(L"tween", Mixer::DEFAULT_TWEEN.toStdWString())));
    setDefer(pt.get(L"defer", Mixer::DEFAULT_DEFER));
}

void CropCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("cropleft", QString::number(getCropLeft()));
    writer->writeTextElement("cropright", QString::number(getCropRight()));
    writer->writeTextElement("croptop", QString::number(getCropTop()));
    writer->writeTextElement("cropbottom", QString::number(getCropBottom()));
    writer->writeTextElement("transtitionDuration", QString::number(getTransitionDuration()));
    writer->writeTextElement("tween", this->getTween());
    writer->writeTextElement("defer", (getDefer() == true) ? "true" : "false");
}
