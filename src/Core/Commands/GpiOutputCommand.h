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

class CORE_EXPORT GpiOutputCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit GpiOutputCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        int getGpoPort() const;
        void setGpoPort(int gpoPort);

    private:
        int gpoPort;

        Q_SIGNAL void gpoPortChanged(int);
};
