#include "TemplateCommand.h"

#include "Global.h"

TemplateCommand::TemplateCommand(QObject* parent)
    : AbstractCommand(parent),
      flashlayer(Template::DEFAULT_FLASHLAYER), invoke(Template::DEFAULT_INVOKE), useStoredData(Template::DEFAULT_USE_STORED_DATA),
      useUppercaseData(Template::DEFAULT_USE_UPPERCASE_DATA), templateName(Template::DEFAULT_TEMPLATENAME), triggerOnNext(Template::DEFAULT_TRIGGER_ON_NEXT)
{
    this->videolayer = Output::DEFAULT_FLASH_VIDEOLAYER;
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

bool TemplateCommand::getUseUppercaseData() const
{
    return this->useUppercaseData;
}

const QString& TemplateCommand::getTemplateName() const
{
    return this->templateName;
}

const QString TemplateCommand::getTemplateData() const
{
    QString templateData;
    if (this->useStoredData)
    {
        if (this->models.count() == 0)
            return "";

        templateData.append(this->models.at(0).getValue());
    }
    else
    {
        templateData.append("<templateData>");
        foreach (KeyValueModel model, this->models)
        {
            QString componentData = TemplateData::DEFAULT_COMPONENT_DATA_XML;
            componentData.replace("#KEY", model.getKey());

            QString value = model.getValue();
            value = Xml::encode(value).replace("\\", "\\\\");
            componentData.replace("#VALUE", (this->useUppercaseData == true) ? value.toUpper() : value);

            templateData.append(componentData);
        }
        templateData.append("</templateData>");
    }

    return templateData;
}

const QList<KeyValueModel>& TemplateCommand::getTemplateDataModels() const
{
    return this->models;
}

bool TemplateCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
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

void TemplateCommand::setUseUppercaseData(bool useUppercaseData)
{
    this->useUppercaseData = useUppercaseData;
    emit useUppercaseDataChanged(this->useUppercaseData);
}

void TemplateCommand::setTemplateName(const QString& templateName)
{
    this->templateName = templateName;
    emit templateNameChanged(this->templateName);
}

void TemplateCommand::setTemplateDataModels(const QList<KeyValueModel>& models)
{
    this->models = models;
    emit templateDataChanged(this->models);
}

void TemplateCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void TemplateCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setFlashlayer(pt.get(L"flashlayer", Template::DEFAULT_FLASHLAYER));
    setInvoke(QString::fromStdWString(pt.get(L"invoke", Template::DEFAULT_INVOKE.toStdWString())));
    setUseStoredData(pt.get(L"usestoreddata", Template::DEFAULT_USE_STORED_DATA));
    setUseUppercaseData(pt.get(L"useuppercasedata", Template::DEFAULT_USE_UPPERCASE_DATA));
    setTriggerOnNext(pt.get(L"triggeronnext", Template::DEFAULT_TRIGGER_ON_NEXT));

    if (pt.count(L"templatedata") > 0)
    {
        BOOST_FOREACH(const boost::property_tree::wptree::value_type& value, pt.get_child(L"templatedata"))
        {
            this->models.push_back(KeyValueModel(QString::fromStdWString(value.second.get<std::wstring>(L"key")),
                                                 QString::fromStdWString(value.second.get<std::wstring>(L"value"))));
        }
    }
}

void TemplateCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("flashlayer", QString::number(this->getFlashlayer()));
    writer->writeTextElement("invoke", this->getInvoke());
    writer->writeTextElement("usestoreddata", (getUseStoredData() == true) ? "true" : "false");
    writer->writeTextElement("useuppercasedata", (getUseUppercaseData() == true) ? "true" : "false");
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");

    if (this->models.count() > 0)
    {
        writer->writeStartElement("templatedata");
        foreach (KeyValueModel model, this->models)
        {
            writer->writeStartElement("componentdata");
            writer->writeTextElement("key", model.getKey());
            writer->writeTextElement("value", model.getValue());
            writer->writeEndElement();
        }
        writer->writeEndElement();
    }
}
