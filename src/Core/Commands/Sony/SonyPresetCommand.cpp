#include "SonyPresetCommand.h"

#include "Global.h"

SonyPresetCommand::SonyPresetCommand(QObject* parent)
    : AbstractCommand(parent),
      address(Sony::DEFAULT_ADDRESS), preset(Sony::DEFAULT_PRESET), triggerOnNext(Sony::DEFAULT_TRIGGER_ON_NEXT)
{
}

const QString& SonyPresetCommand::getAddress() const
{
    return this->address;
}

int SonyPresetCommand::getPreset() const
{
    return this->preset;
}

bool SonyPresetCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

void SonyPresetCommand::setAddress(const QString& address)
{
    this->address = address;
    emit addressChanged(this->address);
}

void SonyPresetCommand::setPreset(int preset)
{
    this->preset = preset;
    emit presetChanged(this->preset);
}

void SonyPresetCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void SonyPresetCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setAddress(QString::fromStdWString(pt.get(L"address", Sony::DEFAULT_ADDRESS.toStdWString())));
    setPreset(pt.get(L"preset", Sony::DEFAULT_PRESET));
    setTriggerOnNext(pt.get(L"triggeronnext", Sony::DEFAULT_TRIGGER_ON_NEXT));
}

void SonyPresetCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("address", getAddress());
    writer->writeTextElement("preset", QString::number(getPreset()));
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
}
