#include "PresetCommand.h"

#include "Global.h"

PresetCommand::PresetCommand(QObject* parent)
    : AbstractCommand(parent),
      source(TriCaster::DEFAULT_SOURCE), preset(TriCaster::DEFAULT_PRESET)
{
}

const QString& PresetCommand::getSource() const
{
    return this->source;
}

const QString& PresetCommand::getPreset() const
{
    return this->preset;
}

void PresetCommand::setSource(const QString& source)
{
    this->source = source;
    emit sourceChanged(this->source);
}

void PresetCommand::setPreset(const QString& preset)
{
    this->preset = preset;
    emit presetChanged(this->preset);
}

void PresetCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setSource(QString::fromStdWString(pt.get(L"source", TriCaster::DEFAULT_SOURCE.toStdWString())));
    setPreset(QString::fromStdWString(pt.get(L"preset", TriCaster::DEFAULT_PRESET.toStdWString())));
}

void PresetCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("source", this->getSource());
    writer->writeTextElement("preset", this->getPreset());
}

