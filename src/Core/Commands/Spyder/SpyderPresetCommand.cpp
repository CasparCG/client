#include "SpyderPresetCommand.h"

#include <QtCore/QXmlStreamWriter>

SpyderPresetCommand::SpyderPresetCommand(QObject* parent)
    : AbstractCommand(parent)
{
}

const QString& SpyderPresetCommand::getAddress() const
{
    return this->address;
}

int SpyderPresetCommand::getPreset() const
{
    return this->preset;
}

bool SpyderPresetCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

void SpyderPresetCommand::setAddress(const QString& address)
{
    this->address = address;
    emit addressChanged(this->address);
}

void SpyderPresetCommand::setPreset(int preset)
{
    this->preset = preset;
    emit presetChanged(this->preset);
}

void SpyderPresetCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void SpyderPresetCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setAddress(QString::fromStdWString(pt.get(L"address", Spyder::DEFAULT_ADDRESS.toStdWString())));
    setPreset(pt.get(L"preset", Spyder::DEFAULT_PRESET));
    setTriggerOnNext(pt.get(L"triggeronnext", Spyder::DEFAULT_TRIGGER_ON_NEXT));
}

void SpyderPresetCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("address", getAddress());
    writer->writeTextElement("preset", QString::number(getPreset()));
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
}
