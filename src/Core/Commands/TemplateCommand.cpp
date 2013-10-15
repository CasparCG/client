#include "TemplateCommand.h"

#include "Global.h"

#include <QtCore/QRegExp>

TemplateCommand::TemplateCommand(QObject* parent)
    : AbstractCommand(parent),
      flashlayer(Template::DEFAULT_FLASHLAYER), invoke(Template::DEFAULT_INVOKE), useStoredData(false), useUppercaseData(false), templateName(Template::DEFAULT_TEMPLATENAME)
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
        foreach (TemplateDataModel model, this->models)
        {
            QString componentData = TemplateData::DEFAULT_COMPONENT_DATA_XML;
            componentData.replace(QRegExp("#KEY"), model.getKey());

            QString value = model.getValue();
            value = Xml::encode(value).replace("\\", "\\\\");
            componentData.replace(QRegExp("#VALUE"), (this->useUppercaseData == true) ? value.toUpper() : value);

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

void TemplateCommand::setTemplateDataModels(const QList<TemplateDataModel>& models)
{
    this->models = models;
    emit templateDataChanged(this->models);
}

void TemplateCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    if (pt.count(L"flashlayer") > 0) setFlashlayer(pt.get<int>(L"flashlayer"));
    if (pt.count(L"invoke") > 0) setInvoke(QString::fromStdWString(pt.get<std::wstring>(L"invoke")));
    if (pt.count(L"usestoreddata") > 0) setUseStoredData(pt.get<bool>(L"usestoreddata"));
    if (pt.count(L"useuppercasedata") > 0) setUseUppercaseData(pt.get<bool>(L"useuppercasedata"));

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
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("flashlayer", QString::number(this->getFlashlayer()));
    writer->writeTextElement("invoke", this->getInvoke());
    writer->writeTextElement("usestoreddata", (getUseStoredData() == true) ? "true" : "false");
    writer->writeTextElement("useuppercasedata", (getUseUppercaseData() == true) ? "true" : "false");

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
