#pragma once

#include "../../Shared.h"
#include "../AbstractCommand.h"

#include "Global.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QObject>
#include <QtCore/QString>

class QXmlStreamWriter;

class CORE_EXPORT PanasonicPresetCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit PanasonicPresetCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter& writer);

        const QString& getAddress() const;
        int getPreset() const;
        bool getTriggerOnNext() const;

        void setAddress(const QString& address);
        void setPreset(int preset);
        void setTriggerOnNext(bool triggerOnNext);

    private:
        QString address = Panasonic::DEFAULT_ADDRESS;
        int preset = Panasonic::DEFAULT_PRESET;
        bool triggerOnNext = Panasonic::DEFAULT_TRIGGER_ON_NEXT;

        Q_SIGNAL void addressChanged(const QString&);
        Q_SIGNAL void presetChanged(int);
        Q_SIGNAL void triggerOnNextChanged(bool);
};
