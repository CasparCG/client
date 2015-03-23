#pragma once

#include "../Shared.h"
#include "AbstractCommand.h"

#include "Global.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QString>

class QObject;
class QXmlStreamWriter;

class CORE_EXPORT OscOutputCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit OscOutputCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        const QString& getOutput() const;
        const QString& getPath() const;
        const QString& getMessage() const;
        const QString& getType() const;
        bool getTriggerOnNext() const;
        bool getUseBundle() const;

        void setOutput(const QString& output);
        void setPath(const QString& path);
        void setMessage(const QString& message);
        void setType(const QString& type);
        void setTriggerOnNext(bool triggerOnNext);
        void setUseBundle(bool useBundle);

    private:
        QString output = Osc::DEFAULT_OUTPUT;
        QString path = "";
        QString message = "";
        QString type = Osc::DEFAULT_TYPE;
        bool triggerOnNext = Osc::DEFAULT_TRIGGER_ON_NEXT;
        bool useBundle = Osc::DEFAULT_USE_BUNDLE;

        Q_SIGNAL void outputChanged(const QString&);
        Q_SIGNAL void pathChanged(const QString&);
        Q_SIGNAL void messageChanged(const QString&);
        Q_SIGNAL void typeChanged(const QString&);
        Q_SIGNAL void triggerOnNextChanged(bool);
        Q_SIGNAL void useBundleChanged(bool);
};
