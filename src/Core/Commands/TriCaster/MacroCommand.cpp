#include "MacroCommand.h"

#include "Global.h"

MacroCommand::MacroCommand(QObject* parent)
    : AbstractCommand(parent),
      name(TriCaster::DEFAULT_MACRO)
{
}

const QString& MacroCommand::getName() const
{
    return this->name;
}

void MacroCommand::setName(const QString& name)
{
    this->name = name;
    emit nameChanged(this->name);
}

void MacroCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setName(QString::fromStdWString(pt.get(L"name", TriCaster::DEFAULT_MACRO.toStdWString())));
}

void MacroCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("name", this->getName());
}

