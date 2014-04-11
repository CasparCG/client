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

class CORE_EXPORT AtemKeyerStateCommand: public AbstractCommand
{
    Q_OBJECT

    public:
        explicit AtemKeyerStateCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        const QString& getKeyer() const;
        bool getState() const;
        bool getTriggerOnNext() const;

        void setKeyer(const QString& keyer);
        void setState(bool state);
        void setTriggerOnNext(bool triggerOnNext);

    private:
        QString keyer;
        bool state;
        bool triggerOnNext;

        Q_SIGNAL void keyerChanged(const QString&);
        Q_SIGNAL void stateChanged(bool);
        Q_SIGNAL void triggerOnNextChanged(bool);
};
