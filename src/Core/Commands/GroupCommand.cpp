#include "GroupCommand.h"

#include "Global.h"

GroupCommand::GroupCommand(QObject* parent)
    : AbstractCommand(parent),
      notes(Group::DEFAULT_NOTE), autoStep(Group::DEFAULT_AUTO_STEP), autoPlay(Group::DEFAULT_AUTO_PLAY), countdown("")
{
}

bool GroupCommand::getAutoStep() const
{
    return this->autoStep;
}

const QString& GroupCommand::getNotes() const
{
    return this->notes;
}

bool GroupCommand::getAutoPlay() const
{
    return this->autoPlay;
}

const QString& GroupCommand::getCountdown() const
{
    return this->countdown;
}

void GroupCommand::setAutoStep(bool autoStep)
{
    this->autoStep = autoStep;
    emit autoStepChanged(this->autoStep);
}

void GroupCommand::setAutoPlay(bool autoPlay)
{
    this->autoPlay = autoPlay;
    emit autoPlayChanged(this->autoPlay);
}

void GroupCommand::setNotes(const QString& notes)
{
    this->notes = notes;
    emit notesChanged(this->notes);
}

void GroupCommand::setCountdown(const QString& countdown)
{
    this->countdown = countdown;
    emit countdownChanged(this->countdown);
}

void GroupCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setNotes(QString::fromStdWString(pt.get(L"notes", Group::DEFAULT_NOTE.toStdWString())));
    setAutoStep(pt.get(L"autostep", Group::DEFAULT_AUTO_STEP));
    setAutoPlay(pt.get(L"autoplay", Group::DEFAULT_AUTO_PLAY));
    setCountdown(QString::fromStdWString(pt.get(L"countdown", L"")));
}

void GroupCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("notes", this->getNotes());
    writer->writeTextElement("autostep", (getAutoStep() == true) ? "true" : "false");
    writer->writeTextElement("autoplay", (getAutoPlay() == true) ? "true" : "false");
    writer->writeTextElement("countdown", this->getCountdown());
}
