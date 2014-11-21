#pragma once

#include "../Shared.h"
#include "AbstractCommand.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QXmlStreamWriter>

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
        QString output;
        QString path;
        QString message;
        QString type;
        bool triggerOnNext;
        bool useBundle;

        Q_SIGNAL void outputChanged(const QString&);
        Q_SIGNAL void pathChanged(const QString&);
        Q_SIGNAL void messageChanged(const QString&);
        Q_SIGNAL void typeChanged(const QString&);
        Q_SIGNAL void triggerOnNextChanged(bool);
        Q_SIGNAL void useBundleChanged(bool);
};
