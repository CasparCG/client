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

class CORE_EXPORT PrintCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit PrintCommand(QObject* parent = 0);

        const QString& getOutput() const;
        void setOutput(const QString& output);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

    private:
        QString output;

        Q_SIGNAL void outputChanged(const QString&);
};
