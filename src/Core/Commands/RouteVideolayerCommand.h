#pragma once

#include "../Shared.h"
#include "AbstractCommand.h"

#include "Global.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QString>

class QObject;
class QXmlStreamWriter;

class CORE_EXPORT RouteVideolayerCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit RouteVideolayerCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter& writer);

        int getFromChannel() const;
        int getFromVideolayer() const;
        int getOutputDelay() const;

        void setFromChannel(int fromChannel);
        void setFromVideolayer(int fromVideolayer);
        void setOutputDelay(int delay);

    private:
        int fromChannel = Route::DEFAULT_FROM_CHANNEL;
        int fromVideolayer = Route::DEFAULT_FROM_VIDEOLAYER;
        int outputDelay = Route::DEFAULT_OUTPUT_DELAY;

        Q_SIGNAL void fromChannelChanged(int);
        Q_SIGNAL void fromVideolayerChanged(int);
        Q_SIGNAL void outputDelayChanged(int);
};
