#pragma once

#include "../Shared.h"
#include "AbstractCommand.h"

#include "Global.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QString>

class QObject;
class QXmlStreamWriter;

class CORE_EXPORT KeyerCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit KeyerCommand(QObject* parent = 0);

        bool getDefer() const;
        void setDefer(bool defer);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter& writer);

    private:
        bool defer = Mixer::DEFAULT_DEFER;

        Q_SIGNAL void deferChanged(bool);
};
