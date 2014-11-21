#pragma once

#include "../Shared.h"
#include "AbstractCommand.h"
#include "Models/KeyValueModel.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QXmlStreamWriter>

class CORE_EXPORT TemplateCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit TemplateCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        int getFlashlayer() const;
        const QString& getInvoke() const;
        bool getUseStoredData() const;
        bool getUseUppercaseData() const;
        const QString& getTemplateName() const;
        const QString getTemplateData() const;
        const QList<KeyValueModel>& getTemplateDataModels() const;
        bool getTriggerOnNext() const;

        void setFlashlayer(int flashlayer); 
        void setInvoke(const QString& invoke);
        void setUseStoredData(bool useStoredData);
        void setUseUppercaseData(bool useUppercaseData);
        void setTemplateName(const QString& templateName);
        void setTemplateDataModels(const QList<KeyValueModel>& models);
        void setTriggerOnNext(bool triggerOnNext);

    private:
        int flashlayer;
        QString invoke;
        bool useStoredData;
        bool useUppercaseData;
        QString templateName;
        QList<KeyValueModel> models;
        bool triggerOnNext;

        Q_SIGNAL void flashlayerChanged(int);
        Q_SIGNAL void invokeChanged(const QString&);
        Q_SIGNAL void useStoredDataChanged(bool);
        Q_SIGNAL void useUppercaseDataChanged(bool);
        Q_SIGNAL void templateNameChanged(const QString&);
        Q_SIGNAL void templateDataChanged(const QList<KeyValueModel>&);
        Q_SIGNAL void triggerOnNextChanged(bool);
};
