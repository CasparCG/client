#pragma once

#include "../Shared.h"
#include "AbstractCommand.h"

#include "Global.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QString>

class QObject;
class QXmlStreamWriter;

class CORE_EXPORT ClearOutputCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit ClearOutputCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter& writer);

        bool getClearChannel() const;
        bool getTriggerOnNext() const;

        void setClearChannel(bool clearChannel);
        void setTriggerOnNext(bool triggerOnNext);

    private:
        bool clearChannel = ClearOutput::DEFAULT_CLEAR_CHANNEL;
        bool triggerOnNext = ClearOutput::DEFAULT_TRIGGER_ON_NEXT;

        Q_SIGNAL void clearChannelChanged(bool);
        Q_SIGNAL void triggerOnNextChanged(bool);
};
