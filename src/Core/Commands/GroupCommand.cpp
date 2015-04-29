#include "GroupCommand.h"

#include <QtCore/QXmlStreamWriter>

GroupCommand::GroupCommand(QObject* parent)
    : AbstractCommand(parent)
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

void GroupCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setNotes(QString::fromStdWString(pt.get(L"notes", Group::DEFAULT_NOTE.toStdWString())));
    setAutoStep(pt.get(L"autostep", Group::DEFAULT_AUTO_STEP));
    setAutoPlay(pt.get(L"autoplay", Group::DEFAULT_AUTO_PLAY));
}

void GroupCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("notes", this->getNotes());
    writer->writeTextElement("autostep", (getAutoStep() == true) ? "true" : "false");
    writer->writeTextElement("autoplay", (getAutoPlay() == true) ? "true" : "false");
}
