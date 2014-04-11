#include "AtemVideoFormatCommand.h"

#include "Global.h"

AtemVideoFormatCommand::AtemVideoFormatCommand(QObject* parent)
    : AbstractCommand(parent),
      format(Atem::DEFAULT_VIDEO_FORMAT), triggerOnNext(Atem::DEFAULT_TRIGGER_ON_NEXT)
{
}

const QString& AtemVideoFormatCommand::getFormat() const
{
    return this->format;
}

bool AtemVideoFormatCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

void AtemVideoFormatCommand::setFormat(const QString& format)
{
    this->format = format;
    emit formatChanged(this->format);
}

void AtemVideoFormatCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void AtemVideoFormatCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setFormat(QString::fromStdWString(pt.get(L"videoformat", Atem::DEFAULT_VIDEO_FORMAT.toStdWString())));
    setTriggerOnNext(pt.get(L"triggeronnext", Atem::DEFAULT_TRIGGER_ON_NEXT));
}

void AtemVideoFormatCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("videoformat", this->getFormat());
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
}
