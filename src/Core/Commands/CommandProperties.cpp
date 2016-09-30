#include "CommandProperties.h"

#include <utility> // std::move

CommandProperties::~CommandProperties() { }

void CommandProperties::readProperties(boost::property_tree::wptree& pt)
{
    for(auto& reader : readers) reader(pt);
}

void CommandProperties::writeProperties(QXmlStreamWriter* sw)
{
    for(auto& writer : writers) writer(sw);
}

void CommandProperties::registerReader(PropertyReader reader)
{
    readers.push_back(std::move(reader));
}

void CommandProperties::registerWriter(PropertyWriter writer)
{
    writers.push_back(std::move(writer));
}
