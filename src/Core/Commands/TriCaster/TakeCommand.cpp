#include "TakeCommand.h"

#include "Global.h"

TakeCommand::TakeCommand(QObject* parent)
    : AbstractCommand(parent),
      step(TriCaster::DEFAULT_STEP)
{
}

void TakeCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);
}

void TakeCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);
}

const QString& TakeCommand::getStep() const
{
    return this->step;
}

void TakeCommand::setStep(const QString& step)
{
    this->step = step;
    emit stepChanged(this->step);
}
