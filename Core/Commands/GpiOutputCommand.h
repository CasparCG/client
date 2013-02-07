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

class CORE_EXPORT GpiOutputCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit GpiOutputCommand(QObject* parent = 0);

        virtual int getDelay() const;
        virtual int getChannel() const {}
        virtual int getVideolayer() const {}
        virtual bool getAllowGpi() const;

        virtual void setChannel(int channel) {}
        virtual void setVideolayer(int videolayer) {}
        virtual void setDelay(int delay);
        virtual void setAllowGpi(bool allowGpi);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        int getGpoPort() const;
        void setGpoPort(int gpoPort);

    private:
        int delay;
        bool allowGpi;
        int gpoPort;

        Q_SIGNAL void allowGpiChanged(bool);
        Q_SIGNAL void delayChanged(int);
        Q_SIGNAL void gpoPortChanged(int);
};
