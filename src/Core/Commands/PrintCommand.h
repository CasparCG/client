#pragma once

#include "../Shared.h"
#include "AbstractCommand.h"

#include "Global.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QString>

class QObject;
class QXmlStreamWriter;

class CORE_EXPORT PrintCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit PrintCommand(QObject* parent = 0);

        const QString& getOutput() const;
        void setOutput(const QString& output);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter& writer);

    private:
        QString output = Print::DEFAULT_OUTPUT;

        Q_SIGNAL void outputChanged(const QString&);
};
