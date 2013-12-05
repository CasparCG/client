#include "TakeCommand.h"

#include "Global.h"

TakeCommand::TakeCommand(QObject* parent)
    : AbstractCommand(parent),
      step(TriCaster::DEFAULT_STEP)
{
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

void TakeCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setStep(QString::fromStdWString(pt.get(L"step", TriCaster::DEFAULT_STEP.toStdWString())));
}

void TakeCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("step", this->getStep());
}
