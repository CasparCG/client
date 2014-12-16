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

class CORE_EXPORT SonyPresetCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit SonyPresetCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        const QString& getAddress() const;
        int getPreset() const;
        bool getTriggerOnNext() const;

        void setAddress(const QString& address);
        void setPreset(int preset);
        void setTriggerOnNext(bool triggerOnNext);

    private:
        QString address;
        int preset;
        bool triggerOnNext;

        Q_SIGNAL void addressChanged(const QString&);
        Q_SIGNAL void presetChanged(int);
        Q_SIGNAL void triggerOnNextChanged(bool);
};
