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

class CORE_EXPORT OscOutputCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit OscOutputCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        const QString& getOutput() const;
        const QString& getPath() const;
        const QString& getMessage() const;

        void setOutput(const QString& output);
        void setPath(const QString& path);
        void setMessage(const QString& message);

    private:
        QString output;
        QString path;
        QString message;

        Q_SIGNAL void outputChanged(const QString&);
        Q_SIGNAL void pathChanged(const QString&);
        Q_SIGNAL void messageChanged(const QString&);
};
