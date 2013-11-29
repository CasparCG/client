#include "AutoCommand.h"

#include "Global.h"

AutoCommand::AutoCommand(QObject* parent)
    : AbstractCommand(parent),
      step(TriCaster::DEFAULT_STEP)
{
}

void AutoCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);
}

void AutoCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);
}

const QString& AutoCommand::getStep() const
{
    return this->step;
}

void AutoCommand::setStep(const QString& step)
{
    this->step = step;
    emit stepChanged(this->step);
}
