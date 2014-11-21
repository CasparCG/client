#include "OscOutputCommand.h"

#include "Global.h"

OscOutputCommand::OscOutputCommand(QObject* parent)
    : AbstractCommand(parent),
      output(Osc::DEFAULT_OUTPUT), path(""), message(""), type(Osc::DEFAULT_TYPE),
      triggerOnNext(Osc::DEFAULT_TRIGGER_ON_NEXT), useBundle(Osc::DEFAULT_USE_BUNDLE)
{
}

const QString& OscOutputCommand::getOutput() const
{
    return this->output;
}

const QString& OscOutputCommand::getPath() const
{
    return this->path;
}

const QString& OscOutputCommand::getMessage() const
{
    return this->message;
}

const QString& OscOutputCommand::getType() const
{
    return this->type;
}

bool OscOutputCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

bool OscOutputCommand::getUseBundle() const
{
    return this->useBundle;
}

void OscOutputCommand::setOutput(const QString& output)
{
    this->output = output;
    emit outputChanged(this->output);
}

void OscOutputCommand::setPath(const QString& path)
{
    this->path = path;
    emit pathChanged(this->path);
}

void OscOutputCommand::setMessage(const QString& message)
{
    this->message = message;
    emit messageChanged(this->message);
}

void OscOutputCommand::setType(const QString& type)
{
    this->type = type;
    emit typeChanged(this->type);
}

void OscOutputCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void OscOutputCommand::setUseBundle(bool useBundle)
{
    this->useBundle = useBundle;
    emit useBundleChanged(this->useBundle);
}

void OscOutputCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setOutput(QString::fromStdWString(pt.get(L"output", Osc::DEFAULT_OUTPUT.toStdWString())));
    setPath(QString::fromStdWString(pt.get(L"path", L"")));
    setMessage(QString::fromStdWString(pt.get(L"message", L"")));
    setType(QString::fromStdWString(pt.get(L"osctype", Osc::DEFAULT_TYPE.toStdWString())));
    setTriggerOnNext(pt.get(L"triggeronnext", Osc::DEFAULT_TRIGGER_ON_NEXT));
    setUseBundle(pt.get(L"usebundle", Osc::DEFAULT_USE_BUNDLE));
}

void OscOutputCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("output", this->getOutput());
    writer->writeTextElement("path", this->getPath());
    writer->writeTextElement("message", this->getMessage());
    writer->writeTextElement("osctype", this->getType());
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
    writer->writeTextElement("usebundle", (getUseBundle() == true) ? "true" : "false");
}
