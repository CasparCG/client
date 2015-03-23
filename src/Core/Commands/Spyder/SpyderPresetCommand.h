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

class CORE_EXPORT SpyderPresetCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit SpyderPresetCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        const QString& getAddress() const;
        const QString& getPort() const;
        int getPreset() const;
        bool getTriggerOnNext() const;

        void setAddress(const QString& address);
        void setPort(const QString& port);
        void setPreset(int preset);
        void setTriggerOnNext(bool triggerOnNext);

    private:
        QString address = Spyder::DEFAULT_ADDRESS;
        QString port = Spyder::DEFAULT_PORT;
        int preset = Spyder::DEFAULT_PRESET;
        bool triggerOnNext = Spyder::DEFAULT_TRIGGER_ON_NEXT;

        Q_SIGNAL void addressChanged(const QString&);
        Q_SIGNAL void portChanged(const QString&);
        Q_SIGNAL void presetChanged(int);
        Q_SIGNAL void triggerOnNextChanged(bool);
};
