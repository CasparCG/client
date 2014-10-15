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

class CORE_EXPORT PanasonicPresetCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit PanasonicPresetCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        const QString& getAddress() const;
        const QString& getPreset() const;
        bool getTriggerOnNext() const;

        void setAddress(const QString& address);
        void setPreset(const QString& preset);
        void setTriggerOnNext(bool triggerOnNext);

    private:
        QString address;
        QString preset;
        bool triggerOnNext;

        Q_SIGNAL void addressChanged(const QString&);
        Q_SIGNAL void presetChanged(const QString&);
        Q_SIGNAL void triggerOnNextChanged(bool);
};
