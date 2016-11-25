#include "AtemCutCommand.h"

#include <QtCore/QXmlStreamWriter>

AtemCutCommand::AtemCutCommand(QObject* parent)
    : AbstractCommand(parent)
{
}

const QString& AtemCutCommand::getStep() const
{
    return this->step;
}

bool AtemCutCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

const QString& AtemCutCommand::getMixerStep() const
{
    return this->mixerStep;
}


void AtemCutCommand::setStep(const QString& step)
{
    this->step = step;
    emit stepChanged(this->step);
}

void AtemCutCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void AtemCutCommand::setMixerStep(const QString& mixerStep)
{
    this->mixerStep = mixerStep;
    emit mixerStepChanged(this->mixerStep);
}

void AtemCutCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setStep(QString::fromStdWString(pt.get(L"step", L"")));
    setTriggerOnNext(pt.get(L"triggeronnext", Atem::DEFAULT_TRIGGER_ON_NEXT));
    setMixerStep(QString::fromStdWString(pt.get(L"mixerstep", Atem::DEFAULT_MIXER_STEP.toStdWString())));
}

void AtemCutCommand::writeProperties(QXmlStreamWriter& writer)
{
    AbstractCommand::writeProperties(writer);

    writer.writeTextElement("step", this->getStep());
    writer.writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
    writer.writeTextElement("mixerstep", this->getMixerStep());
}
