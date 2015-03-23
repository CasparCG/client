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

class CORE_EXPORT GpiOutputCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit GpiOutputCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        int getGpoPort() const;
        bool getTriggerOnNext() const;

        void setGpoPort(int gpoPort);
        void setTriggerOnNext(bool triggerOnNext);

    private:
        int gpoPort = GpiOutput::DEFAULT_PORT;
        bool triggerOnNext = GpiOutput::DEFAULT_TRIGGER_ON_NEXT;

        Q_SIGNAL void gpoPortChanged(int);
        Q_SIGNAL void triggerOnNextChanged(bool);
};
