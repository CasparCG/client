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

class CORE_EXPORT AtemAudioInputStateCommand: public AbstractCommand
{
    Q_OBJECT

    public:
        explicit AtemAudioInputStateCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        const QString& getInput() const;
        const QString& getState() const;
        bool getTriggerOnNext() const;

        void setInput(const QString& input);
        void setState(const QString& state);
        void setTriggerOnNext(bool triggerOnNext);

    private:
        QString input;
        QString state;
        bool triggerOnNext;

        Q_SIGNAL void inputChanged(const QString&);
        Q_SIGNAL void stateChanged(const QString&);
        Q_SIGNAL void triggerOnNextChanged(bool);
};
