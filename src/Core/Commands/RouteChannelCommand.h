#pragma once

#include "../Shared.h"
#include "AbstractCommand.h"

#include "Global.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QString>

class QObject;
class QXmlStreamWriter;

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
        int fromChannel = Route::DEFAULT_FROM_CHANNEL;

        Q_SIGNAL void fromChannelChanged(int);
};
