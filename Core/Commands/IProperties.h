#pragma once

#include "../Shared.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QXmlStreamWriter>

class CORE_EXPORT IProperties
{
    public:
        virtual ~IProperties() {}

        virtual void readProperties(boost::property_tree::wptree& pt) = 0;
        virtual void writeProperties(QXmlStreamWriter* writer) = 0;
};
