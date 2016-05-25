#include "AbstractProperties.h"

#include <utility> // std::move

AbstractProperties::~AbstractProperties()
{
}

void AbstractProperties::readProperties(boost::property_tree::wptree& pt)
{
    for(auto& reader : readers) reader(pt);
}

void AbstractProperties::writeProperties(QXmlStreamWriter* sw)
{
    for(auto& writer : writers) writer(sw);
}

void AbstractProperties::registerReader(PropertyReader reader)
{
    readers.push_back(std::move(reader));
}

void AbstractProperties::registerWriter(PropertyWriter writer)
{
    writers.push_back(std::move(writer));
}
