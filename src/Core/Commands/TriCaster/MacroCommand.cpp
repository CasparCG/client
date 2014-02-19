#include "MacroCommand.h"

#include "Global.h"

MacroCommand::MacroCommand(QObject* parent)
    : AbstractCommand(parent),
      macro(TriCaster::DEFAULT_MACRO), triggerOnNext(TriCaster::DEFAULT_TRIGGER_ON_NEXT)
{
}

const QString& MacroCommand::getMacro() const
{
    return this->macro;
}

bool MacroCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

void MacroCommand::setMacro(const QString& macro)
{
    this->macro = macro;
    emit macroChanged(this->macro);
}

void MacroCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void MacroCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setMacro(QString::fromStdWString(pt.get(L"macro", TriCaster::DEFAULT_MACRO.toStdWString())));
    setTriggerOnNext(pt.get(L"triggeronnext", TriCaster::DEFAULT_TRIGGER_ON_NEXT));
}

void MacroCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("macro", this->getMacro());
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
}

