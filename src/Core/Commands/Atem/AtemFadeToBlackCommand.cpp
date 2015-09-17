#include "AtemFadeToBlackCommand.h"

#include <QtCore/QXmlStreamWriter>

AtemFadeToBlackCommand::AtemFadeToBlackCommand(QObject* parent)
    : AbstractCommand(parent)
{
}

bool AtemFadeToBlackCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

const QString& AtemFadeToBlackCommand::getMixerStep() const
{
    return this->mixerStep;
}

double AtemFadeToBlackCommand::getSpeed() const
{
    return this->speed;
}

void AtemFadeToBlackCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void AtemFadeToBlackCommand::setMixerStep(const QString& mixerStep)
{
    this->mixerStep = mixerStep;
    emit mixerStepChanged(this->mixerStep);
}

void AtemFadeToBlackCommand::setSpeed(double speed)
{
    this->speed = speed;
    emit speedChanged(this->speed);
}

void AtemFadeToBlackCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setSpeed(pt.get(L"speed", Atem::DEFAULT_SPEED));
    setTriggerOnNext(pt.get(L"triggeronnext", Atem::DEFAULT_TRIGGER_ON_NEXT));
    setMixerStep(QString::fromStdWString(pt.get(L"mixerstep", Atem::DEFAULT_MIXER_STEP.toStdWString())));
}

void AtemFadeToBlackCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("speed", QString::number(this->getSpeed()));
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
    writer->writeTextElement("mixerstep", this->getMixerStep());
}
