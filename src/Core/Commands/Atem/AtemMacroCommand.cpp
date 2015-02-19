#include "AtemMacroCommand.h"

#include "Global.h"

AtemMacroCommand::AtemMacroCommand(QObject* parent)
    : AbstractCommand(parent),
      macro(Atem::DEFAULT_MACRO), triggerOnNext(Atem::DEFAULT_TRIGGER_ON_NEXT)
{
}

const QString& AtemMacroCommand::getMacro() const
{
    return this->macro;
}

bool AtemMacroCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

void AtemMacroCommand::setMacro(const QString& macro)
{
    this->macro = macro;
    emit macroChanged(this->macro);
}

void AtemMacroCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void AtemMacroCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setMacro(QString::fromStdWString(pt.get(L"macro", Atem::DEFAULT_MACRO.toStdWString())));
    setTriggerOnNext(pt.get(L"triggeronnext", Atem::DEFAULT_TRIGGER_ON_NEXT));
}

void AtemMacroCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("macro", this->getMacro());
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
}
