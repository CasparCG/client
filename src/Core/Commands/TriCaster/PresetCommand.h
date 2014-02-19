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

class CORE_EXPORT PresetCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit PresetCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        const QString& getSource() const;
        const QString& getPreset() const;
        bool getTriggerOnNext() const;

        void setSource(const QString& source);
        void setPreset(const QString& preset);
        void setTriggerOnNext(bool triggerOnNext);

    private:
        QString source;
        QString preset;
        bool triggerOnNext;

        Q_SIGNAL void sourceChanged(const QString&);
        Q_SIGNAL void presetChanged(const QString&);
        Q_SIGNAL void triggerOnNextChanged(bool);
};
