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

class CORE_EXPORT SonyPresetCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit SonyPresetCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter& writer);

        const QString& getAddress() const;
        int getPreset() const;
        bool getTriggerOnNext() const;

        void setAddress(const QString& address);
        void setPreset(int preset);
        void setTriggerOnNext(bool triggerOnNext);

    private:
        QString address = Sony::DEFAULT_ADDRESS;
        int preset = Sony::DEFAULT_PRESET;
        bool triggerOnNext = Sony::DEFAULT_TRIGGER_ON_NEXT;

        Q_SIGNAL void addressChanged(const QString&);
        Q_SIGNAL void presetChanged(int);
        Q_SIGNAL void triggerOnNextChanged(bool);
};
