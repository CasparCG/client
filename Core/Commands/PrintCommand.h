#pragma once

#include "Shared.h"
#include "AbstractCommand.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QXmlStreamWriter>

class CORE_EXPORT PrintCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit PrintCommand(QObject* parent = 0);

        virtual int getDelay() const;
        virtual int getChannel() const;
        virtual int getVideolayer() const {}
        virtual bool getAllowGpi() const;

        virtual void setChannel(int channel);
        virtual void setVideolayer(int videolayer) {}
        virtual void setDelay(int delay);
        virtual void setAllowGpi(bool allowGpi);

        bool getDefer() const;
        void setDefer(bool defer);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

    private:
        int channel;
        int delay;
        bool allowGpi;
        bool defer;

        Q_SIGNAL void channelChanged(int);
        Q_SIGNAL void delayChanged(int);
        Q_SIGNAL void allowGpiChanged(bool);
        Q_SIGNAL void deferChanged(bool);
};
