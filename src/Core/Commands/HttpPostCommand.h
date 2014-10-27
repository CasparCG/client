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

class CORE_EXPORT HttpPostCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit HttpPostCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        const QString& getUrl() const;
        const QByteArray getHttpData() const;
        const QList<KeyValueModel>& getHttpDataModels() const;
        bool getTriggerOnNext() const;

        void setUrl(const QString& url);
        void setHttpDataModels(const QList<KeyValueModel>& models);
        void setTriggerOnNext(bool triggerOnNext);

    private:
        QString url;
        QList<KeyValueModel> models;
        bool triggerOnNext;

        Q_SIGNAL void urlChanged(const QString&);
        Q_SIGNAL void httpDataChanged(const QList<KeyValueModel>&);
        Q_SIGNAL void triggerOnNextChanged(bool);
};
