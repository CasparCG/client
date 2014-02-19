#pragma once

#include "../../Shared.h"
#include "../AbstractCommand.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QXmlStreamWriter>

class CORE_EXPORT NetworkSourceCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit NetworkSourceCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        const QString& getSource() const;
        const QString& getTarget() const;
        bool getTriggerOnNext() const;

        void setSource(const QString& source);
        void setTarget(const QString& target);
        void setTriggerOnNext(bool triggerOnNext);

    private:
        QString source;
        QString target;
        bool triggerOnNext;

        Q_SIGNAL void sourceChanged(const QString&);
        Q_SIGNAL void targetChanged(const QString&);
        Q_SIGNAL void triggerOnNextChanged(bool);
};
