#pragma once

#include "../Shared.h"

#include <boost/property_tree/ptree.hpp>

#include <QtCore/QXmlStreamWriter>

#include <functional>
#include <vector>

using PropertyReader = std::function<void(boost::property_tree::wptree&)>;
using PropertyWriter = std::function<void(QXmlStreamWriter*)>;

class CORE_EXPORT CommandProperties
{
public:
    virtual ~CommandProperties();

    virtual void readProperties(boost::property_tree::wptree&);
    virtual void writeProperties(QXmlStreamWriter*);

    void registerReader(PropertyReader);
    void registerWriter(PropertyWriter);

private:
    std::vector<PropertyReader> readers;
    std::vector<PropertyWriter> writers;
};
