#include "PresetCommand.h"

#include "Global.h"

PresetCommand::PresetCommand(QObject* parent)
    : AbstractCommand(parent),
      input(TriCaster::DEFAULT_PRESET_INPUT), preset(TriCaster::DEFAULT_PRESET)
{
}

const QString& PresetCommand::getInput() const
{
    return this->input;
}

const QString& PresetCommand::getPreset() const
{
    return this->preset;
}

void PresetCommand::setInput(const QString& input)
{
    this->input = input;
    emit inputChanged(this->input);
}

void PresetCommand::setPreset(const QString& preset)
{
    this->preset = preset;
    emit presetChanged(this->preset);
}

void PresetCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setInput(QString::fromStdWString(pt.get(L"input", TriCaster::DEFAULT_PRESET_INPUT.toStdWString())));
    setPreset(QString::fromStdWString(pt.get(L"preset", TriCaster::DEFAULT_PRESET.toStdWString())));
}

void PresetCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("input", this->getInput());
    writer->writeTextElement("preset", this->getPreset());
}

