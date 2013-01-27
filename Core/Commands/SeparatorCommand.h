#pragma once

#include "Shared.h"
#include "ICommand.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QXmlStreamWriter>

class CORE_EXPORT SeparatorCommand : public QObject, public ICommand
{
    Q_OBJECT

    public:
        explicit SeparatorCommand(QObject* parent = 0);

        virtual int getDelay() const {}
        virtual int getChannel() const {}
        virtual int getVideolayer() const {}
        virtual bool getAllowGpi() const {}

        virtual void setChannel(int channel) {}
        virtual void setVideolayer(int videolayer) {}
        virtual void setDelay(int delay) {}
        virtual void setAllowGpi(bool allowGpi) {}

        virtual void readProperties(boost::property_tree::wptree& pt) {}
        virtual void writeProperties(QXmlStreamWriter* writer) {}
};
