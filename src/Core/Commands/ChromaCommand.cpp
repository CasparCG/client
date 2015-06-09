#include "ChromaCommand.h"

#include <QtCore/QXmlStreamWriter>

ChromaCommand::ChromaCommand(QObject* parent)
    : AbstractCommand(parent)
{
}

const QString& ChromaCommand::getKey() const
{
    return this->key;
}

float ChromaCommand::getSpread() const
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

void ChromaCommand::setKey(const QString& chromaKey)
{
    this->key = chromaKey;
    emit keyChanged(this->key);
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

    setKey(QString::fromStdWString(pt.get(L"key", Mixer::DEFAULT_CHROMAKEY.toStdWString())));
    setSpread(pt.get(L"spread", Mixer::DEFAULT_CHROMABLENDSTOP));
    setSpill(pt.get(L"spill", Mixer::DEFAULT_CHROMASPILL));
    setThreshold(pt.get(L"threshold", Mixer::DEFAULT_CHROMABLENDSTART));
}

void ChromaCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("key", this->getKey());
    writer->writeTextElement("spread",  QString::number(this->getSpread()));
    writer->writeTextElement("spill", QString::number(this->getSpill()));
    writer->writeTextElement("threshold", QString::number(this->getThreshold()));
}
