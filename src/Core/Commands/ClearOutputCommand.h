#pragma once

#include "../Shared.h"
#include "AbstractCommand.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QXmlStreamWriter>

class CORE_EXPORT ClearOutputCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit ClearOutputCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        bool getClearChannel() const;
        bool getTriggerOnNext() const;

        void setClearChannel(bool clearChannel);
        void setTriggerOnNext(bool triggerOnNext);

    private:
        bool clearChannel;
        bool triggerOnNext;

        Q_SIGNAL void clearChannelChanged(bool);
        Q_SIGNAL void triggerOnNextChanged(bool);
};
