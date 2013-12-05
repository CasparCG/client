#pragma once

#include "../../Shared.h"
#include "../AbstractCommand.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QXmlStreamWriter>

class CORE_EXPORT InputCommand: public AbstractCommand
{
    Q_OBJECT

    public:
        explicit InputCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        const QString& getSwitcher() const;
        const QString& getInput() const;

        void setSwitcher(const QString& switcher);
        void setInput(const QString& input);

    private:
        QString switcher;
        QString input;

        Q_SIGNAL void switcherChanged(const QString&);
        Q_SIGNAL void inputChanged(const QString&);
};
