#pragma once

#include "../Shared.h"
#include "AbstractCommand.h"
#include "Models/KeyValueModel.h"

#include "Global.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QString>
#include <QJsonObject>
#include <QJsonDocument>

class QObject;
class QXmlStreamWriter;

class CORE_EXPORT TemplateCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit TemplateCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter& writer);

        int getFlashlayer() const;
        const QString& getInvoke() const;
        bool getUseStoredData() const;
        bool getSendAsJson() const;
        bool getUseUppercaseData() const;
        const QString& getTemplateName() const;
        const QString getTemplateData() const;
        const QList<KeyValueModel>& getTemplateDataModels() const;
        bool getTriggerOnNext() const;

        void setFlashlayer(int flashlayer); 
        void setInvoke(const QString& invoke);
        void setUseStoredData(bool useStoredData);
        void setSendAsJson(bool sendAsJson);
        void setUseUppercaseData(bool useUppercaseData);
        void setTemplateName(const QString& templateName);
        void setTemplateDataModels(const QList<KeyValueModel>& models);
        void setTriggerOnNext(bool triggerOnNext);

    private:
        int flashlayer = Template::DEFAULT_FLASHLAYER;
        QString invoke = Template::DEFAULT_INVOKE;
        bool useStoredData = Template::DEFAULT_USE_STORED_DATA;
        bool useUppercaseData = Template::DEFAULT_USE_UPPERCASE_DATA;
        QString templateName = Template::DEFAULT_TEMPLATENAME;
        QList<KeyValueModel> models;
        bool triggerOnNext = Template::DEFAULT_TRIGGER_ON_NEXT;
        bool sendAsJson = Template::DEFAULT_SEND_AS_JSON;

        Q_SIGNAL void flashlayerChanged(int);
        Q_SIGNAL void invokeChanged(const QString&);
        Q_SIGNAL void useStoredDataChanged(bool);
        Q_SIGNAL void sendAsJsonChanged(bool);
        Q_SIGNAL void useUppercaseDataChanged(bool);
        Q_SIGNAL void templateNameChanged(const QString&);
        Q_SIGNAL void templateDataChanged(const QList<KeyValueModel>&);
        Q_SIGNAL void triggerOnNextChanged(bool);
};
