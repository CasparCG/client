#include "HttpGetCommand.h"

#include <QtCore/QXmlStreamWriter>

HttpGetCommand::HttpGetCommand(QObject* parent)
    : AbstractCommand(parent)
{
}

const QString& HttpGetCommand::getUrl() const
{
    return this->url;
}

const QList<KeyValueModel>& HttpGetCommand::getHttpDataModels() const
{
    return this->models;
}

const QUrlQuery HttpGetCommand::getHttpData() const
{
    QUrlQuery httpData;

    foreach (KeyValueModel model, this->models)
        httpData.addQueryItem(model.getKey(), model.getValue());

    return httpData;
}

bool HttpGetCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

void HttpGetCommand::setUrl(const QString& url)
{
    this->url = url;
    emit urlChanged(this->url);
}

void HttpGetCommand::setHttpDataModels(const QList<KeyValueModel>& models)
{
    this->models = models;
    emit httpDataChanged(this->models);
}

void HttpGetCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void HttpGetCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setUrl(QString::fromStdWString(pt.get(L"url", Http::DEFAULT_URL.toStdWString())));
    setTriggerOnNext(pt.get(L"triggeronnext", Http::DEFAULT_TRIGGER_ON_NEXT));

    if (pt.count(L"httpdata") > 0)
    {
        for (const boost::property_tree::wptree::value_type &value : pt.get_child(L"httpdata"))
        {
            this->models.push_back(KeyValueModel(QString::fromStdWString(value.second.get<std::wstring>(L"key")),
                                                 QString::fromStdWString(value.second.get<std::wstring>(L"value"))));
        }
    }
}

void HttpGetCommand::writeProperties(QXmlStreamWriter& writer)
{
    AbstractCommand::writeProperties(writer);

    writer.writeTextElement("url", getUrl());
    writer.writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");

    if (this->models.count() > 0)
    {
        writer.writeStartElement("httpdata");
        foreach (KeyValueModel model, this->models)
        {
            writer.writeStartElement("componentdata");
            writer.writeTextElement("key", model.getKey());
            writer.writeTextElement("value", model.getValue());
            writer.writeEndElement();
        }
        writer.writeEndElement();
    }
}
