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

class CORE_EXPORT AtemVideoFormatCommand: public AbstractCommand
{
    Q_OBJECT

    public:
        explicit AtemVideoFormatCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        const QString& getFormat() const;
        bool getTriggerOnNext() const;

        void setFormat(const QString& format);
        void setTriggerOnNext(bool triggerOnNext);

    private:
        QString format = Atem::DEFAULT_VIDEO_FORMAT;
        bool triggerOnNext = Atem::DEFAULT_TRIGGER_ON_NEXT;

        Q_SIGNAL void formatChanged(const QString&);
        Q_SIGNAL void triggerOnNextChanged(bool);
};
