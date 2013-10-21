#include "ChromaCommand.h"

#include "Global.h"

ChromaCommand::ChromaCommand(QObject* parent)
    : AbstractCommand(parent),
    key(Mixer::DEFAULT_CHROMAKEY), blur(Mixer::DEFAULT_CHROMABLUR), spread(Mixer::DEFAULT_CHROMABLENDSTOP),
    spill(Mixer::DEFAULT_CHROMASPILL), threshold(Mixer::DEFAULT_CHROMABLENDSTART), showMask(false)
{
}

float ChromaCommand::getBlur() const
{
    return this->blur;
}

const QString& ChromaCommand::getKey() const
{
    return this->key;
}

bool ChromaCommand::getShowMask() const
{
    return this->showMask;
}

float ChromaCommand::getSoftness() const
{
    return this->spread;
}

float ChromaCommand::getSpill() const
{
    return this->spill;
}

float ChromaCommand::getThreshold() const
{
    return this->threshold;
}

void ChromaCommand::setBlur(float blur)
{
    this->blur = blur;
    emit blurChanged(this->blur);
}

void ChromaCommand::setKey(const QString& chromaKey)
{
    this->key = chromaKey;
    emit keyChanged(this->key);
}

void ChromaCommand::setShowMask(bool show)
{
    this->showMask = show;
    emit showMaskChanged(this->showMask);
}

void ChromaCommand::setSpread(float stop)
{
    this->spread = stop;
    emit spreadChanged(this->spread);
}

void ChromaCommand::setSpill(float spill)
{
    this->spill = spill;
    emit spillChanged(this->spill);
}

void ChromaCommand::setThreshold(float start)
{
    this->threshold = start;
    emit thresholdChanged(this->threshold);
}

void ChromaCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    if (pt.count(L"blur") > 0) setSpread(pt.get<float>(L"blur"));
    if (pt.count(L"key") > 0) setKey(QString::fromStdWString(pt.get<std::wstring>(L"key")));
    if (pt.count(L"softness") > 0) setSpread(pt.get<float>(L"softness"));
    if (pt.count(L"spill") > 0) setSpill(pt.get<float>(L"spill"));
    if (pt.count(L"threshold") > 0) setThreshold(pt.get<float>(L"threshold"));
    if (pt.count(L"showmask") > 0) setShowMask(pt.get<bool>(L"showmask"));
}

void ChromaCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("showmask", (getShowMask() == true) ? "true" : "false");
    writer->writeTextElement("blur",  QString::number(this->getBlur()));
    writer->writeTextElement("key", this->getKey());
    writer->writeTextElement("softness",  QString::number(this->getSoftness()));
    writer->writeTextElement("spill", QString::number(this->getSpill()));
    writer->writeTextElement("threshold", QString::number(this->getThreshold()));
}
