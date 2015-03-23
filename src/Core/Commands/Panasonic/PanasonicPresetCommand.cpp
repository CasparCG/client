#include "PanasonicPresetCommand.h"

#include <QtCore/QXmlStreamWriter>

PanasonicPresetCommand::PanasonicPresetCommand(QObject* parent)
    : AbstractCommand(parent)
{
}

const QString& PanasonicPresetCommand::getAddress() const
{
    return this->address;
}

int PanasonicPresetCommand::getPreset() const
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

void PanasonicPresetCommand::setPreset(int preset)
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
    setPreset(pt.get(L"preset", Panasonic::DEFAULT_PRESET));
    setTriggerOnNext(pt.get(L"triggeronnext", Panasonic::DEFAULT_TRIGGER_ON_NEXT));
}

void PanasonicPresetCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("address", getAddress());
    writer->writeTextElement("preset", QString::number(getPreset()));
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
}
