#include "PanasonicPresetCommand.h"

#include "Global.h"

PanasonicPresetCommand::PanasonicPresetCommand(QObject* parent)
    : AbstractCommand(parent),
      address(Panasonic::DEFAULT_ADDRESS), preset(Panasonic::DEFAULT_PRESET), triggerOnNext(Panasonic::DEFAULT_TRIGGER_ON_NEXT)
{
}

const QString& PanasonicPresetCommand::getAddress() const
{
    return this->address;
}

const QString& PanasonicPresetCommand::getPreset() const
{
    return this->preset;
}

bool PanasonicPresetCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

void PanasonicPresetCommand::setAddress(const QString& address)
{
    this->address = address;
    emit addressChanged(this->address);
}

void PanasonicPresetCommand::setPreset(const QString& preset)
{
    this->preset = preset;
    emit presetChanged(this->preset);
}

void PanasonicPresetCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void PanasonicPresetCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setAddress(QString::fromStdWString(pt.get(L"address", Panasonic::DEFAULT_ADDRESS.toStdWString())));
    setPreset(QString::fromStdWString(pt.get(L"preset", Panasonic::DEFAULT_PRESET.toStdWString())));
    setTriggerOnNext(pt.get(L"triggeronnext", Panasonic::DEFAULT_TRIGGER_ON_NEXT));
}

void PanasonicPresetCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("address", getAddress());
    writer->writeTextElement("preset", getPreset());
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
}
