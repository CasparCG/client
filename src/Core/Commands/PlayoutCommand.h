#pragma once

#include "../Shared.h"
#include "AbstractCommand.h"

#include "Global.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QString>

class QObject;
class QXmlStreamWriter;

class CORE_EXPORT PlayoutCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit PlayoutCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter& writer);

        const QString& getPlayoutCommand() const;

        void setPlayoutCommand(const QString& command);

    private:
        QString command = Output::DEFAULT_PLAYOUT_COMMAND;

        Q_SIGNAL void playoutCommandChanged(const QString&);
};
