#include "TemplateCommand.h"

#include "Global.h"

#include <QtCore/QRegExp>

TemplateCommand::TemplateCommand(QObject* parent)
    : QObject(parent),
      channel(Output::DEFAULT_CHANNEL), videolayer(Output::DEFAULT_VIDEOLAYER_FLASH), delay(Output::DEFAULT_DELAY),
      allowGpi(Output::DEFAULT_ALLOW_GPI), flashlayer(Template::DEFAULT_FLASHLAYER), invoke(Template::DEFAULT_INVOKE),
      useStoredData(false), templateName(Template::DEFAULT_TEMPLATENAME)
{
}

int TemplateCommand::getDelay() const
{
    return this->delay;
}

bool TemplateCommand::getAllowGpi() const
{
    return this->allowGpi;
}

int TemplateCommand::getChannel() const
{
    return this->channel;
}

int TemplateCommand::getVideolayer() const
{
    return this->videolayer;
}

void TemplateCommand::setChannel(int channel)
{
    this->channel = channel;
    emit channelChanged(this->channel);
}

void TemplateCommand::setVideolayer(int videolayer)
{
    this->videolayer = videolayer;
    emit videolayerChanged(this->videolayer);
}

void TemplateCommand::setDelay(int delay)
{
    this->delay = delay;
    emit delayChanged(this->delay);
}

void TemplateCommand::setAllowGpi(bool allowGpi)
{
    this->allowGpi = allowGpi;
    emit allowGpiChanged(this->allowGpi);
}

int TemplateCommand::getFlashlayer() const
{
    return this->flashlayer;
}

const QString& TemplateCommand::getInvoke() const
{
    return this->invoke;
}

bool TemplateCommand::getUseStoredData() const
{
    return this->useStoredData;
}

const QString& TemplateCommand::getTemplateName() const
{
    return this->templateName;
}

const QString TemplateCommand::getTemplateData() const
{
    if (this->models.count() == 0)
        return "";

    QString templateData;
    if (this->useStoredData)
    {
        templateData.append(this->models.at(0).getValue());
    }
    else
    {
        templateData.append("<templateData>");
        foreach (TemplateDataModel model, this->models)
        {
            QString componentData = TemplateData::DEFAULT_COMPONENT_DATA_XML;
            componentData.replace(QRegExp("#KEY"), model.getKey());

            QString value = model.getValue();
            value = value.replace("\"", "&quot;").replace("<", "&lt;").replace(">", "&gt;").replace("\\", "\\\\");
            componentData.replace(QRegExp("#VALUE"), value);

            templateData.append(componentData);
        }
        templateData.append("</templateData>");
    }

    return templateData;
}

const QList<TemplateDataModel>& TemplateCommand::getTemplateDataModels() const
{
    return this->models;
}

void TemplateCommand::setFlashlayer(int flashlayer)
{
    this->flashlayer = flashlayer;
    emit flashlayerChanged(this->flashlayer);
}

void TemplateCommand::setInvoke(const QString& invoke)
{
    this->invoke = invoke;
    emit invokeChanged(this->invoke);
}

void TemplateCommand::setUseStoredData(bool useStoredData)
{
    this->useStoredData = useStoredData;
    emit useStoredDataChanged(this->useStoredData);
}

void TemplateCommand::setTemplateName(const QString& templateName)
{
    this->templateName = templateName;
    emit templateNameChanged(this->templateName);
}

void TemplateCommand::setTemplateDataModels(const QList<TemplateDataModel>& models)
{
    this->models = models;
    emit templateDataChanged(this->models);
}

void TemplateCommand::readProperties(boost::property_tree::wptree& pt)
{
    if (pt.count(L"channel") > 0) setChannel(pt.get<int>(L"channel"));
    if (pt.count(L"videolayer") > 0) setVideolayer(pt.get<int>(L"videolayer"));
    if (pt.count(L"delay") > 0) setDelay(pt.get<int>(L"delay"));
    if (pt.count(L"allowgpi") > 0) setAllowGpi(pt.get<bool>(L"allowgpi"));
    if (pt.count(L"flashlayer") > 0) setFlashlayer(pt.get<int>(L"flashlayer"));
    if (pt.count(L"invoke") > 0) setInvoke(QString::fromStdWString(pt.get<std::wstring>(L"invoke")));
    if (pt.count(L"usestoreddata") > 0) setUseStoredData(pt.get<bool>(L"usestoreddata"));

    if (pt.count(L"templatedata") > 0)
    {
        BOOST_FOREACH(const boost::property_tree::wptree::value_type& value, pt.get_child(L"templatedata"))
        {
            this->models.push_back(TemplateDataModel(QString::fromStdWString(value.second.get<std::wstring>(L"id")),
                                                     QString::fromStdWString(value.second.get<std::wstring>(L"value"))));
        }
    }
}

void TemplateCommand::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("channel", QString::number(this->getChannel()));
    writer->writeTextElement("videolayer", QString::number(this->getVideolayer()));
    writer->writeTextElement("delay", QString::number(this->getDelay()));
    writer->writeTextElement("allowgpi", (getAllowGpi() == true) ? "true" : "false");
    writer->writeTextElement("flashlayer", QString::number(this->getFlashlayer()));
    writer->writeTextElement("invoke", this->getInvoke());
    writer->writeTextElement("usestoreddata", (getUseStoredData() == true) ? "true" : "false");

    if (this->models.count() > 0)
    {
        writer->writeStartElement("templatedata");
        foreach (TemplateDataModel model, this->models)
        {
            writer->writeStartElement("componentdata");
            writer->writeTextElement("id", model.getKey());
            writer->writeTextElement("value", model.getValue());
            writer->writeEndElement();
        }
        writer->writeEndElement();
    }
}
