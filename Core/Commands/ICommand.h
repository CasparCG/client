#pragma once

#include "../Shared.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QXmlStreamWriter>

class CORE_EXPORT ICommand
{
    public:
        virtual ~ICommand() {}

        virtual int getDelay() const = 0;
        virtual int getChannel() const = 0;
        virtual int getVideolayer() const = 0;
        virtual bool getAllowGpi() const = 0;

        virtual void setDelay(int delay) = 0;
        virtual void setChannel(int channel) = 0;
        virtual void setVideolayer(int videoLayer) = 0;
        virtual void setAllowGpi(bool allowGpi) = 0;

        virtual void readProperties(boost::property_tree::wptree& pt) = 0;
        virtual void writeProperties(QXmlStreamWriter* writer) = 0;
};
