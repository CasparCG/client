#pragma once

#include "../Shared.h"
#include "AbstractCommand.h"
#include "Models/KeyValueModel.h"

#include "Global.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QUrlQuery>
#include <QtCore/QString>

class QObject;
class QXmlStreamWriter;

class CORE_EXPORT HttpPostCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit HttpPostCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter& writer);

        const QString& getUrl() const;
        const QUrlQuery getHttpData() const;
        const QList<KeyValueModel>& getHttpDataModels() const;
        bool getTriggerOnNext() const;

        void setUrl(const QString& url);
        void setHttpDataModels(const QList<KeyValueModel>& models);
        void setTriggerOnNext(bool triggerOnNext);

    private:
        QString url = Http::DEFAULT_URL;
        bool triggerOnNext = Http::DEFAULT_TRIGGER_ON_NEXT;

        QList<KeyValueModel> models;

        Q_SIGNAL void urlChanged(const QString&);
        Q_SIGNAL void httpDataChanged(const QList<KeyValueModel>&);
        Q_SIGNAL void triggerOnNextChanged(bool);
};
