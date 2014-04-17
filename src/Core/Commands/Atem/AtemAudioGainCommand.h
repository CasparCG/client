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

class CORE_EXPORT AtemAudioGainCommand: public AbstractCommand
{
    Q_OBJECT

    public:
        explicit AtemAudioGainCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        const QString& getSource() const;
        float getGain() const;
        bool getTriggerOnNext() const;

        void setSource(const QString& source);
        void setGain(float gain);
        void setTriggerOnNext(bool triggerOnNext);

    private:
        QString source;
        float gain;
        bool triggerOnNext;

        Q_SIGNAL void sourceChanged(const QString&);
        Q_SIGNAL void gainChanged(float);
        Q_SIGNAL void triggerOnNextChanged(bool);
};
