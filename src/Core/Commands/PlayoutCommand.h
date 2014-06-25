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

class CORE_EXPORT PlayoutCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit PlayoutCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        const QString& getPlayoutCommand() const;

        void setPlayoutCommand(const QString& command);

    private:
        QString command;

        Q_SIGNAL void playoutCommandChanged(const QString&);
};
