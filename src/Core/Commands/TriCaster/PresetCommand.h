#pragma once

#include "../../Shared.h"
#include "../AbstractCommand.h"

#include "Global.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QString>

class QObject;
class QXmlStreamWriter;

class CORE_EXPORT PresetCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit PresetCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter& writer);

        const QString& getSource() const;
        const QString& getPreset() const;
        bool getTriggerOnNext() const;

        void setSource(const QString& source);
        void setPreset(const QString& preset);
        void setTriggerOnNext(bool triggerOnNext);

    private:
        QString source = TriCaster::DEFAULT_SOURCE;
        QString preset = TriCaster::DEFAULT_PRESET;
        bool triggerOnNext = TriCaster::DEFAULT_TRIGGER_ON_NEXT;

        Q_SIGNAL void sourceChanged(const QString&);
        Q_SIGNAL void presetChanged(const QString&);
        Q_SIGNAL void triggerOnNextChanged(bool);
};
