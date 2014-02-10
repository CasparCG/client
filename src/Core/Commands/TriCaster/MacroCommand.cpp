#include "MacroCommand.h"

#include "Global.h"

MacroCommand::MacroCommand(QObject* parent)
    : AbstractCommand(parent),
      macro(TriCaster::DEFAULT_MACRO)
{
}

const QString& MacroCommand::getMacro() const
{
    return this->macro;
}

void MacroCommand::setMacro(const QString& macro)
{
    this->macro = macro;
    emit macroChanged(this->macro);
}

void MacroCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setMacro(QString::fromStdWString(pt.get(L"macro", TriCaster::DEFAULT_MACRO.toStdWString())));
}

void MacroCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("macro", this->getMacro());
}

