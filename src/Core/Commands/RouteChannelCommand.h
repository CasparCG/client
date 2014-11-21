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

class CORE_EXPORT RouteChannelCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit RouteChannelCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        int getFromChannel() const;

        void setFromChannel(int fromChannel);

    private:
        int fromChannel;

        Q_SIGNAL void fromChannelChanged(int);
};
